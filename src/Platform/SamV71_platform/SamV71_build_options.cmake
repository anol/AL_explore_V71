target_compile_options(${THE_TARGET_NAME} PUBLIC
        # Enable all warnings
        -Wall
        # Kernel development mode (-mkernel)
        -fno-exceptions
        $<$<COMPILE_LANGUAGE:CXX>:-fno-rtti>
        # Override the automatic link-time optimization (lto)
        -fno-lto
        # No dead code elimination (dce) or dead store elimination (dse)
        #        -fno-dce
        #        -fno-dse
        #        -fno-tree-dce
        #        -fno-tree-dse
        # To reduce image size
        -ffunction-sections
        -fdata-sections
        # Target architecture
        #        -mabi=aapcs
        -mthumb
        -march=armv7-m
        #        -march=armv7e-m+fp.dp
        #        -mfloat-abi=hard
        --specs=nano.specs
        # When target outside 64-megabyte addr range
        #        -mlong-calls
)

target_compile_definitions(${THE_TARGET_NAME} PUBLIC
        #        -DARM_MATH_CM7=true
        -Dscanf=iscanf
        -Dprintf=iprintf
        -D__SAMV71Q21B__

        -DBOARD=SAMV71_XPLAINED_ULTRA

        #        -D__FPU_PRESENT
)

target_link_options(${THE_TARGET_NAME} PUBLIC
        -mthumb
        -march=armv7-m
        #-march=armv7e-m+fp.dp
        #        -mfloat-abi=hard
        #        -Wl,--library-path=${CMAKE_BINARY_DIR}
        #        -Wl,--library-path=${CMAKE_CURRENT_SOURCE_DIR}
        #        -static
        --specs=nano.specs
        # Enable garbage collection of unused input sections
        -Wl,--gc-sections
        # Diagnostics
        -Wl,-Map=${PROJECT_NAME}.map
        -Wl,--cref
        -Wl,--print-memory-usage
        #        -v
)
