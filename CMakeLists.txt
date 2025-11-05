cmake_minimum_required(VERSION 3.10)
project(MachineLanguage VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Menemukan paket Qt yang diperlukan
find_package(Qt6 COMPONENTS Widgets REQUIRED)

# Otomatis menangani file yang memerlukan MOC, Uic, Rcc dari Qt
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Menambahkan folder 'include'
include_directories(include)

# --- Konfigurasi PortAudio ---
set(PA_BUILD_STATIC OFF CACHE BOOL "Build static PortAudio library")
set(PA_BUILD_SHARED ON CACHE BOOL "Build shared PortAudio library")
set(PA_USE_WASAPI ON CACHE BOOL "Use WASAPI for Windows")
add_subdirectory(lib/portaudio)
include_directories(lib/portaudio/include)

# --- Menyiapkan KissFFT ---
include_directories(lib/kissfft)

# Membuat executable dari semua file .cpp kita
add_executable(MachineLanguage
    mainwindow.h
    mainwindow.cpp
    main.cpp
    Penerjemah.h
    Penerjemah.cpp
    Trie.h
    Trie.cpp
    audioprocessor.h
    audioprocessor.cpp
    # Tambahkan file C dari KissFFT untuk dikompilasi
    lib/kissfft/kiss_fft.c
    lib/kissfft/kiss_fftr.c
)

# Menautkan library yang dibutuhkan
target_link_libraries(MachineLanguage
    PRIVATE
    Qt6::Widgets
    portaudio
    # Tautkan dengan library 'm' untuk fungsi matematika (seperti log10)
    m
)

# Perintah untuk menyalin kamus.txt ke folder build
add_custom_command(TARGET MachineLanguage POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    "${CMAKE_SOURCE_DIR}/kamus.txt"
    "${CMAKE_BINARY_DIR}/kamus.txt"
)
