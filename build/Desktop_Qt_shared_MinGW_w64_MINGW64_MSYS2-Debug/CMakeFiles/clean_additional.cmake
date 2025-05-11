# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\KoridorsGame_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\KoridorsGame_autogen.dir\\ParseCache.txt"
  "KoridorsGame_autogen"
  )
endif()
