# FreeRTOS Classic Distribution
# Copyright (C) 2022 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
# SPDX-License-Identifier: MIT

set( FREERTOS_PORT "GCC_ARM_CM3" CACHE STRING "" )
set( FREERTOS_HEAP "4" CACHE STRING "" )
set( FREERTOS_CONFIG_FILE_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}" CACHE STRING "" )

add_library( freertos_platform STATIC )

target_include_directories( freertos_platform
                            PUBLIC
                            ${CMAKE_CURRENT_LIST_DIR}
                            ../cmsis
                            bsp )

target_compile_definitions( freertos_platform PUBLIC CMSDK_CM3 )

target_sources( freertos_platform
                PRIVATE
                ../freertos_assert_arm.c
                ../freertos_hooks_arm.c
                ../rng_hook_semihost.c
                ../freertos_logging_semihost.c
                ../runtime_stats_cmsdk_timer.c )

target_sources( freertos_platform
                PRIVATE
                bsp/startup_CMSDK_CM3.S
                bsp/startup_CMSDK_CM3.c
                bsp/system_CMSDK_CM3.c )

target_sources( freertos_platform
                PUBLIC
                bsp/CMSDK_CM3.h )

target_sources( freertos_platform
                PUBLIC
                FreeRTOSConfig.h
                ../cmsis/core_cm3.h )

target_link_libraries( freertos_platform PRIVATE freertos_kernel FreeRTOS::Logging )

if( CONFIG_NET_IP_STACK_FREERTOS_PLUS_TCP )
    target_sources( freertos_platform
                    PRIVATE
                    plus_tcp_hooks_mps2_an385.c )

    target_sources( freertos_platform
                    PUBLIC
                    FreeRTOSIPConfig.h )

    target_link_libraries( freertos_platform PRIVATE FreeRTOS::TCP )

endif( CONFIG_NET_IP_STACK_FREERTOS_PLUS_TCP )

# Semihosting support
target_link_libraries( freertos_platform PRIVATE rdimon c )

target_link_libraries( freertos_platform PUBLIC freertos_kernel )

add_library( FreeRTOS::Platform ALIAS freertos_platform )
