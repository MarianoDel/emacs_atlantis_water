# 2Ch Control Board
micro STM32F030K6T6
Two Pote inputs

Before start check uc selection and the configuration
-----------------------------------------------------

* work area size & flash image for flasher
>stm32f0_flash.cfg
>stm32f0_flash_lock.cfg

* work area size for debugger
>stm32f0_gdb.cfg
>stm32f0_reset.cfg

* end of ram; stack size; memory length
>./cmsis_boot/startup/stm32_flash.ld

* pick processor definition, this are in stm32f0xx.h -> if its there, change it in the Makefile
>./cmsis_boot/stm32f0xx.h
>.Makefile



