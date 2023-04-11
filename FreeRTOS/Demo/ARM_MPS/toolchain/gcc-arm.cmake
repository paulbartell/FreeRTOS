###############################################################################
# FreeRTOS
# Copyright (C) 2023 Amazon.com, Inc. or its affiliates. All Rights Reserved.
# SPDX-License-Identifier: MIT
###############################################################################

set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

if(DEFINED TOOLCHAIN_PATH)
    set(TC_BIN_PATH                     ${TOOLCHAIN_PATH}/bin/)
else()
    set(TC_BIN_PATH                     "")
endif()

set(TC_PREFIX arm-none-eabi-)

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)

set(TC_SUFFIX                           ${CMAKE_EXECUTABLE_SUFFIX})

set(CMAKE_C_COMPILER_WORKS              TRUE)
set(CMAKE_CXX_COMPILER_WORKS            TRUE)

set(CMAKE_AR                            ${TC_BIN_PATH}${TC_PREFIX}ar${TC_SUFFIX} CACHE INTERNAL "")
set(CMAKE_ASM_COMPILER                  ${TC_BIN_PATH}${TC_PREFIX}gcc${TC_SUFFIX} CACHE INTERNAL "")
set(CMAKE_C_COMPILER                    ${TC_BIN_PATH}${TC_PREFIX}gcc${TC_SUFFIX} CACHE INTERNAL "")
set(CMAKE_CXX_COMPILER                  ${TC_BIN_PATH}${TC_PREFIX}g++${TC_SUFFIX} CACHE INTERNAL "" )
set(CMAKE_LINKER                        ${TC_BIN_PATH}${TC_PREFIX}ld${TC_SUFFIX} CACHE INTERNAL "")
set(CMAKE_OBJCOPY                       ${TC_BIN_PATH}${TC_PREFIX}objcopy${TC_SUFFIX} CACHE INTERNAL "")
set(CMAKE_RANLIB                        ${TC_BIN_PATH}${TC_PREFIX}ranlib${TC_SUFFIX} CACHE INTERNAL "")
set(CMAKE_SIZE                          ${TC_BIN_PATH}${TC_PREFIX}size${TC_SUFFIX} CACHE INTERNAL "")
set(CMAKE_STRIP                         ${TC_BIN_PATH}${TC_PREFIX}size${TC_SUFFIX} CACHE INTERNAL "")

set(CMAKE_C_FLAGS                       "-ffunction-sections -fdata-sections -O0 -g -ggdb")
# set(CMAKE_EXE_LINKER_FLAGS              "-Wl,--gc-sections -O0 -g -ggdb")
set(CMAKE_EXE_LINKER_FLAGS              "-O0 -g -ggdb")


set(CMAKE_C_FLAGS_DEBUG                 "-O0 -g" CACHE INTERNAL "")
set(CMAKE_C_FLAGS_RELEASE               "-O0 -DNDEBUG" CACHE INTERNAL "")
set(CMAKE_CXX_FLAGS_DEBUG               "${CMAKE_C_FLAGS_DEBUG}" CACHE INTERNAL "")
set(CMAKE_CXX_FLAGS_RELEASE             "${CMAKE_C_FLAGS_RELEASE}" CACHE INTERNAL "")

set(CMAKE_C_STANDARD                    90)
set(CMAKE_C_EXTENSIONS                  1)

function(target_newlibc target_name)
    target_compile_options(
        ${target_name}
        PUBLIC
            --include newlib.h
    )

    target_link_libraries(
        ${target_name}
        PUBLIC
            -lc
    )
endfunction()

function(target_newlibc_nano target_name)
    target_link_options(
        ${target_name}
        PUBLIC
            # --specs=nano.specs
    )

    target_compile_options(
        ${target_name}
        PUBLIC
            # --specs=nano.specs
            --include newlib.h
    )

    target_link_libraries(
        ${target_name}
        PUBLIC
            -lc
    )
endfunction()

function(target_enable_semihost target_name)
    target_link_options(
        ${target_name}
        PUBLIC
            --specs=rdimon.specs
            # --specs=rdimon-v2m.specs
    )

    target_compile_options(
        ${target_name}
        PUBLIC
            --specs=rdimon.specs
            # --specs=rdimon-v2m.specs
    )

    target_link_libraries(
        ${target_name}
        PUBLIC
            -lrdimon
    )
