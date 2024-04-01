# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles/MDP_GUI_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/MDP_GUI_autogen.dir/ParseCache.txt"
  "MDP_GUI_autogen"
  )
endif()
