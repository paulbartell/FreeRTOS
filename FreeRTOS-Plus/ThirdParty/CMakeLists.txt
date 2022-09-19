# FreeRTOS Classic Distribution
# Copyright (C) 2022 Amazon.com, Inc. or its affiliates. All Rights Reserved.
# SPDX-License-Identifier: MIT

add_subdirectory(libpcap)
add_subdirectory(libslirp)

add_subdirectory(mbedtls)
# add_subdirectory(wolfssl)

include(llhttp.cmake)
include(tinycbor.cmake)