endfunction()

function(target_linker_script target_name script_name)
    target_link_options(
        ${target_name}
        INTERFACE
            -T ${CMAKE_CURRENT_SOURCE_DIR}/${script_name}
    )

    set_target_properties(
        ${target_name}
        PROPERTIES INTERFACE_LINK_DEPENDS
            ${CMAKE_CURRENT_SOURCE_DIR}/${script_name}
    )
endfunction()

# ARMv6M Cortex-M0
# Compatible with ARM MPS2 AN382
# Compatible with Qemu microbit machine
add_library( target_mps2_an382_cm0 INTERFACE )

target_compile_options(
    target_mps2_an382_cm0
    INTERFACE
        -mthumb
        -march=armv6-m
        -mcpu=cortex-m0
        -mfloat-abi=soft
        -include soc_mps2_an382_cm0.h
)

target_link_options(
    target_mps2_an382_cm0
    INTERFACE
        -mthumb
        -march=armv6-m
        -mcpu=cortex-m0
        -mfloat-abi=soft
)

target_linker_script(
    target_mps2_an382_cm0
    mps2_armv67m.ld
)

# ARMv6M Cortex-M0-Plus
# Compatible with ARM MPS2 AN383
# No compatible qemu machine
add_library( target_mps2_an383_cm0plus INTERFACE )

target_compile_options(
    target_mps2_an383_cm0plus
    INTERFACE
        -mthumb
        -march=armv6-m
        -mcpu=cortex-m0plus
        -mfloat-abi=soft
        -include soc_mps2_an383_cm0plus.h
)

target_link_options(
    target_mps2_an383_cm0plus
    INTERFACE
        -mthumb
        -march=armv6-m
        -mcpu=cortex-m0plus
        -mfloat-abi=soft
)

target_linker_script(
    target_mps2_an383_cm0plus
    mps2_armv67m.ld
)

# ARMv7M Cortex-M3
# Compatible with ARM MPS2 AN385
add_library( target_mps2_an385_cm3 INTERFACE )

target_compile_options(
    target_mps2_an385_cm3
    INTERFACE
        -mthumb
        -march=armv7-m
        -mcpu=cortex-m3
        -mfloat-abi=soft
        -include soc_mps2_an385_cm3.h
)

target_link_options(
    target_mps2_an385_cm3
    INTERFACE
        -mthumb
        -march=armv7-m
        -mcpu=cortex-m3
        -mfloat-abi=soft
)

target_linker_script(
    target_mps2_an385_cm3
    mps2_armv67m.ld
)

# ARMv7EM Cortex-M4 Soft FP
# Compatible with ARM MPS2 AN386
add_library( target_mps2_an386_cm4 INTERFACE )

target_compile_options(
    target_mps2_an386_cm4
    INTERFACE
        -mthumb
        -march=armv7e-m
        -mcpu=cortex-m4
        -mfloat-abi=soft
        -include soc_mps2_an386_cm4.h
)

target_link_options(
    target_mps2_an386_cm4
    INTERFACE
        -mthumb
        -march=armv7e-m
        -mcpu=cortex-m4
        -mfloat-abi=soft
)

target_linker_script(
    target_mps2_an386_cm4
    # CMSDK_CM4_gcc.ld
    mps2_armv67m.ld
)

# ARMv7EM Cortex-M4 Hard FP
# Compatible with ARM MPS2 AN386
add_library( target_mps2_an386_cm4_hardfp INTERFACE )

target_compile_options(
    target_mps2_an386_cm4_hardfp
    INTERFACE
        -mthumb
        -march=armv7e-m
        -mcpu=cortex-m4
        -mfloat-abi=hard
        -mfpu=fpv4-sp-d16
        -include soc_mps2_an386_cm4.h
)

target_link_options(
    target_mps2_an386_cm4_hardfp
    INTERFACE
        -mthumb
        -march=armv7e-m
        -mcpu=cortex-m4
        -mfloat-abi=hard
        -mfpu=fpv4-sp-d16
)

target_linker_script(
    target_mps2_an386_cm4_hardfp
    # CMSDK_CM4_gcc.ld
    mps2_armv67m.ld
)

