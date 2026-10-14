#include "ll4c_async.h"
#include "ll4c.h"

#include <assert.h>
#include <bits/pthreadtypes.h>
#include <errno.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct _1dLinkedListNode {
  LogLevel logLevel;
  const char *codeFilename;
  int codeLine;
  char *payload;
  struct _1dLinkedListNode *next;
};

typedef struct {
  struct _1dLinkedListNode *head;
  struct _1dLinkedListNode *tail;
  pthread_mutex_t mutex;
} LinkedListBox;

typedef struct {
  struct {
    bool isInterrupted;
    bool isShutdown;
    pthread_mutex_t mutex;
  } execution;

  LinkedListBox *published;
  LinkedListBox *toBeLogged;
} LogWorkerState;

struct {
  bool isInitialized;
  LogWorkerState *logWorkerState;
  PtrLogCallback logCallback;
  unsigned long waitMs;
  pthread_t worker_thread;
} _GlobalState;

static void mutexInit(pthread_mutex_t *mutex) {
  int result = pthread_mutex_init(mutex, NULL);
  if (result != 0) {
    fprintf(stderr, "Failed to initialize mutex: %s (Error code: %d)\n", strerror(result), result);
  }
}

static void mutexRelease(pthread_mutex_t *mutex) {
  int result = pthread_mutex_destroy(mutex);
  if (result != 0) {
    fprintf(stderr, "Failed to destroy locked mutex: %s (Error code: %d)\n", strerror(result), result);
  }
}

// 0 OK
// 1 interruption
// 2 unknown error
static int c_sleep(unsigned long millis) {
  struct timespec req = {0, millis * 1000000L};
  struct timespec rem;
  if (nanosleep(&req, &rem) == -1) {
    if (errno == EINTR) {
      return 1;
    } else {
      return 2;
    }
  }
  return 0;
}

static void *runLogWorker(void *arg) {
  LogWorkerState *state = (LogWorkerState *)arg;

  bool isLoopOngoing = true;
  while (isLoopOngoing) {
    int sleepRes = c_sleep(_GlobalState.waitMs);

    // handle interruption and sleep error
    if (sleepRes > 0) {
      pthread_mutex_lock(&state->execution.mutex);
      {
        state->execution.isInterrupted = sleepRes == 1;
        state->execution.isShutdown = true;
        isLoopOngoing = false;
      }
      pthread_mutex_unlock(&state->execution.mutex);
    }

    // handle external loop stop
    if (isLoopOngoing) {
      pthread_mutex_lock(&state->execution.mutex);
      {
        if (state->execution.isShutdown) {
          isLoopOngoing = false;
        }
      }
      pthread_mutex_unlock(&state->execution.mutex);
    }

    // copy published to toBeLogged and nullify published
    pthread_mutex_lock(&state->published->mutex);
    {
      if (state->published->head != NULL && state->published->tail != NULL) {
        pthread_mutex_lock(&state->toBeLogged->mutex);
        {
          if (state->toBeLogged->head == NULL && state->toBeLogged->tail == NULL) {
            state->toBeLogged->head = state->published->head;
            state->toBeLogged->tail = state->published->tail;
            state->published->head = NULL;
            state->published->tail = NULL;
          } else {
            state->toBeLogged->tail->next = state->published->head;
            state->toBeLogged->tail = state->published->tail;
            state->published->head = NULL;
            state->published->tail = NULL;
          }
        }
        pthread_mutex_unlock(&state->toBeLogged->mutex);
      }
    }
    pthread_mutex_unlock(&state->published->mutex);

    // log toBeLogged & clean it
    pthread_mutex_lock(&state->toBeLogged->mutex);
    {
      struct _1dLinkedListNode *curr;
      while ((curr = state->toBeLogged->head) != NULL) {

        _GlobalState.logCallback(curr->logLevel, curr->codeFilename, curr->codeLine, curr->payload);

        free(curr->payload);

        state->toBeLogged->head = state->toBeLogged->head->next;
        free(curr);
      }
      state->toBeLogged->tail = NULL;
    }
    pthread_mutex_unlock(&state->toBeLogged->mutex);
  }
  return 0;
}

void startup_ThreadUnsafe(PtrLogCallback logCallback, unsigned long waitMs) {
  if (_GlobalState.isInitialized) {
    return;
  }

  LinkedListBox *published = malloc(sizeof(LinkedListBox));
  published->head = published->tail = NULL;
  mutexInit(&published->mutex);
  LinkedListBox *toBeLogged = malloc(sizeof(LinkedListBox));
  toBeLogged->head = toBeLogged->tail = NULL;
  mutexInit(&toBeLogged->mutex);

  LogWorkerState *logWorkerState = malloc(sizeof(LogWorkerState));
  logWorkerState->execution.isInterrupted = false;
  logWorkerState->execution.isShutdown = false;
  mutexInit(&logWorkerState->execution.mutex);
  logWorkerState->published = published;
  logWorkerState->toBeLogged = toBeLogged;

  pthread_t *worker_thread = &_GlobalState.worker_thread;

  int resultCode = pthread_create(worker_thread, 0, runLogWorker, (void *)logWorkerState);
  assert(resultCode == 0);

  _GlobalState.logWorkerState = logWorkerState;
  _GlobalState.logCallback = logCallback;
  _GlobalState.waitMs = waitMs;
  _GlobalState.isInitialized = true;
}

void shutdown_ThreadUnsafe(void) {
  LogWorkerState *logWorkerState = _GlobalState.logWorkerState;
  pthread_t *worker_thread = &_GlobalState.worker_thread;
  LinkedListBox *published = logWorkerState->published;
  LinkedListBox *toBeLogged = logWorkerState->toBeLogged;

  pthread_mutex_lock(&logWorkerState->execution.mutex);
  {
    if (!logWorkerState->execution.isShutdown) {
      logWorkerState->execution.isShutdown = true;
    }
  }
  pthread_mutex_unlock(&logWorkerState->execution.mutex);

  int resultCode = pthread_join(*worker_thread, 0);
  assert(resultCode == 0);

  mutexRelease(&published->mutex);
  mutexRelease(&toBeLogged->mutex);
  free(published);
  free(toBeLogged);

  mutexRelease(&logWorkerState->execution.mutex);
  free(logWorkerState);

  _GlobalState.logWorkerState = NULL;
  _GlobalState.logCallback = NULL;
  _GlobalState.waitMs = 0;
  _GlobalState.isInitialized = false;
}

void publishLogMessage(
    LogLevel level,
    const char *codeFilename,
    int codeLine,
    char *formattedMessage) {
  LogWorkerState *logWorkerState = _GlobalState.logWorkerState;
  if (logWorkerState == NULL) {
    return;
  }

  pthread_mutex_lock(&logWorkerState->published->mutex);
  {
    struct _1dLinkedListNode *current = malloc(sizeof(struct _1dLinkedListNode));

    current->payload = formattedMessage;
    current->codeFilename = codeFilename;
    current->codeLine = codeLine;
    current->logLevel = level;
    current->next = NULL;

    if (logWorkerState->published->head == NULL && logWorkerState->published->tail == NULL) {
      logWorkerState->published->head = logWorkerState->published->tail = current;
    } else {
      logWorkerState->published->tail->next = current;
      logWorkerState->published->tail = logWorkerState->published->tail->next;
    }
  }
  pthread_mutex_unlock(&logWorkerState->published->mutex);
}
