# FreeRTOS Classic Distribution
# Copyright (C) 2022 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
# SPDX-License-Identifier: MIT

if(NOT FREERTOS_PLUS_CONFIGURED)
    include( ../../Target/toolchain.cmake )

    project( "CoreSNTP Demo" )
    cmake_minimum_required( VERSION 3.16.3 )

    set( CONFIG_NET_IP_STACK_FREERTOS_PLUS_TCP ON )
    set( CONFIG_NET_TLS_MBEDTLS ON )

    add_subdirectory(../../ lib)
endif()

add_executable( core_sntp_demo
                sntp_demo_main.c
                SNTPClientTask.c
                SampleAppTask.c )

target_include_directories( core_sntp_demo
                            PRIVATE
                            ${CMAKE_CURRENT_LIST_DIR} )

# Add Header files so that they show up properly in Visual Studio
target_sources( core_sntp_demo
                PRIVATE
                common_demo_include.h
                demo_config.h )

target_link_libraries( core_sntp_demo
                       PRIVATE FreeRTOS::TCP
                       PRIVATE FreeRTOS::Platform
                       PRIVATE FreeRTOS::Logging
                       PRIVATE FreeRTOS::coreSNTP
                       PRIVATE FreeRTOS::corePKCS11
                       PRIVATE FreeRTOS::BackoffAlgorithm
                       PRIVATE freertos_kernel
                       PRIVATE MbedTLS::crypto )

