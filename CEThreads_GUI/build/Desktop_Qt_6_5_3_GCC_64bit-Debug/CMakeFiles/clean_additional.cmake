# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CEThreads_GUI_autogen"
  "CMakeFiles/CEThreads_GUI_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/CEThreads_GUI_autogen.dir/ParseCache.txt"
  )
endif()
