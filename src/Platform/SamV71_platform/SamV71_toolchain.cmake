#
# Copyright (C) 2024 Integrated Detector Electronics AS
# All Rights Reserved.
#
# NOTICE: All information contained herein is, and remains
# the property of Integrated Detector Electronics AS and its suppliers,
# if any. The intellectual and technical concepts contained
# herein are proprietary to Integrated Detector Electronics AS
# and its suppliers and may be covered by Norwegian, EU. or U.S. patents,
# patents in process, and are protected by trade secret or copyright law.
# Dissemination of this information or reproduction of this material
# is strictly forbidden unless prior written permission is obtained
# from Integrated Detector Electronics AS.
#

if (DEFINED GNU_VERSION)

    if (${GNU_VERSION} STREQUAL "2019-Q3")
        set(GNU_TOOL_HOME "C:/Program Files (x86)/GNU Tools ARM Embedded/8 2019-q3-update")
    elseif (${GNU_VERSION} STREQUAL "2019-Q4")
        set(GNU_TOOL_HOME "C:/Program Files (x86)/GNU Tools Arm Embedded/9 2019-q4-major")
    elseif (${GNU_VERSION} STREQUAL "2020-Q2")
        set(GNU_TOOL_HOME "C:/Program Files (x86)/GNU Arm Embedded Toolchain/9 2020-q2-update")
    elseif (${GNU_VERSION} STREQUAL "2020-Q4")
        set(GNU_TOOL_HOME "C:/Program Files (x86)/GNU Arm Embedded Toolchain/10 2020-q4-major")
    elseif (${GNU_VERSION} STREQUAL "2022-02")
        set(GNU_TOOL_HOME "C:/Program Files (x86)/Arm GNU Toolchain arm-none-eabi/11.2 2022.02")
    elseif (${GNU_VERSION} STREQUAL "12.2.rel1")
        set(GNU_TOOL_HOME "C:/Program Files (x86)/Arm GNU Toolchain arm-none-eabi/12.2 rel1")
    elseif (${GNU_VERSION} STREQUAL "13.2.rel1")
        set(GNU_TOOL_HOME "C:/Program Files (x86)/Arm GNU Toolchain arm-none-eabi/13.2 rel1")
    elseif (${GNU_VERSION} STREQUAL "GNU_ANY")
        set(GNU_TOOL_HOME "C:/Program Files (x86)/Arm GNU Toolchain arm-none-eabi/13.2 rel1")
    else ()

        message(FATAL_ERROR " <> Sorry, the GNU_VERSION='${GNU_VERSION}' is not supported <> ")

    endif ()

    if (EXISTS ${GNU_TOOL_HOME})
        set(CMAKE_CROSSCOMPILING 1)
        set(CMAKE_SYSTEM_NAME Generic)
        set(CMAKE_SYSTEM_PROCESSOR arm)
        # Use nosys_libc for the CMake test compilation
        set(CMAKE_EXE_LINKER_FLAGS --specs=nosys.specs)

        set(CMAKE_ASM_COMPILER ${GNU_TOOL_HOME}/bin/arm-none-eabi-gcc.exe)
        set(CMAKE_C_COMPILER ${GNU_TOOL_HOME}/bin/arm-none-eabi-gcc.exe)
        set(CMAKE_CXX_COMPILER ${GNU_TOOL_HOME}/bin/arm-none-eabi-g++.exe)
        set(CMAKE_LINKER ${GNU_TOOL_HOME}/bin/arm-none-eabi-gcc.exe)
        set(OBJCOPY ${GNU_TOOL_HOME}/bin/arm-none-eabi-objcopy.exe)
        set(OBJDUMP ${GNU_TOOL_HOME}/bin/arm-none-eabi-objdump.exe)
    else ()

        message(FATAL_ERROR " <> Sorry, the GNU_TOOL_HOME='${GNU_TOOL_HOME}' is not installed <> ")

    endif ()

    set(HEXBIN "${CMAKE_SOURCE_DIR}/lib/hexbin/hexbin.exe")

else ()

    message(FATAL_ERROR " <> GNU_VERSION is not defined <> ")

endif ()
