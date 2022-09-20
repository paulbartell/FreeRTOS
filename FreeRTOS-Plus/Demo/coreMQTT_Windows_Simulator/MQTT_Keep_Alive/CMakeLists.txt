# FreeRTOS Classic Distribution
# Copyright (C) 2022 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
# SPDX-License-Identifier: MIT

project( "coreMQTT Keep Alive Demo" )
cmake_minimum_required( VERSION 3.16.3 )

if(NOT FREERTOS_PLUS_CONFIGURED)
    set( CONFIG_NET_IP_STACK_FREERTOS_PLUS_TCP ON )

    if(WIN32)
        add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
    endif()

    add_subdirectory(../../../ lib)
endif()

add_executable( core_mqtt_keepalive_demo
                KeepAliveMQTTExample.c
                ../coremqtt_demo_main.c )

target_include_directories( core_mqtt_keepalive_demo
                            PUBLIC
                            ${CMAKE_CURRENT_LIST_DIR}
                            ../ )

# Add Header files so that they show up properly in Visual Studio
target_sources( core_mqtt_keepalive_demo
                PUBLIC
                demo_config.h )

target_link_libraries( core_mqtt_keepalive_demo
                       PRIVATE FreeRTOS::Platform
                       PRIVATE FreeRTOS::TCP
                       PRIVATE FreeRTOS::Logging
                       PRIVATE FreeRTOS::coreMQTT
                       PRIVATE FreeRTOS::BackoffAlgorithm
                       PRIVATE FreeRTOS::Transport::Plaintext )

