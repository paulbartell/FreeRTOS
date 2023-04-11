set architecture armv8-m.main

target extended-remote :1234
compare-sections

break HardFault_Handler
