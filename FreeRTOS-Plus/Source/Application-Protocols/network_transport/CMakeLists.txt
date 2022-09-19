# FreeRTOS Classic Distribution
# Copyright (C) 2022 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
# SPDX-License-Identifier: MIT

if(CONFIG_NET_IP_STACK_FREERTOS_PLUS_TCP)
    add_library( FreeRTOS_Socket INTERFACE )

    target_sources( FreeRTOS_Socket
                    INTERFACE
                    sockets_wrapper/freertos_plus_tcp/sockets_wrapper.c )

    target_include_directories( FreeRTOS_Socket
                                INTERFACE
                                sockets_wrapper/freertos_plus_tcp )

    target_sources( FreeRTOS_Socket
                    INTERFACE
                    sockets_wrapper/freertos_plus_tcp/sockets_wrapper.h )

    target_link_libraries( FreeRTOS_Socket INTERFACE FreeRTOS::TCP )

    add_library( FreeRTOS::Socket ALIAS FreeRTOS_Socket )
endif(CONFIG_NET_IP_STACK_FREERTOS_PLUS_TCP)

if(CONFIG_NET_IP_STACK_CELLULAR)
    add_library( FreeRTOS_Socket INTERFACE )

    target_sources( FreeRTOS_Socket
                    INTERFACE
                    sockets_wrapper/cellular/sockets_wrapper.c )

    target_include_directories( FreeRTOS_Socket
                                INTERFACE
                                sockets_wrapper/cellular )

    target_sources( FreeRTOS_Socket
                    INTERFACE
                    sockets_wrapper/cellular/sockets_wrapper.h )

    target_link_libraries( FreeRTOS_Socket INTERFACE FreeRTOS::Cellular )

    add_library( FreeRTOS::Socket ALIAS FreeRTOS_Socket )
endif(CONFIG_NET_IP_STACK_CELLULAR)

if( CONFIG_NET_TLS_MBEDTLS )
    add_library( FreeRTOS_Transport_mbedtls INTERFACE )

    target_sources( FreeRTOS_Transport_mbedtls
                    INTERFACE
                    using_mbedtls/using_mbedtls.c )

    target_include_directories( FreeRTOS_Transport_mbedtls
                                INTERFACE
                                using_mbedtls )

    target_sources( FreeRTOS_Transport_mbedtls
                    INTERFACE
                    using_mbedtls/using_mbedtls.h )

    target_link_libraries( FreeRTOS_Transport_mbedtls INTERFACE FreeRTOS_Socket )
    target_link_libraries( FreeRTOS_Transport_mbedtls INTERFACE MbedTLS::tls  MbedTLS_platform)

    add_library( FreeRTOS::Transport::Mbedtls ALIAS FreeRTOS_Transport_mbedtls )


    add_library( FreeRTOS_Transport_mbedtls_pkcs11 INTERFACE )

    target_sources( FreeRTOS_Transport_mbedtls_pkcs11
                    INTERFACE
                    using_mbedtls_pkcs11/using_mbedtls_pkcs11.c )

    target_include_directories( FreeRTOS_Transport_mbedtls_pkcs11
                                INTERFACE
                                using_mbedtls_pkcs11 )

    target_sources( FreeRTOS_Transport_mbedtls_pkcs11
                    INTERFACE
                    using_mbedtls_pkcs11/using_mbedtls_pkcs11.h )

    target_link_libraries( FreeRTOS_Transport_mbedtls_pkcs11 INTERFACE FreeRTOS_Socket )

    target_link_libraries( FreeRTOS_Transport_mbedtls_pkcs11 INTERFACE MbedTLS::tls )

    add_library( FreeRTOS::Transport::MbedtlsPkcs11 ALIAS FreeRTOS_Transport_mbedtls_pkcs11 )
endif( CONFIG_NET_TLS_MBEDTLS )

if( CONFIG_NET_TLS_WOLFSSL )
    add_library( FreeRTOS_Transport_wolfssl INTERFACE )

    target_sources( FreeRTOS_Transport_wolfssl
                    INTERFACE
                    using_wolfSSL/using_wolfSSL.c )

    target_include_directories( FreeRTOS_Transport_wolfssl
                                INTERFACE
                                using_wolfSSL )

    target_sources( FreeRTOS_Transport_wolfssl
                    INTERFACE
                    using_wolfSSL/using_wolfSSL.h )

    target_link_libraries( FreeRTOS_Transport_wolfssl INTERFACE wolfssl)
    target_link_libraries( FreeRTOS_Transport_wolfssl INTERFACE FreeRTOS_Socket)

    add_library( FreeRTOS::Transport::wolfSSL ALIAS FreeRTOS_Transport_wolfssl )
endif( CONFIG_NET_TLS_WOLFSSL )

add_library( FreeRTOS_Transport_plaintext INTERFACE )

target_sources( FreeRTOS_Transport_plaintext
                INTERFACE
                using_plaintext/using_plaintext.c )

target_include_directories( FreeRTOS_Transport_plaintext
                            INTERFACE
                            using_plaintext )

target_sources( FreeRTOS_Transport_plaintext
                INTERFACE
                using_plaintext/using_plaintext.h )

target_link_libraries( FreeRTOS_Transport_plaintext INTERFACE FreeRTOS_Socket )

add_library( FreeRTOS::Transport::Plaintext ALIAS FreeRTOS_Transport_plaintext )

