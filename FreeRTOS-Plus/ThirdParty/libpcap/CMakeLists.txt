# FreeRTOS Classic Distribution
# Copyright (C) 2022 Amazon.com, Inc. or its affiliates. All Rights Reserved.
# SPDX-License-Identifier: MIT

# Link against winpcap for windows builds
if(WIN32 AND MSVC AND CONFIG_NET_IP_STACK_NETIF_LIBPCAP)
    add_library( WinPcap INTERFACE )

    if(${CMAKE_SIZEOF_VOID_P} EQUAL 8)
        message(NOTICE "Using x64 (64 bit) wpcap library")
        target_link_libraries( WinPcap INTERFACE ${CMAKE_CURRENT_LIST_DIR}/winpcap/lib/x64/wpcap.lib )
    elseif(${CMAKE_SIZEOF_VOID_P} EQUAL 4)
        message(NOTICE "Using x86 (32 bit) wpcap library")
        target_link_libraries( WinPcap INTERFACE ${CMAKE_CURRENT_LIST_DIR}/winpcap/lib/wpcap.lib )
    else()
        message(FATAL_ERROR "Failed to determine MSVC platform achirecture (x86 / x64).")
    endif()

    target_include_directories( WinPcap INTERFACE ${CMAKE_CURRENT_LIST_DIR}/winpcap/include )

    target_sources( WinPcap
                    INTERFACE
                    winpcap/include/Packet32.h
                    winpcap/include/Win32-Extensions.h
                    winpcap/include/bittypes.h
                    winpcap/include/ip6_misc.h
                    winpcap/include/pcap-bpf.h
                    winpcap/include/pcap-namedb.h
                    winpcap/include/pcap-stdinc.h
                    winpcap/include/pcap.h
                    winpcap/include/remote-ext.h )

# Otherwise, link against libpcap directly
elseif( CONFIG_NET_IP_STACK_NETIF_LIBPCAP )


endif()

