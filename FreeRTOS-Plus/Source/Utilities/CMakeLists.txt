# FreeRTOS Classic Distribution
# Copyright (C) 2022 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
# SPDX-License-Identifier: MIT

add_subdirectory(logging)

#
# Backoff Algorithm Library
#
add_library( FreeRTOS_backoff_algorithm STATIC EXCLUDE_FROM_ALL )

target_include_directories( FreeRTOS_backoff_algorithm
                            PUBLIC backoff_algorithm/source/include )

target_sources( FreeRTOS_backoff_algorithm
                PRIVATE backoff_algorithm/source/backoff_algorithm.c )

add_library( FreeRTOS::BackoffAlgorithm ALIAS FreeRTOS_backoff_algorithm )

