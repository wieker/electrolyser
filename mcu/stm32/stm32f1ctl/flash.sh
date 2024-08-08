openocd -f tools/stm32f4discovery.cfg -c "reset_config none; init; halt; program src/STM32F4Template.elf reset exit"

