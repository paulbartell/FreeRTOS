# FreeRTOS Classic Distribution
# Copyright (C) 2022 Amazon.com, Inc. or its affiliates. All Rights Reserved.
# SPDX-License-Identifier: MIT

if(${WIN32})
    add_library( WinPcap INTERFACE )

    if( NOT ${MSVC})
        message(FATAL_ERROR "Only MSVC is supported with the wpcap library.")
    endif()

    if(${CMAKE_SIZEOF_VOID_P} EQUAL 8)
        message(NOTICE "Using x64 (64 bit) wpcap library")
        target_link_libraries( WinPcap INTERFACE ${CMAKE_CURRENT_LIST_DIR}/lib/x64/wpcap.lib )
    elseif(${CMAKE_SIZEOF_VOID_P} EQUAL 4)
        message(NOTICE "Using x86 (32 bit) wpcap library")
        target_link_libraries( WinPcap INTERFACE ${CMAKE_CURRENT_LIST_DIR}/lib/wpcap.lib )
    else()
        message(FATAL_ERROR "Failed to determine MSVC platform achirecture (x86 / x64).")
    endif()

    target_include_directories( WinPcap INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include )

    target_sources( WinPcap
                    INTERFACE FILE_SET HEADERS FILES
                    WinPcap/include/Packet32.h
                    WinPcap/include/Win32-Extensions.h
                    WinPcap/include/bittypes.h
                    WinPcap/include/ip6_misc.h
                    WinPcap/include/pcap-bpf.h
                    WinPcap/include/pcap-namedb.h
                    WinPcap/include/pcap-stdinc.h
                    WinPcap/include/pcap.h
                    WinPcap/include/remote-ext.h )
endif()