# ARMv7EM Cortex-M7 Soft FP
# Compatible with ARM MPS2 AN500
add_library( target_mps2_an500_cm7 INTERFACE )

target_compile_options(
    target_mps2_an500_cm7
    INTERFACE
        -mthumb
        -march=armv7e-m
        -mcpu=cortex-m7
        -mfloat-abi=soft
        -include soc_mps2_an500_cm7.h
)

target_link_options(
    target_mps2_an500_cm7
    INTERFACE
        -mthumb
        -march=armv7e-m
        -mcpu=cortex-m7
        -mfloat-abi=soft
)

target_linker_script(
    target_mps2_an500_cm7
    mps2_armv67m.ld
)

# ARMv7EM Cortex-M7 Hard FP with FPv5 Dual Precision FPU
# Compatible with ARM MPS2 AN500
add_library( target_mps2_an500_cm7_hardfp INTERFACE )

target_compile_options(
    target_mps2_an500_cm7_hardfp
    INTERFACE
        -mthumb
        -march=armv7e-m
        -mcpu=cortex-m7
        -mfloat-abi=hard
        -mfpu=fpv5-d16
        -include soc_mps2_an500_cm7.h
)

target_link_options(
    target_mps2_an500_cm7_hardfp
    INTERFACE
        -mthumb
        -march=armv7e-m
        -mcpu=cortex-m7
        -mfloat-abi=hard
        -mfpu=fpv5-d16
)

target_linker_script(
    target_mps2_an500_cm7_hardfp
    mps2_armv67m.ld
)

# ARMv8M Baseline Cortex-M23
# Compatible with ARM MPS2 AN519
# Note, not available as a qemu target
add_library( target_mps2_an519_cm23 INTERFACE )

target_compile_options(
    target_mps2_an519_cm23
    INTERFACE
        -mthumb
        -march=armv8-m.base
        -mcpu=cortex-m23
        -mfloat-abi=soft
        -include soc_mps2_an519_cm23.h
)

target_link_options(
    target_mps2_an519_cm23
    INTERFACE
        -mthumb
        -march=armv8-m.base
        -mcpu=cortex-m23
        -mfloat-abi=soft
)

target_linker_script(
    target_mps2_an519_cm23
    mps2_armv8m.ld
)

# ARMv8M Mainline Single Cortex-M33, Soft FP
# Compatible with ARM MPS2 AN505
add_library( target_mps2_an505_cm33 INTERFACE )

target_compile_options(
    target_mps2_an505_cm33
    INTERFACE
        -mthumb
        -march=armv8-m.main+dsp
        -mcpu=cortex-m33
        -mfloat-abi=soft
        -include soc_mps2_an505_cm33.h
)

target_link_options(
    target_mps2_an505_cm33
    INTERFACE
        -mthumb
        -march=armv8-m.main+dsp
        -mcpu=cortex-m33
        -mfloat-abi=soft
)

target_linker_script(
    target_mps2_an505_cm33
    # IOTKit_CM33_gcc.ld
    mps2_armv8m.ld
)

# ARMv8M Mainline Single Cortex-M33, FPv5 Single-Precision FPU
# Compatible with ARM MPS2 AN505
add_library( target_mps2_an505_cm33_hardfp INTERFACE )

target_compile_options(
    target_mps2_an505_cm33_hardfp
    INTERFACE
        -mthumb
        -march=armv8-m.main+dsp
        -mcpu=cortex-m33
        -mfloat-abi=hard
        -mfpu=fpv5-sp-d16
        -include soc_mps2_an505_cm33.h
)

target_link_options(
    target_mps2_an505_cm33_hardfp
    INTERFACE
        -mthumb
        -march=armv8-m.main+dsp
        -mcpu=cortex-m33
        -mfloat-abi=hard
        -mfpu=fpv5-sp-d16
)

target_linker_script(
    target_mps2_an505_cm33_hardfp
    # IOTKit_CM33_gcc.ld
    mps2_armv8m.ld
)

# ARMv8.1M Mainline Single Cortex-M55, Soft FP, No MVE
# Compatible with ARM MPS3 AN547
add_library( target_mps3_an547_cm55 INTERFACE )

