# Host toolchain for PLATFORM=Win11: a native MinGW-w64 GCC with C++20 module support (GCC >= 14).
#
# CLion supplies the compiler through its toolchain setting. From the command line, either put MinGW's
# bin directory on PATH or pass -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++. When nothing is given,
# fall back to the MinGW bundled with CLion if it is installed.

if (NOT CMAKE_CXX_COMPILER AND NOT DEFINED ENV{CXX})
    file(GLOB _CLION_MINGW_GXX "C:/Program Files/JetBrains/CLion*/bin/mingw/bin/g++.exe")
    if (_CLION_MINGW_GXX)
        list(SORT _CLION_MINGW_GXX ORDER DESCENDING)
        list(GET _CLION_MINGW_GXX 0 _GXX)
        get_filename_component(_MINGW_BIN ${_GXX} DIRECTORY)
        set(CMAKE_C_COMPILER ${_MINGW_BIN}/gcc.exe)
        set(CMAKE_CXX_COMPILER ${_MINGW_BIN}/g++.exe)
    endif ()
endif ()
