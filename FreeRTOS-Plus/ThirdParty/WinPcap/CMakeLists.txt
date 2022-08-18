add_library( WinPcap INTERFACE )

# if("${CMAKE_GENERATOR_PLATFORM}" STREQUAL "x64")
    target_link_libraries( WinPcap INTERFACE ${CMAKE_CURRENT_LIST_DIR}/lib/x64/wpcap.lib )
# elseif("${CMAKE_GENERATOR_PLATFORM}" STREQUAL "Win32")
#     target_link_libraries( WinPcap INTERFACE ${CMAKE_CURRENT_LIST_DIR}/lib/wpcap.lib )
# endif()

target_include_directories( WinPcap INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include )
