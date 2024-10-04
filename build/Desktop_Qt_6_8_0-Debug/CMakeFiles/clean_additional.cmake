# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles/El_Canal_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/El_Canal_autogen.dir/ParseCache.txt"
  "El_Canal_autogen"
  )
endif()
