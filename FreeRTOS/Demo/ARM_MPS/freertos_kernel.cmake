add_library( kernel INTERFACE )
target_sources(
    kernel
    INTERFACE
        ../../Source/event_groups.c
        ../../Source/list.c
        ../../Source/queue.c
        ../../Source/stream_buffer.c
        ../../Source/tasks.c
        ../../Source/timers.c

    INTERFACE
        ../../Source/include/FreeRTOS.h
        ../../Source/include/StackMacros.h
        ../../Source/include/atomic.h
        ../../Source/include/deprecated_definitions.h
        ../../Source/include/event_groups.h
        ../../Source/include/list.h
        ../../Source/include/message_buffer.h
        ../../Source/include/mpu_prototypes.h
        ../../Source/include/mpu_wrappers.h
        ../../Source/include/newlib-freertos.h
        ../../Source/include/picolibc-freertos.h
        ../../Source/include/portable.h
        ../../Source/include/projdefs.h
        ../../Source/include/queue.h
        ../../Source/include/semphr.h
        ../../Source/include/stack_macros.h
        ../../Source/include/stream_buffer.h
        ../../Source/include/task.h
        ../../Source/include/timers.h

)

target_include_directories(
    kernel
    INTERFACE
        ../../Source/include
)

add_library(
    port_armv6m
    INTERFACE
)

target_include_directories(
    port_armv6m
    INTERFACE
        ../../Source/portable/GCC/ARM_CM0
)

target_sources(
    port_armv6m
    INTERFACE
        ../../Source/portable/GCC/ARM_CM0/port.c
        ../../Source/portable/GCC/ARM_CM0/portmacro.h
)

add_library(
    port_armv7m
    INTERFACE
)

target_include_directories(
    port_armv7m
    INTERFACE
        ../../Source/portable/GCC/ARM_CM3
)

target_sources(
    port_armv7m
    INTERFACE
        ../../Source/portable/GCC/ARM_CM3/port.c
        ../../Source/portable/GCC/ARM_CM3/portmacro.h
)

add_library( port_armv7em ALIAS port_armv7m )

add_library(
    port_armv7em_hardfp
    INTERFACE
)

target_include_directories(
    port_armv7em_hardfp
    INTERFACE
        ../../Source/portable/GCC/ARM_CM4F
)

target_sources(
    port_armv7em_hardfp
    INTERFACE
        ../../Source/portable/GCC/ARM_CM4F/port.c
        ../../Source/portable/GCC/ARM_CM4F/portmacro.h
)

add_library(
    port_armv8m_base
    INTERFACE
)

target_include_directories(
    port_armv8m_base
    INTERFACE
    ../../Source/portable/ARMv8M/non_secure
    ../../Source/portable/ARMv8M/non_secure/portable/GCC/ARM_CM23_NTZ
)

target_sources(
    port_armv8m_base
    INTERFACE
        ../../Source/portable/ARMv8M/non_secure/port.c
        # ../../Source/portable/ARMv8M/non_secure/portable/GCC/ARM_CM23_NTZ/portasm.S
        ../../Source/portable/ARMv8M/non_secure/portable/GCC/ARM_CM23_NTZ/portasm.c
    INTERFACE
        ../../Source/portable/ARMv8M/non_secure/portasm.h
        ../../Source/portable/ARMv8M/non_secure/portmacrocommon.h
        ../../Source/portable/ARMv8M/non_secure/portable/GCC/ARM_CM23_NTZ/portmacro.h

)

add_library(
    port_armv8m_main
    INTERFACE
)

target_include_directories(
    port_armv8m_main
    INTERFACE
        ../../Source/portable/ARMv8M/non_secure
        ../../Source/portable/ARMv8M/non_secure/portable/GCC/ARM_CM33_NTZ
)

target_sources(
    port_armv8m_main
    INTERFACE
        ../../Source/portable/ARMv8M/non_secure/port.c
        ../../Source/portable/ARMv8M/non_secure/portable/GCC/ARM_CM33_NTZ/portasm.c
        # ../../Source/portable/ARMv8M/non_secure/portable/GCC/ARM_CM33/portasm.c
    INTERFACE
        ../../Source/portable/ARMv8M/non_secure/portasm.h
        ../../Source/portable/ARMv8M/non_secure/portmacrocommon.h
        ../../Source/portable/ARMv8M/non_secure/portable/GCC/ARM_CM33_NTZ/portmacro.h
)

add_library(
    port_armv81m_main
    INTERFACE
)

target_include_directories(
    port_armv81m_main
    INTERFACE
        ../../Source/portable/ARMv8M/non_secure
        ../../Source/portable/ARMv8M/non_secure/portable/GCC/ARM_CM55_NTZ
)

target_sources(
    port_armv81m_main
    INTERFACE
        ../../Source/portable/ARMv8M/non_secure/port.c
        ../../Source/portable/ARMv8M/non_secure/portable/GCC/ARM_CM55_NTZ/portasm.c
    INTERFACE
        ../../Source/portable/ARMv8M/non_secure/portasm.h
        ../../Source/portable/ARMv8M/non_secure/portmacrocommon.h
        ../../Source/portable/ARMv8M/non_secure/portable/GCC/ARM_CM55_NTZ/portmacro.h
)
