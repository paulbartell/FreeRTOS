# FreeRTOS Classic Distribution
# Copyright (C) 2022 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
# SPDX-License-Identifier: MIT

if(NOT FREERTOS_PLUS_CONFIGURED)
    include( ../../Target/toolchain.cmake )

    project( "coreMQTT Demos" )
    cmake_minimum_required( VERSION 3.16.3 )

    set( CONFIG_NET_IP_STACK_FREERTOS_PLUS_TCP ON )
    set( CONFIG_NET_TLS_MBEDTLS ON )
    set( CONFIG_NET_TLS_WOLFSSL ON )

    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

    add_subdirectory(../../ lib)
endif()

add_subdirectory( MQTT_Basic_TLS )
add_subdirectory( MQTT_Keep_Alive )
add_subdirectory( MQTT_Multitask )
add_subdirectory( MQTT_Mutual_Auth )
# add_subdirectory( MQTT_Mutual_Auth_wolfSSL )
add_subdirectory( MQTT_Plain_Text )
add_subdirectory( MQTT_Serializer )
