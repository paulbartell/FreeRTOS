# FreeRTOS Classic Distribution
# Copyright (C) 2022 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
# SPDX-License-Identifier: MIT

if(NOT FREERTOS_PLUS_CONFIGURED)
    include( ../../Target/toolchain.cmake )

    project("FreeRTOS+TCP Echo Client / Server Demo")
    cmake_minimum_required(VERSION 3.16.3)

    set( CONFIG_NET_IP_STACK_FREERTOS_PLUS_TCP ON )
    add_subdirectory(../../ lib)
endif()

add_executable( tcp_echo_demo
                TCPEchoClientTask.c
                TCPEchoServerTask.c
                tcp_echo_demo_main.c )

target_include_directories( tcp_echo_demo PRIVATE ${CMAKE_CURRENT_LIST_DIR} )

target_sources( tcp_echo_demo
                PRIVATE
                tcp_echo_config.h )

target_link_libraries( tcp_echo_demo
                       PRIVATE freertos_kernel
                       PRIVATE FreeRTOS::TCP
                       PRIVATE FreeRTOS::Logging
                       PRIVATE FreeRTOS::Platform )
