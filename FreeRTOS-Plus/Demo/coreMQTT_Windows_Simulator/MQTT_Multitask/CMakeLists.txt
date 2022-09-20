# FreeRTOS Classic Distribution
# Copyright (C) 2022 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
# SPDX-License-Identifier: MIT

project( "coreMQTT Multi Task Demo" )
cmake_minimum_required( VERSION 3.13 )

if(NOT FREERTOS_PLUS_CONFIGURED)
    set( CONFIG_NET_TLS_MBEDTLS ON )
    set( CONFIG_NET_IP_STACK_FREERTOS_PLUS_TCP ON )

    if(WIN32)
        add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
    endif()

    add_subdirectory(../../../ freertos_plus)
endif()

add_executable( core_mqtt_multi_task_demo
                mqtt-agent-task.c
                simple_sub_pub_demo.c
                ../../Common/coreMQTT_AgentInterface/freertos_agent_message.c
                ../../Common/coreMQTT_AgentInterface/freertos_command_pool.c
                ../../Common/coreMQTT_AgentInterface/subscription_manager.c
                ../coremqtt_demo_main.c )

target_include_directories( core_mqtt_multi_task_demo
                            PUBLIC
                            ${CMAKE_CURRENT_LIST_DIR}
                            ../
                            ../../Common/coreMQTT_AgentInterface/include )

# Add Header files so that they show up properly in Visual Studio
target_sources( core_mqtt_multi_task_demo
                PUBLIC
                demo_config.h )

target_link_libraries( core_mqtt_multi_task_demo
                       PRIVATE FreeRTOS::TCP
                       PRIVATE FreeRTOS::Logging
                       PRIVATE FreeRTOS::coreMQTT
                       PRIVATE FreeRTOS::coreMQTTAgent
                       PRIVATE FreeRTOS::BackoffAlgorithm
                       PRIVATE freertos_kernel
                       PRIVATE FreeRTOS::Transport::Mbedtls )