target_compile_options(
    target_mps3_an547_cm55
    INTERFACE
        -mthumb
        -march=armv8.1-m.main+dsp
        -mcpu=cortex-m55+nomve+nofp
        -mfloat-abi=soft
        -include soc_mps3_an547_cm55.h
)

target_link_options(
    target_mps3_an547_cm55
    INTERFACE
        -mthumb
        -march=armv8.1-m.main+dsp
        -mcpu=cortex-m55+nomve+nofp
        -mfloat-abi=soft
)

target_linker_script(
    target_mps3_an547_cm55
    mps2_armv8m.ld
)

# ARMv8.1M Mainline Single Cortex-M55, Soft FP, Integer MVE
# Compatible with ARM MPS3 AN547
add_library( target_mps3_an547_cm55_mve INTERFACE )

target_compile_options(
    target_mps3_an547_cm55_mve
    INTERFACE
        -mthumb
        -march=armv8.1-m.main+mve
        -mcpu=cortex-m55+nomve.fp
        -mfloat-abi=soft
        -include soc_mps3_an547_cm55.h
)

target_link_options(
    target_mps3_an547_cm55_mve
    INTERFACE
        -mthumb
        -march=armv8.1-m.main+mve
        -mcpu=cortex-m55+nomve.fp
        -mfloat-abi=soft
)

target_linker_script(
    target_mps3_an547_cm55_mve
    mps2_armv8m.ld
)

# ARMv8.1M Mainline Single Cortex-M55, Hard FP, No MVE
# Compatible with ARM MPS3 AN547
add_library( target_mps3_an547_cm55_hardfp INTERFACE )

target_compile_options(
    target_mps3_an547_cm55_hardfp
    INTERFACE
        -mthumb
        -march=armv8.1-m.main+dsp+fp.dp
        -mcpu=cortex-m55+nomve
        -mfloat-abi=hard
        -mfpu=fpv5-d16
        -include soc_mps3_an547_cm55.h
)

target_link_options(
    target_mps3_an547_cm55_hardfp
    INTERFACE
        -mthumb
        -march=armv8.1-m.main+dsp+fp.dp
        -mcpu=cortex-m55+nomve
        -mfloat-abi=hard
        -mfpu=fpv5-d16
)

target_linker_script(
    target_mps3_an547_cm55_hardfp
    mps2_armv8m.ld
)

# ARMv8.1M Mainline Single Cortex-M55, Hard FP, Integer MVE
# Compatible with ARM MPS3 AN547
add_library( target_mps3_an547_cm55_hardfp_mve INTERFACE )

target_compile_options(
    target_mps3_an547_cm55_hardfp_mve
    INTERFACE
        -mthumb
        -march=armv8.1-m.main+mve+fp.dp
        -mcpu=cortex-m55+nomve.fp
        -mfloat-abi=hard
        -mfpu=fpv5-d16
        -include soc_mps3_an547_cm55.h
)

target_link_options(
    target_mps3_an547_cm55_hardfp_mve
    INTERFACE
        -mthumb
        -march=armv8.1-m.main+mve+fp.dp
        -mcpu=cortex-m55+nomve.fp
        -mfloat-abi=hard
        -mfpu=fpv5-d16
)

target_linker_script(
    target_mps3_an547_cm55_hardfp_mve
    mps2_armv8m.ld
)

# ARMv8.1M Mainline Single Cortex-M55, Hard FP, Floating Point MVE
# Compatible with ARM MPS3 AN547
add_library( target_mps3_an547_cm55_hardfp_mvefp INTERFACE )

target_compile_options(
    target_mps3_an547_cm55_hardfp_mvefp
    INTERFACE
        -mthumb
        -march=armv8.1-m.main+mve.fp+fp.dp
        -mcpu=cortex-m55
        -mfloat-abi=hard
        -mfpu=fpv5-d16
        -include soc_mps3_an547_cm55.h
)

target_link_options(
    target_mps3_an547_cm55_hardfp_mvefp
    INTERFACE
        -mthumb
        -march=armv8.1-m.main+mve.fp+fp.dp
        -mcpu=cortex-m55
        -mfloat-abi=hard
        -mfpu=fpv5-d16
)

target_linker_script(
    target_mps3_an547_cm55_hardfp_mvefp
    mps2_armv8m.ld
)
