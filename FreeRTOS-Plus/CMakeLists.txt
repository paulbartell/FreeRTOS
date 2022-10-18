# FreeRTOS Classic Distribution
# Copyright (C) 2022 Amazon.com, Inc. or its affiliates. All Rights Reserved.
# SPDX-License-Identifier: MIT

# Enable verbose output when fetching 3rd party dependencies with FetchContent.
set(FETCHCONTENT_QUIET OFF)

find_package( Git )
if(NOT Git_FOUND)
    message(STATUS "Git was not found.. skipping submodule operations")
endif()

function( check_init_submodule submodule_path )
    if( NOT EXISTS "${CMAKE_CURRENT_LIST_DIR}/${submodule_path}/.git" )
        if( Git_FOUND )
            message( STATUS "Initializing submodule: ${submodule_path}")
            execute_process( COMMAND ${GIT_EXECUTABLE} submodule update --init --checkout ${submodule_path}
                             WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
                             RESULT_VARIABLE GIT_SUBMOD_RESULT)
        else()
            message( FATAL_ERROR "Git binary is not available and library submodule ${submodule_path} was not found." )
        endif()
    endif()
endfunction()

set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR} )
set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib )
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib )

set( FREERTOS_PLUS_CONFIGURED TRUE PARENT_SCOPE )

add_subdirectory( Target ${CMAKE_CURRENT_BINARY_DIR}/target )

# FreeRTOS Kernel file + port
check_init_submodule( ../FreeRTOS/Source )
add_subdirectory( ../FreeRTOS/Source ${CMAKE_CURRENT_BINARY_DIR}/kernel )

# FreeRTOS Kernel depends on hooks provided by FreeRTOS::Platform
target_link_libraries( freertos_kernel FreeRTOS::Platform )

add_subdirectory( Source ${CMAKE_CURRENT_BINARY_DIR}/freertos_plus )

add_subdirectory( ThirdParty ${CMAKE_CURRENT_BINARY_DIR}/third_party )
