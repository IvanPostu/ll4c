cmake_minimum_required(VERSION 3.9.2)

# include utilities
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(ParseVersion)
include(EnableWarnings)

parse_changelog_version()
set(FULL_VERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")
message(STATUS "Version: ${FULL_VERSION}")

# set project name
project(ll4c
  VERSION ${FULL_VERSION}
  DESCRIPTION "Logging library for C"
  HOMEPAGE_URL "https://github.com/IvanPostu/ll4c"
  LANGUAGES C)

option(BUILD_TESTBED "Build the testbed executable" OFF)
option(INCLUDE_DEBUG_SYMBOLS "Include debug symbols in the build" OFF)

# configure requirements
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# setup target and directory names
set(INCLUDE_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/include")
set(SOURCE_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(TEST_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/test")

add_library(ll4c
  "${INCLUDE_DIRECTORY}/ll4c.h"
  "${SOURCE_DIRECTORY}/ll4c.c")
target_compile_definitions(ll4c PRIVATE
  APP_VERSION_MAJOR="${VERSION_MAJOR}"
  APP_VERSION_MINOR="${VERSION_MINOR}"
  APP_VERSION_PATCH="${VERSION_PATCH}"
)
target_include_directories(ll4c PUBLIC
  ${INCLUDE_DIRECTORY}
)
set_target_properties(ll4c PROPERTIES PUBLIC_HEADER "${INCLUDE_DIRECTORY}/ll4c.h")
set_target_properties(ll4c PROPERTIES DEFINE_SYMBOL API_LL4C)
enable_warnings(ll4c)
if(INCLUDE_DEBUG_SYMBOLS)
  target_compile_options(ll4c PRIVATE -g -O0)
endif()

if(BUILD_TESTBED)
  add_executable(ll4c_testbed testbed/src/main.c)
  target_link_libraries(ll4c_testbed PRIVATE ll4c)
  enable_warnings(ll4c_testbed)
  if(INCLUDE_DEBUG_SYMBOLS)
    target_compile_options(ll4c_testbed PRIVATE -g -O0)
  endif()
endif()
