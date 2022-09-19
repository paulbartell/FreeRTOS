# FreeRTOS Classic Distribution
# Copyright (C) 2022 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
# SPDX-License-Identifier: MIT

set( FREERTOS_PORT "MSVC_MINGW" CACHE STRING "" )
set( FREERTOS_HEAP "4" CACHE STRING "" )
set( FREERTOS_CONFIG_FILE_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}" CACHE STRING "" )

add_library( freertos_platform STATIC )

target_include_directories( freertos_platform PUBLIC ${CMAKE_CURRENT_LIST_DIR} )

add_compile_definitions(_CRT_SECURE_NO_WARNINGS)

target_sources( freertos_platform
                PRIVATE
                runtime_stats_winsim.c
                freertos_hooks_winsim.c
                logging_winsim.c )

if(CONFG_NET_IP_STACK_CELLULAR)
    target_sources( freertos_platform
                    PRIVATE
                    comm_if_winsim.c )
endif(CONFG_NET_IP_STACK_CELLULAR)

target_sources( freertos_platform
                PUBLIC
                FreeRTOSConfig.h
                FreeRTOSIPConfig.h )

if( CONFIG_NET_IP_STACK_FREERTOS_PLUS_TCP )
    target_sources( freertos_platform
                    PRIVATE
                    plus_tcp_hooks_winsim.c )

    target_sources( freertos_platform
                    PUBLIC
                    FreeRTOSIPConfig.h )
endif( CONFIG_NET_IP_STACK_FREERTOS_PLUS_TCP )

target_link_libraries( freertos_platform freertos_kernel )
target_link_libraries( freertos_platform FreeRTOS::TCP )
target_link_libraries( freertos_platform FreeRTOS::Logging )

add_library( FreeRTOS::Platform ALIAS freertos_platform )
