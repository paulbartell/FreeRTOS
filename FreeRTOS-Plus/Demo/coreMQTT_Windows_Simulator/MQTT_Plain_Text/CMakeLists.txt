# FreeRTOS Classic Distribution
# Copyright (C) 2022 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
# SPDX-License-Identifier: MIT

if(NOT FREERTOS_PLUS_CONFIGURED)
    include( ../../Target/toolchain.cmake )

    project( "coreMQTT Plain Text Demo" )
    cmake_minimum_required( VERSION 3.16.3 )

    set( CONFIG_NET_TLS_MBEDTLS ON )
    set( CONFIG_NET_IP_STACK_FREERTOS_PLUS_TCP ON )

    add_subdirectory(../../../ lib)
endif()

add_executable( core_mqtt_plaintext_demo
                PlaintextMQTTExample.c
                ../coremqtt_demo_main.c )

target_include_directories( core_mqtt_plaintext_demo
                            PUBLIC
                            ${CMAKE_CURRENT_LIST_DIR}
                            ../ )

# Add Header files so that they show up properly in Visual Studio
target_sources( core_mqtt_plaintext_demo
                PUBLIC
                demo_config.h )

target_link_libraries( core_mqtt_plaintext_demo
                       PRIVATE FreeRTOS::Platform
                       PRIVATE FreeRTOS::TCP
                       PRIVATE FreeRTOS::Logging
                       PRIVATE FreeRTOS::coreMQTT
                       PRIVATE FreeRTOS::BackoffAlgorithm
                       PRIVATE FreeRTOS::Transport::Plaintext )

