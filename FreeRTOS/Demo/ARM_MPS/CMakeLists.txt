cmake_minimum_required(VERSION 3.22)

include( toolchain/gcc-arm.cmake )

project( arm_mps_demo LANGUAGES C ASM)

include( freertos_kernel.cmake)

add_library( runtime INTERFACE )

target_compile_options(
    runtime
    INTERFACE
        -Wall
        -Wextra
        # -Wpedantic
)

target_sources(
    runtime
    INTERFACE
        kernel_hooks.c
        fault_handlers.c
        # fault_handlers_asm.S
        irq_vectors_core.c
        startup.c
        lib/newlibc/newlibc_retarget_lock.c
        lib/newlibc/newlibc_heap.c
        include/FreeRTOSConfig.h
        include/irq_vectors.h
        include/arm/arch_armv6m.h
        include/arm/arch_armv7m.h
        include/arm/arch_armv8.1m_main.h
        include/arm/arch_armv8m_base.h
        include/arm/arch_armv8m_main.h
        include/arm/arch_cortex_m.h
        include/arm/core_cortex_m0.h
        include/arm/core_cortex_m0plus.h
        include/arm/core_cortex_m23.h
        include/arm/core_cortex_m3.h
        include/arm/core_cortex_m33.h
        include/arm/core_cortex_m4.h
        include/arm/core_cortex_m55.h
        include/arm/core_cortex_m7.h
        include/arm/soc_mps2_an382_cm0.h
        include/arm/soc_mps2_an383_cm0plus.h
        include/arm/soc_mps2_an385_cm3.h
        include/arm/soc_mps2_an386_cm4.h
        include/arm/soc_mps2_an500_cm7.h
        include/arm/soc_mps2_an505_cm33.h
        include/arm/soc_mps2_an519_cm23.h
        include/arm/soc_mps3_an547_cm55.h
)

target_include_directories(
    runtime
    INTERFACE
        include
        include/arm )

set( TARGET_LIST
    #  mps2_an382_cm0
    #  mps2_an383_cm0plus
     mps2_an385_cm3
     mps2_an386_cm4
    #  mps2_an386_cm4_hardfp
     mps2_an500_cm7
    #  mps2_an500_cm7_hardfp
     mps2_an519_cm23
     mps2_an505_cm33
    #  mps2_an505_cm33_hardfp
    #  mps3_an547_cm55
    #  mps3_an547_cm55_mve
    #  mps3_an547_cm55_hardfp
    #  mps3_an547_cm55_hardfp_mve
    #  mps3_an547_cm55_hardfp_mvefp
)

set( TARGET_PORT_LIST
    #  port_armv6m
    #  port_armv6m
     port_armv7m
     port_armv7em
    #  port_armv7em_hardfp
     port_armv7em
    #  port_armv7em
     port_armv8m_base
     port_armv8m_main
    #  port_armv8m_main
    #  port_armv8m_main
    #  port_armv8m_main
    #  port_armv8m_main
    #  port_armv8m_main
    #  port_armv8m_main
)

foreach( target_name port_name IN ZIP_LISTS TARGET_LIST TARGET_PORT_LIST )
    message("Target: ${target_name}, port: ${port_name}")

    add_library(
        rt_${target_name}
    )

    target_newlibc_nano( rt_${target_name} )
    target_enable_semihost( rt_${target_name} )

    target_link_libraries(
        rt_${target_name}
        PUBLIC
            target_${target_name}
            runtime
            kernel
            ${port_name} )

    add_executable(
        ${target_name}_blink_demo.elf
        main_blinky.c
    )

    target_link_libraries(
        ${target_name}_blink_demo.elf
        PRIVATE
            rt_${target_name}
    )

    add_executable(
        ${target_name}_test_hardfault.elf
        main_test_hardfault.c
    )

    target_link_libraries(
        ${target_name}_test_hardfault.elf
        PRIVATE
            rt_${target_name}
    )

    add_executable(
        ${target_name}_test_hardfault_task.elf
        main_test_hardfault_from_task.c
    )

    target_link_libraries(
        ${target_name}_test_hardfault_task.elf
        PRIVATE
            rt_${target_name}
    )

endforeach()
