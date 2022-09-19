# FreeRTOS Classic Distribution
# Copyright (C) 2022 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
# SPDX-License-Identifier: MIT

set( FREERTOS_PORT "GCC_POSIX" CACHE STRING "" )
set( FREERTOS_HEAP "4" CACHE STRING "" )
set( FREERTOS_CONFIG_FILE_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}" CACHE STRING "" )

add_library( freertos_platform STATIC )

target_include_directories( freertos_platform PUBLIC ${CMAKE_CURRENT_LIST_DIR} )

target_sources( freertos_platform
                PRIVATE
                freertos_hooks_posix.c
                freertos_assert_posix.c
                freertos_logging_posix.c
                runtime_stats_posix.c )

target_sources( freertos_platform
                PUBLIC
                FreeRTOSConfig.h )

target_link_libraries( freertos_platform freertos_kernel  FreeRTOS::Logging )

if( CONFIG_NET_IP_STACK_FREERTOS_PLUS_TCP )
    target_sources( freertos_platform
                    PRIVATE
                    plus_tcp_hooks_posix.c )

    target_sources( freertos_platform
                    PUBLIC
                    FreeRTOSIPConfig.h )

    target_link_libraries( freertos_platform FreeRTOS::TCP )

endif( CONFIG_NET_IP_STACK_FREERTOS_PLUS_TCP )

add_library( FreeRTOS::Platform ALIAS freertos_platform )
