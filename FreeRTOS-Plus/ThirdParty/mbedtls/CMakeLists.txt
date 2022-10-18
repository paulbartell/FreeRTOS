# FreeRTOS Classic Distribution
# Copyright (C) 2022 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
# SPDX-License-Identifier: MIT

set(MBEDTLS_2_VERSION 2.28.1)
set(MBEDTLS_3_VERSION 3.2.1)

option(CONFIG_NET_TLS_MBEDTLS "Build MbedTLS" OFF)
option(CONFIG_NET_TLS_MBEDTLS_LEGACY "Use Legacy (2.28.1) MbedTLS stack." OFF)

if(CONFIG_NET_TLS_MBEDTLS OR CONFIG_NET_TLS_MBEDTLS_LEGACY )
    include(FetchContent)

    if(CONFIG_NET_TLS_MBEDTLS_LEGACY)
        set(MBEDTLS_VERSION ${MBEDTLS_2_VERSION})
    elseif(CONFIG_NET_TLS_MBEDTLS)
        set(MBEDTLS_VERSION ${MBEDTLS_3_VERSION})
    endif()

    FetchContent_Declare(
        mbedtls
        GIT_REPOSITORY "https://github.com/Mbed-TLS/mbedtls.git"
        GIT_TAG v${MBEDTLS_VERSION}
        SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/mbedtls_${MBEDTLS_VERSION}
        BINARY_DIR ${CMAKE_BINARY_DIR}/mbedtls_${MBEDTLS_VERSION}
    )

    FetchContent_GetProperties(
        mbedtls
        POPULATED mbedtls_POPULATED
    )

    if(NOT ${mbedtls_POPULATED})
        FetchContent_Populate(mbedtls)
    endif()

    if(NOT TARGET MbedTLS_mbedtls)
        set(MBEDTLS_BIN_DIR ${mbedtls_BINARY_DIR})
        set(MBEDTLS_TARGET_PREFIX "MbedTLS_")

        option(USE_STATIC_MBEDTLS_LIBRARY "" ON)
        option(USE_SHARED_MBEDTLS_LIBRARY "" OFF)

        # For compatibility with MbedTLS 3.2.1+
        function(link_to_source base_name)
        endfunction()

        add_library( MbedTLS_platform STATIC )
        target_sources( MbedTLS_platform
                        PRIVATE mbedtls_freertos_port.c )

        target_link_libraries( MbedTLS_platform
                               PRIVATE
                               FreeRTOS::Logging )

        target_include_directories( MbedTLS_platform
                                    PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include )

        target_include_directories( MbedTLS_platform
                                    PUBLIC ${mbedtls_includes}
                                    PUBLIC ${mbedtls_SOURCE_DIR}/library
                                    PUBLIC ${mbedtls_SOURCE_DIR}/include
                                    PUBLIC ${mbedtls_SOURCE_DIR}/include/mbedtls )

        file(GLOB MBEDTLS_PUB_HEADERS ${mbedtls_SOURCE_DIR}/include/mbedtls/*.h )
        file(GLOB MBEDTLS_PUB_HEADERS_PSA ${mbedtls_SOURCE_DIR}/include/psa/*.h )
        file(GLOB MBEDTLS_PRIV_HEADERS ${mbedtls_SOURCE_DIR}/library/*.h )

        target_sources( MbedTLS_platform
                        PUBLIC
                            ${mbedtls_SOURCE_DIR}/include/mbedtls
                            ${mbedtls_SOURCE_DIR}/include/psa
                            ${mbedtls_SOURCE_DIR}/library
                            ${MBEDTLS_PUB_HEADERS}
                            ${MBEDTLS_PUB_HEADERS_PSA}
                            ${MBEDTLS_PRIV_HEADERS} )

        target_compile_definitions( MbedTLS_platform PUBLIC "MBEDTLS_CONFIG_FILE=\"mbedtls_config_v${MBEDTLS_VERSION}.h\"" )

        target_link_libraries( MbedTLS_platform PUBLIC freertos_kernel )

        if(CONFIG_NET_IP_STACK_FREERTOS_PLUS_TCP)
            target_sources( MbedTLS_platform
                            PRIVATE
                            mbedtls_bio_freertos_plus_tcp.c )

            target_link_libraries( MbedTLS_platform PRIVATE FreeRTOS::TCP )
        elseif(CONFG_NET_IP_STACK_CELLULAR)
            target_sources( MbedTLS_platform
                            PRIVATE
                            mbedtls_bio_freertos_cellular.c )

            target_link_libraries( MbedTLS_platform PRIVATE FreeRTOS::Socket )
        endif(CONFIG_NET_IP_STACK_FREERTOS_PLUS_TCP)

        add_subdirectory(${mbedtls_SOURCE_DIR}/library ${mbedtls_BINARY_DIR})

        set_target_properties(
            MbedTLS_mbedcrypto MbedTLS_mbedtls MbedTLS_mbedx509
            PROPERTIES
            ARCHIVE_OUTPUT_DIRECTORY ${MBEDTLS_BIN_DIR}
            LIBRARY_OUTPUT_DIRECTORY ${MBEDTLS_BIN_DIR}
        )

        target_link_libraries(MbedTLS_mbedtls PUBLIC MbedTLS_platform)
        target_link_libraries(MbedTLS_mbedtls PRIVATE MbedTLS_mbedcrypto MbedTLS_mbedx509)

        target_link_libraries(MbedTLS_mbedcrypto PUBLIC MbedTLS_platform)
        target_link_libraries(MbedTLS_mbedx509 PUBLIC MbedTLS_platform)

        add_library(MbedTLS::tls ALIAS MbedTLS_mbedtls)
        add_library(MbedTLS::crypto ALIAS MbedTLS_mbedcrypto)
        add_library(MbedTLS::x509 ALIAS MbedTLS_mbedx509)

    endif()
endif()
