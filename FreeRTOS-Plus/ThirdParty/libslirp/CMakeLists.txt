# FreeRTOS Classic Distribution
# Copyright (C) 2022 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
# SPDX-License-Identifier: MIT

if(CONFIG_NET_IP_STACK_NETIF_LIBSLIRP)
    include(ExternalProject)

    find_package(PkgConfig)

    if(PKG_CONFIG_FOUND)
        pkg_search_module(GLIB glib-2.0)
        pkg_search_module(SLIRP slirp)
    endif()

    if(SLIRP_FOUND)
        add_library( slirp INTERFACE )
        target_include_directories(slirp INTERFACE ${SLIRP_INCLUDE_DIRS})
        target_link_directories(slirp INTERFACE ${SLIRP_LIBRARY_DIRS})
        target_link_libraries(slirp INTERFACE ${SLIRP_LINK_LIBRARIES})
    else()
        include(FetchContent)

        set(LIBSLIRP_VERSION 4.7.0-next)

        set(LIBSLIRP_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/libslirp_${LIBSLIRP_VERSION})
        set(LIBSLIRP_BINARY_DIR ${CMAKE_BINARY_DIR}/libslirp_${LIBSLIRP_VERSION})

        FetchContent_Declare(
            Slirp
            GIT_REPOSITORY https://gitlab.freedesktop.org/slirp/libslirp.git
            GIT_TAG 38638382c8f3dc2523468f0c54014f4fdf0aa38f
            SOURCE_DIR ${LIBSLIRP_SOURCE_DIR}
            BINARY_DIR ${LIBSLIRP_BINARY_DIR}
        )

        FetchContent_GetProperties(
            Slirp
            POPULATED Slirp_POPULATED
        )

        if(NOT ${Slirp_POPULATED})
            FetchContent_Populate(Slirp)
        endif()

        add_library( slirp SHARED EXCLUDE_FROM_ALL )

        set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

        string(REPLACE "." ";" VERSION_LIST ${LIBSLIRP_VERSION})

        list(POP_FRONT VERSION_LIST SLIRP_MAJOR_VERSION)
        list(POP_FRONT VERSION_LIST SLIRP_MINOR_VERSION)
        list(POP_FRONT VERSION_LIST SLIRP_MICRO_VERSION)

        set(SLIRP_VERSION_STRING "\"${LIBSLIRP_VERSION}\"")

        configure_file(${LIBSLIRP_SOURCE_DIR}/src/libslirp-version.h.in
                       ${LIBSLIRP_SOURCE_DIR}/src/libslirp-version.h)

        target_sources( slirp
                        PRIVATE
                        ${LIBSLIRP_SOURCE_DIR}/src/arp_table.c
                        ${LIBSLIRP_SOURCE_DIR}/src/bootp.c
                        ${LIBSLIRP_SOURCE_DIR}/src/cksum.c
                        ${LIBSLIRP_SOURCE_DIR}/src/dhcpv6.c
                        ${LIBSLIRP_SOURCE_DIR}/src/dnssearch.c
                        ${LIBSLIRP_SOURCE_DIR}/src/if.c
                        ${LIBSLIRP_SOURCE_DIR}/src/ip6_icmp.c
                        ${LIBSLIRP_SOURCE_DIR}/src/ip6_input.c
                        ${LIBSLIRP_SOURCE_DIR}/src/ip6_output.c
                        ${LIBSLIRP_SOURCE_DIR}/src/ip_icmp.c
                        ${LIBSLIRP_SOURCE_DIR}/src/ip_input.c
                        ${LIBSLIRP_SOURCE_DIR}/src/ip_output.c
                        ${LIBSLIRP_SOURCE_DIR}/src/mbuf.c
                        ${LIBSLIRP_SOURCE_DIR}/src/misc.c
                        ${LIBSLIRP_SOURCE_DIR}/src/ncsi.c
                        ${LIBSLIRP_SOURCE_DIR}/src/ndp_table.c
                        ${LIBSLIRP_SOURCE_DIR}/src/sbuf.c
                        ${LIBSLIRP_SOURCE_DIR}/src/slirp.c
                        ${LIBSLIRP_SOURCE_DIR}/src/socket.c
                        ${LIBSLIRP_SOURCE_DIR}/src/state.c
                        ${LIBSLIRP_SOURCE_DIR}/src/stream.c
                        ${LIBSLIRP_SOURCE_DIR}/src/tcp_input.c
                        ${LIBSLIRP_SOURCE_DIR}/src/tcp_output.c
                        ${LIBSLIRP_SOURCE_DIR}/src/tcp_subr.c
                        ${LIBSLIRP_SOURCE_DIR}/src/tcp_timer.c
                        ${LIBSLIRP_SOURCE_DIR}/src/tftp.c
                        ${LIBSLIRP_SOURCE_DIR}/src/udp.c
                        ${LIBSLIRP_SOURCE_DIR}/src/udp6.c
                        ${LIBSLIRP_SOURCE_DIR}/src/util.c
                        ${LIBSLIRP_SOURCE_DIR}/src/version.c
                        ${LIBSLIRP_SOURCE_DIR}/src/vmstate.c )

        target_sources( slirp
                        PRIVATE
                            ${LIBSLIRP_SOURCE_DIR}/src/bootp.h
                            ${LIBSLIRP_SOURCE_DIR}/src/debug.h
                            ${LIBSLIRP_SOURCE_DIR}/src/dhcpv6.h
                            ${LIBSLIRP_SOURCE_DIR}/src/if.h
                            ${LIBSLIRP_SOURCE_DIR}/src/ip.h
                            ${LIBSLIRP_SOURCE_DIR}/src/ip6.h
                            ${LIBSLIRP_SOURCE_DIR}/src/ip6_icmp.h
                            ${LIBSLIRP_SOURCE_DIR}/src/ip_icmp.h
                            ${LIBSLIRP_SOURCE_DIR}/src/main.h
                            ${LIBSLIRP_SOURCE_DIR}/src/mbuf.h
                            ${LIBSLIRP_SOURCE_DIR}/src/misc.h
                            ${LIBSLIRP_SOURCE_DIR}/src/ncsi-pkt.h
                            ${LIBSLIRP_SOURCE_DIR}/src/sbuf.h
                            ${LIBSLIRP_SOURCE_DIR}/src/slirp.h
                            ${LIBSLIRP_SOURCE_DIR}/src/socket.h
                            ${LIBSLIRP_SOURCE_DIR}/src/stream.h
                            ${LIBSLIRP_SOURCE_DIR}/src/tcp.h
                            ${LIBSLIRP_SOURCE_DIR}/src/tcp_timer.h
                            ${LIBSLIRP_SOURCE_DIR}/src/tcp_var.h
                            ${LIBSLIRP_SOURCE_DIR}/src/tcpip.h
                            ${LIBSLIRP_SOURCE_DIR}/src/tftp.h
                            ${LIBSLIRP_SOURCE_DIR}/src/udp.h
                            ${LIBSLIRP_SOURCE_DIR}/src/util.h
                            ${LIBSLIRP_SOURCE_DIR}/src/vmstate.h
                        PUBLIC
                            ${LIBSLIRP_SOURCE_DIR}/src/libslirp.h)

        target_include_directories( slirp
                                    PUBLIC ${LIBSLIRP_SOURCE_DIR}/src )

        if(NOT WIN32)
            find_library(LIBRESOLV resolv)
            set(LIBRESOLV "-lresolv")
        endif()

        if(NOT GLIB_FOUND)
            set(GLIB_VERSION_MAJOR "2")
            set(GLIB_VERSION_MINOR "73")
            set(GLIB_VERSION_PATCH "3")
            set(GLIB_VERSION_STR_MM ${GLIB_VERSION_MAJOR}.${GLIB_VERSION_MINOR})
            set(GLIB_VERSION_STR_MMP ${GLIB_VERSION_STR_MM}.${GLIB_VERSION_PATCH})
            set(GLIB_ARCHIVE_URL https://download.gnome.org/sources/glib/${GLIB_VERSION_STR_MM}/glib-${GLIB_VERSION_STR_MMP}.tar.xz)

            set(GLIB_INSTALL_DIR ${CMAKE_CURRENT_LIST_DIR}/glib-${GLIB_VERSION_STR_MMP}-install)

            if(NOT IS_DIRECTORY ${GLIB_INSTALL_DIR})
                find_program(MESON_BIN NAMES "meson" DOC "Path to meson executable" REQUIRED)
                find_program(NINJA_BIN NAMES "ninja" DOC "Path to ninja build executable" REQUIRED)

                ExternalProject_Add(
                    GLIB
                    URL ${GLIB_ARCHIVE_URL}
                    URL_HASH SHA256=df1a2b841667d6b48b2ef6969ebda4328243829f6e45866726f806f90f64eead
                    PREFIX glib
                    INSTALL_DIR ${GLIB_INSTALL_DIR}
                    CONFIGURE_COMMAND
                        ${MESON_BIN}
                        -Dinstalled_tests=false
                        -Dtests=false
                        -Dxattr=false
                        -Dlibelf=disabled
                        -Dselinux=disabled
                        -Dlibmount=disabled
                        -Diconv=external
                        --prefix=<INSTALL_DIR> <BINARY_DIR> <SOURCE_DIR>
                    BUILD_COMMAND
                        ${NINJA_BIN} -C <BINARY_DIR>
                    INSTALL_COMMAND
                        ${NINJA_BIN} -C <BINARY_DIR> install
                )

                add_dependencies( slirp GLIB )
            endif()

            set(GLIB_INCLUDE_DIRS ${GLIB_INSTALL_DIR}/include
                                  ${GLIB_INSTALL_DIR}/include/glib-2.0
                                  ${GLIB_INSTALL_DIR}/lib/glib-2.0/include )

            set(GLIB_LIBRARY_DIRS ${GLIB_INSTALL_DIR}/lib)
            set(GLIB_LIBRARIES "glib-2.0")
        endif()

        target_include_directories(slirp PRIVATE ${GLIB_INCLUDE_DIRS})
        target_link_directories(slirp PRIVATE ${GLIB_LIBRARY_DIRS})
        target_link_libraries(slirp PRIVATE ${GLIB_LIBRARIES})

        if(MSVC)
            target_compile_definitions( slirp PRIVATE _WINSOCK_DEPRECATED_NO_WARNINGS)
            target_compile_options(slirp PRIVATE
                                    "/wd4273"
                                    "/wd4013"
                                    "/wd4018"
                                    "/wd4267"
                                    "/wd4244")
        endif()
    endif()

    if(WIN32)
        target_link_libraries( slirp PUBLIC Iphlpapi Ws2_32)
    else()
        target_link_libraries( slirp PRIVATE ${LIBRESOLV} )
    endif()
endif()
