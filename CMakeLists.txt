cmake_minimum_required(VERSION 3.9.2)

# set project name
project(ll4c
  VERSION 0.0.1
  DESCRIPTION "A logging library for C"
  HOMEPAGE_URL "https://github.com/IvanPostu/ll4c"
  LANGUAGES C)

option(BUILD_TESTBED "Build the testbed executable" OFF)

# configure requirements
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# setup target and directory names
set(INCLUDE_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/include")
set(SOURCE_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(TEST_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/test")

execute_process(
  COMMAND awk -F[][] "/^## \\[[0-9]/ { print $2; exit }" "${CMAKE_CURRENT_SOURCE_DIR}/CHANGELOG.MD"
  OUTPUT_VARIABLE LATEST_VERSION
  OUTPUT_STRIP_TRAILING_WHITESPACE
  ERROR_VARIABLE AWK_ERROR
  RESULT_VARIABLE AWK_CMD_RETURN_CODE
)
if(NOT AWK_CMD_RETURN_CODE EQUAL 0 OR LATEST_VERSION STREQUAL "")
  message(FATAL_ERROR "Failed to parse version from CHANGELOG.md!\nError output: ${AWK_ERROR}")
endif()

message(STATUS "Building version: ${LATEST_VERSION}")

add_library(ll4c
  "${INCLUDE_DIRECTORY}/ll4c.h"
  "${SOURCE_DIRECTORY}/ll4c.c")
target_include_directories(ll4c PUBLIC
  ${INCLUDE_DIRECTORY}
)
set_target_properties(ll4c PROPERTIES PUBLIC_HEADER "${INCLUDE_DIRECTORY}/ll4c.h")
set_target_properties(ll4c PROPERTIES DEFINE_SYMBOL API_LL4C)

if(BUILD_TESTBED)
  add_executable(ll4c_testbed testbed/src/main.c)
  target_link_libraries(ll4c_testbed PRIVATE ll4c)
  target_compile_definitions(ll4c_testbed PRIVATE
    APP_VERSION="${LATEST_VERSION}"
  )
endif()
