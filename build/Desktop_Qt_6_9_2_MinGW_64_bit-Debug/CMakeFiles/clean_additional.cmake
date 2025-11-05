# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\MachineLanguage_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\MachineLanguage_autogen.dir\\ParseCache.txt"
  "MachineLanguage_autogen"
  "lib\\portaudio\\CMakeFiles\\portaudio_autogen.dir\\AutogenUsed.txt"
  "lib\\portaudio\\CMakeFiles\\portaudio_autogen.dir\\ParseCache.txt"
  "lib\\portaudio\\portaudio_autogen"
  )
endif()
