# FreeRTOS Classic Distribution
# Copyright (C) 2022 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
# SPDX-License-Identifier: MIT

project("FreeRTOS+TCP Echo Client Demo for Posix Simulator")
cmake_minimum_required(VERSION 3.16.3)

if(NOT FREERTOS_PLUS_CONFIGURED)
    set(CMAKE_C_FLAGS "-DprojCOVERAGE_TEST=0")

    set( CONFIG_TARGET_POSIX_SIMULATOR ON )
    set( CONFIG_NET_IP_STACK_BUFFER_ALLOCATION_DYNAMIC ON )
    set( CONFIG_NET_IP_STACK_FREERTOS_PLUS_TCP ON )

    add_subdirectory(../../ lib)
endif()

add_executable( tcp_echo_demo_posix
                SimpleTCPEchoServer.c
                TCPEchoClientTask.c
                tcp_echo_demo_main.c )

target_include_directories( tcp_echo_demo_posix PRIVATE ${CMAKE_CURRENT_LIST_DIR} )

target_sources( tcp_echo_demo_posix
                PRIVATE
                tcp_echo_client_config.h )

target_link_libraries( tcp_echo_demo_posix
                       PRIVATE FreeRTOS::TCP
                       PRIVATE FreeRTOS::Logging
                       PRIVATE FreeRTOS::Platform )
