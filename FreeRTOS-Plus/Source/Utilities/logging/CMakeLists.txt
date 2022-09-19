# FreeRTOS Classic Distribution
# Copyright (C) 2022 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
# SPDX-License-Identifier: MIT

add_library( FreeRTOS_logging INTERFACE )

target_include_directories( FreeRTOS_logging INTERFACE ${CMAKE_CURRENT_LIST_DIR} )
target_sources( FreeRTOS_logging
                INTERFACE
                logging.h
                logging_levels.h
                logging_stack.h )

add_library( FreeRTOS::Logging ALIAS FreeRTOS_logging )
