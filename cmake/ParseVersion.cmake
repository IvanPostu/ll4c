function(parse_changelog_version)
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

  if(LATEST_VERSION MATCHES "^([0-9]+)\\.([0-9]+)\\.([0-9]+)$")
      set(VERSION_MAJOR "${CMAKE_MATCH_1}" PARENT_SCOPE)
      set(VERSION_MINOR "${CMAKE_MATCH_2}" PARENT_SCOPE)
      set(VERSION_PATCH "${CMAKE_MATCH_3}" PARENT_SCOPE)
  else()
      message(FATAL_ERROR "Parsed version string '${LATEST_VERSION}' does not follow semantic versioning (MAJOR.MINOR.PATCH)!")
  endif()
endfunction()
