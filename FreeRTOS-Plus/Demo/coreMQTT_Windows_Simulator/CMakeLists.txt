# FreeRTOS Classic Distribution
# Copyright (C) 2022 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
# SPDX-License-Identifier: MIT

project( "coreMQTT Windows Simulator Demos" )
cmake_minimum_required( VERSION 3.16.3 )

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

if(NOT FREERTOS_PLUS_CONFIGURED)
    set( CONFIG_NET_TLS_MBEDTLS ON )
    set( CONFIG_NET_TLS_WOLFSSL ON )
    set( CONFIG_NET_IP_STACK_FREERTOS_PLUS_TCP ON )

    if(WIN32)
        add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
    endif()

    add_subdirectory(../../ lib)

endif()

add_subdirectory( MQTT_Basic_TLS )
add_subdirectory( MQTT_Keep_Alive )
add_subdirectory( MQTT_Multitask )
add_subdirectory( MQTT_Mutual_Auth )
# add_subdirectory( MQTT_Mutual_Auth_wolfSSL )
add_subdirectory( MQTT_Plain_Text )
add_subdirectory( MQTT_Serializer )
