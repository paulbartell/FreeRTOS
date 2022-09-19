# FreeRTOS Classic Distribution
# Copyright (C) 2022 Amazon.com, Inc. or its affiliates. All Rights Reserved.
# SPDX-License-Identifier: MIT

#
# AWS IoT Device Defender
#
check_init_submodule( device-defender )
add_library( AWS_IoT_DeviceDefender STATIC EXCLUDE_FROM_ALL )

target_sources( AWS_IoT_DeviceDefender
                PRIVATE
                device-defender/source/defender.c )

target_include_directories( AWS_IoT_DeviceDefender
                            PUBLIC
                            device-defender/source/include )

target_sources( AWS_IoT_DeviceDefender
                PUBLIC
                device-defender/source/include/defender.h
                device-defender/source/include/defender_config_defaults.h )

add_library( AWS::IoT::DeviceDefender ALIAS AWS_IoT_DeviceDefender )

#
# AWS IoT Device Shadow
#
check_init_submodule( device-shadow )
add_library( AWS_IoT_DeviceShadow STATIC EXCLUDE_FROM_ALL )

target_sources( AWS_IoT_DeviceShadow
                PRIVATE
                device-shadow/source/shadow.c )

target_include_directories( AWS_IoT_DeviceShadow
                            PUBLIC
                            device-shadow/source/include )

target_sources( AWS_IoT_DeviceShadow
                PUBLIC
                device-shadow/source/include/shadow.h
                device-shadow/source/include/shadow_config_defaults.h )

add_library( AWS::IoT::DeviceShadow ALIAS AWS_IoT_DeviceShadow )

#
# AWS Fleet Provisioning
#
check_init_submodule( fleet-provisioning )
add_library( AWS_IoT_FleetProvisioning STATIC EXCLUDE_FROM_ALL )

target_sources( AWS_IoT_FleetProvisioning
                PRIVATE
                fleet-provisioning/source/fleet_provisioning.c )

target_include_directories( AWS_IoT_FleetProvisioning
                            PUBLIC
                            fleet-provisioning/source/include )

target_sources( AWS_IoT_FleetProvisioning
                PUBLIC
                fleet-provisioning/source/include/fleet_provisioning.h
                fleet-provisioning/source/include/fleet_provisioning_config_defaults.h )

add_library( AWS::IoT::FleetProvisioning ALIAS AWS_IoT_FleetProvisioning )

#
# AWS IoT Jobs
#
check_init_submodule( jobs )
add_library( AWS_IoT_Jobs STATIC EXCLUDE_FROM_ALL )

target_sources( AWS_IoT_Jobs
                PRIVATE
                jobs/source/jobs.c )

target_include_directories( AWS_IoT_Jobs
                            PUBLIC
                            jobs/source/include )

target_sources( AWS_IoT_Jobs
                PUBLIC
                jobs/source/include/jobs.h )

add_library( AWS::IoT::Jobs ALIAS AWS_IoT_Jobs )

#
# AWS IoT Over The Air updates
#
check_init_submodule( ota )
add_library( AWS_IoT_Ota STATIC EXCLUDE_FROM_ALL )

target_sources( AWS_IoT_Ota
                PRIVATE
                ota/source/ota.c
                ota/source/ota_base64.c
                ota/source/ota_cbor.c
                ota/source/ota_http.c
                ota/source/ota_interface.c
                ota/source/ota_mqtt.c )

target_include_directories( AWS_IoT_Ota
                            PUBLIC
                            ota/source/include )

target_sources( AWS_IoT_Ota
                PRIVATE
                ota/source/include/ota_base64_private.h
                ota/source/include/ota_cbor_private.h
                ota/source/include/ota_http_private.h
                ota/source/include/ota_interface_private.h
                ota/source/include/ota_mqtt_private.h
                ota/source/include/ota_private.h )

target_sources( AWS_IoT_Ota
                PUBLIC
                ota/source/include/ota.h
                ota/source/include/ota_appversion32.h
                ota/source/include/ota_config_defaults.h
                ota/source/include/ota_http_interface.h
                ota/source/include/ota_mqtt_interface.h
                ota/source/include/ota_os_interface.h
                ota/source/include/ota_platform_interface.h )

target_link_libraries( AWS_IoT_Ota INTERFACE
                       FreeRTOS::coreJSON
                       TinyCBOR )

add_library( AWS::IoT::Ota ALIAS AWS_IoT_Ota )

#
# AWS Signature v4
#
check_init_submodule( sigv4 )
add_library( AWS_Sigv4 STATIC EXCLUDE_FROM_ALL )

target_sources( AWS_Sigv4
                PRIVATE
                sigv4/source/sigv4.c
                sigv4/source/sigv4_quicksort.c )

target_include_directories( AWS_Sigv4
                            PUBLIC
                            sigv4/source/include )

target_sources( AWS_Sigv4
                PRIVATE
                sigv4/source/include/sigv4_internal.h )

target_sources( AWS_Sigv4
                PUBLIC
                sigv4/source/include/sigv4.h
                sigv4/source/include/sigv4_config_defaults.h
                sigv4/source/include/sigv4_quicksort.h )

add_library( AWS::Sigv4 ALIAS AWS_Sigv4 )
