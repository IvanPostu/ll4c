#include "cmd2_async_log_worker.h"
#include <assert.h>
#include <bits/pthreadtypes.h>
#include <errno.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void handleCmd2(void);

struct _1dLinkedListNode {
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

void mutexInit(pthread_mutex_t *mutex) {
  if (pthread_mutex_init(mutex, NULL) != 0) {
    perror("Mutex initialization failed");
    exit(1);
  }
}

void mutexRelease(pthread_mutex_t *mutex) {
  pthread_mutex_destroy(mutex);
}

// 0 OK
// 1 interruption
// 2 unknown error
int c_sleep(unsigned long millis) {
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

void *log_worker(void *arg) {
  LogWorkerState *state = (LogWorkerState *)arg;

  bool isLoopOngoing = true;
  while (isLoopOngoing) {
    int sleepRes = c_sleep(500L);

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
        printf("%s", curr->payload);
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

void handleCmd2(void) {
  LinkedListBox published;
  published.head = published.tail = NULL;
  mutexInit(&published.mutex);
  LinkedListBox toBeLogged;
  toBeLogged.head = toBeLogged.tail = NULL;
  mutexInit(&toBeLogged.mutex);

  LogWorkerState *logWorkerState = malloc(sizeof(LogWorkerState));
  logWorkerState->execution.isInterrupted = false;
  logWorkerState->execution.isShutdown = false;
  mutexInit(&logWorkerState->execution.mutex);
  logWorkerState->published = &published;
  logWorkerState->toBeLogged = &toBeLogged;

  {
    pthread_t worker_thread;
    int resultCode = pthread_create(&worker_thread, 0, log_worker, (void *)logWorkerState);
    assert(resultCode == 0);

    char input[100];
    while (true) {
      if (fgets(input, sizeof(input), stdin) == NULL) {
        break;
      }

      input[strcspn(input, "\n")] = '\0';

      if (strlen(input) == 0) {
        pthread_mutex_lock(&logWorkerState->published->mutex);
        {
          for (int i = 0; i < 50; i++) {
            struct _1dLinkedListNode *current = malloc(sizeof(struct _1dLinkedListNode));

            char *buff = malloc(256);
            snprintf(buff, 256, "%d) %s\n", i, "example");
            current->payload = buff;
            current->next = NULL;

            if (logWorkerState->published->head == NULL && logWorkerState->published->tail == NULL) {
              logWorkerState->published->head = logWorkerState->published->tail = current;
              continue;
            }

            logWorkerState->published->tail->next = current;
            logWorkerState->published->tail = logWorkerState->published->tail->next;
          }
        }
        pthread_mutex_unlock(&logWorkerState->published->mutex);
      }

      if (strcmp(input, "1") == 0) {
        break;
      }
    }

    pthread_mutex_lock(&logWorkerState->execution.mutex);
    {
      if (!logWorkerState->execution.isShutdown) {
        logWorkerState->execution.isShutdown = true;
      }
    }
    pthread_mutex_unlock(&logWorkerState->execution.mutex);

    resultCode = pthread_join(worker_thread, 0);
    assert(resultCode == 0);
  }

  mutexRelease(&published.mutex);
  mutexRelease(&toBeLogged.mutex);
  mutexRelease(&logWorkerState->execution.mutex);
  free(logWorkerState);
}
