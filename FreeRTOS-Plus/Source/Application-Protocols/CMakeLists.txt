# FreeRTOS Classic Distribution
# Copyright (C) 2022 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
# SPDX-License-Identifier: MIT

# Add subdirectories
add_subdirectory( network_transport )

#
# coreHTTP
#
check_init_submodule( coreHTTP )
add_library( coreHTTP STATIC EXCLUDE_FROM_ALL )

target_sources( coreHTTP
                PRIVATE
                coreHTTP/source/core_http_client.c )

target_include_directories( coreHTTP
                            PUBLIC
                            coreHTTP/source/include
                            coreHTTP/source/interface
                            ../config )

target_sources( coreHTTP
                PUBLIC
                coreHTTP/source/include/core_http_client.h
                coreHTTP/source/include/core_http_client_private.h
                coreHTTP/source/include/core_http_config_defaults.h
                coreHTTP/source/interface/transport_interface.h
                ../config/core_http_config.h )

target_link_libraries(coreHTTP PRIVATE llhttp)
target_link_libraries(coreHTTP PRIVATE FreeRTOS::Logging)

add_library( FreeRTOS::coreHTTP ALIAS coreHTTP )


#
# coreMQTT
#
check_init_submodule( coreMQTT )
add_library( coreMQTT STATIC EXCLUDE_FROM_ALL )

target_sources( coreMQTT
                PRIVATE
                    coreMQTT/source/core_mqtt.c
                    coreMQTT/source/core_mqtt_state.c
                    coreMQTT/source/core_mqtt_serializer.c )

target_include_directories( coreMQTT
                            PUBLIC
                                coreMQTT/source/include
                                coreMQTT/source/interface
                                ../config )

target_sources( coreMQTT
                PUBLIC
                    coreMQTT/source/include/core_mqtt.h
                    coreMQTT/source/include/core_mqtt_config_defaults.h
                    ../config/core_mqtt_config.h
                    coreMQTT/source/include/core_mqtt_serializer.h
                    coreMQTT/source/include/core_mqtt_state.h
                    coreMQTT/source/interface/transport_interface.h )

target_link_libraries(coreMQTT PRIVATE FreeRTOS::Logging)

add_library( FreeRTOS::coreMQTT ALIAS coreMQTT )

#
# coreMQTT-Agent
#
check_init_submodule( coreMQTT-Agent )
add_library( coreMQTT_Agent STATIC EXCLUDE_FROM_ALL )

target_sources( coreMQTT_Agent
                PRIVATE
                coreMQTT-Agent/source/core_mqtt_agent.c
                coreMQTT-Agent/source/core_mqtt_agent_command_functions.c )

target_include_directories( coreMQTT_Agent
                            PUBLIC
                            coreMQTT-Agent/source/include )
target_sources( coreMQTT_Agent
                PUBLIC
                coreMQTT-Agent/source/include/core_mqtt_agent.h
                coreMQTT-Agent/source/include/core_mqtt_agent_command_functions.h
                coreMQTT-Agent/source/include/core_mqtt_agent_message_interface.h )

target_link_libraries( coreMQTT_Agent PUBLIC coreMQTT )

target_link_libraries(coreMQTT_Agent PUBLIC FreeRTOS::Logging)

add_library( FreeRTOS::coreMQTTAgent ALIAS coreMQTT_Agent )

#
# coreSNTP
#
check_init_submodule( coreSNTP )
add_library( coreSNTP STATIC EXCLUDE_FROM_ALL )

target_sources( coreSNTP
                PRIVATE
                coreSNTP/source/core_sntp_client.c
                coreSNTP/source/core_sntp_serializer.c )

target_include_directories( coreSNTP
                            PUBLIC
                            coreSNTP/source/include
                            ../config )

target_sources( coreSNTP
                PUBLIC
                    coreSNTP/source/include/core_sntp_client.h
                    coreSNTP/source/include/core_sntp_config_defaults.h
                    coreSNTP/source/include/core_sntp_serializer.h
                    ../config/core_sntp_config.h )

target_link_libraries(coreSNTP PRIVATE FreeRTOS::Logging)

add_library( FreeRTOS::coreSNTP ALIAS coreSNTP )
