if( CONFIG_TARGET_WINDOWS_SIMULATOR )
    add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
endif()

if( CONFIG_TARGET_ARM_MPS2_AN385 )
    set( GCC_M_CPU cortex-m3 )
    set( GCC_M_ARCH armv7-m )

    add_link_options( -T ${CMAKE_CURRENT_LIST_DIR}/arm/mps2_cm3_an385/bsp/gcc_arm.ld
                      -specs=nano.specs
                      -specs=rdimon.specs)

    include( ${CMAKE_CURRENT_LIST_DIR}/arm/gcc-arm-none-eabi.cmake )

    set( CMAKE_EXECUTABLE_SUFFIX_C .elf )
endif()
