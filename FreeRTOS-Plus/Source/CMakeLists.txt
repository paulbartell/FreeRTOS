# FreeRTOS Classic Distribution
# Copyright (C) 2022 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
# SPDX-License-Identifier: MIT

#
# FreeRTOS+TCP
#
check_init_submodule( FreeRTOS-Plus-TCP )
include( FreeRTOS_Plus_TCP.cmake )

#
# coreJSON
#
check_init_submodule( coreJSON )
add_library( FreeRTOS_coreJSON STATIC EXCLUDE_FROM_ALL )

target_sources( FreeRTOS_coreJSON
                PRIVATE
                coreJSON/source/core_json.c )

target_include_directories( FreeRTOS_coreJSON
                            PUBLIC
                            coreJSON/source/include )

target_sources( FreeRTOS_coreJSON
                PUBLIC
                coreJSON/source/include/core_json.h )

add_library( FreeRTOS::coreJSON ALIAS FreeRTOS_coreJSON )

#
# corePKCS11
#

check_init_submodule( corePKCS11 )
add_library( FreeRTOS_corePKCS11 STATIC EXCLUDE_FROM_ALL )

target_sources( FreeRTOS_corePKCS11
                PRIVATE
                corePKCS11/source/core_pkcs11.c
                corePKCS11/source/core_pki_utils.c
                corePKCS11/source/portable/os/core_pkcs11_pal_utils.c )

target_include_directories( FreeRTOS_corePKCS11
                            PUBLIC
                            corePKCS11/source/include
                            config )

target_include_directories( FreeRTOS_corePKCS11
                            PRIVATE
                            corePKCS11/source/portable/os )

target_sources( FreeRTOS_corePKCS11
                PUBLIC
                corePKCS11/source/include/core_pkcs11.h
                corePKCS11/source/include/core_pkcs11_config_defaults.h
                corePKCS11/source/include/core_pkcs11_pal.h
                corePKCS11/source/include/core_pki_utils.h
                corePKCS11/source/portable/os/core_pkcs11_pal_utils.h
                config/core_pkcs11_config.h )

target_link_libraries( FreeRTOS_corePKCS11
                        PRIVATE FreeRTOS::Logging
                        PUBLIC OASIS::PKCS11 )

if(CONFIG_NET_TLS_MBEDTLS)
    target_sources( FreeRTOS_corePKCS11
                    PRIVATE corePKCS11/source/portable/mbedtls/core_pkcs11_mbedtls.c )

    target_link_libraries( FreeRTOS_corePKCS11
                            PRIVATE MbedTLS::crypto
                            PRIVATE MbedTLS::x509 )
endif()

if(CONFIG_TARGET_WINDOWS_SIMULATOR)
    target_sources( FreeRTOS_corePKCS11
                    PRIVATE
                    corePKCS11/source/portable/os/freertos_winsim/core_pkcs11_pal.c )
    target_link_libraries( FreeRTOS_corePKCS11 PRIVATE freertos_kernel )
elseif(CONFIG_TARGET_POSIX_SIMULATOR)
    target_sources( FreeRTOS_corePKCS11
                    PRIVATE
                    corePKCS11/source/portable/os/posix/core_pkcs11_pal.c )
    target_link_libraries( FreeRTOS_corePKCS11 PRIVATE freertos_kernel )
endif()
add_library( FreeRTOS::corePKCS11 ALIAS FreeRTOS_corePKCS11 )

# Handle subdirectories
add_subdirectory( FreeRTOS-Plus-CLI )
add_subdirectory( Utilities )
add_subdirectory( Application-Protocols )
add_subdirectory( AWS )
