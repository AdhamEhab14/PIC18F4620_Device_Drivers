# PIC18 & STM32 Peripheral Drivers

Register-level, bare-metal peripheral drivers for two microcontroller platforms, written from the datasheet up (no HAL/vendor driver layer underneath). Built and reused across 5+ embedded projects.

| Platform | MCU / Board | Toolchain |
|---|---|---|
| [`pic18/`](pic18/) | PIC18F46K20 | MPLAB X, XC8 |
| [`stm32/`](stm32/) | STM32F103RB (Nucleo) | STM32CubeIDE |

## pic18/ — PIC18F46K20

A layered driver stack (MCAL + ECU), each peripheral in its own folder with a `_cfg.h` configuration header so drivers are reusable across projects without touching the driver source itself.

**MCAL layer** (`pic18/MCAL_Layer/`): GPIO, ADC, SPI, I2C, EUSART/USART, Timer0–Timer3, CCP (Capture/Compare/PWM), EEPROM, external & internal interrupts, device configuration bits.

**ECU layer** (`pic18/ECU_Layer/`): higher-level components built on top of MCAL — LED, Button, 7-Segment display, Character LCD, Keypad, DC Motor, Relay, Ultrasonic sensor.

Open `pic18/Makefile` / the `nbproject/` files directly in MPLAB X to build.

## stm32/ — STM32F103RB

Bare-metal peripheral drivers for the STM32F103RB, built directly on CMSIS (no HAL), one STM32CubeIDE project per peripheral.

- [`stm32/nvic/`](stm32/nvic/) — Nested Vectored Interrupt Controller (`CortexM3_Core_NVIC`), plus the System Control Block (`CortexM3_Core_SCB`) it depends on for priority grouping.
- [`stm32/rcc/`](stm32/rcc/) — Reset and Clock Control: system/bus clock configuration, peripheral clock gating.
- [`stm32/systick/`](stm32/systick/) — SysTick timer: millisecond tick and blocking/non-blocking delay generation.
- [`stm32/dma/`](stm32/dma/) — Direct Memory Access: memory-to-peripheral and memory-to-memory transfers.
- [`stm32/gpio/`](stm32/gpio/) — General-Purpose I/O: pin mode, speed, and alternate-function configuration.
- [`stm32/flash/`](stm32/flash/) — Flash memory: wait-state/latency configuration and read/write/erase operations.

Each driver folder is a self-contained STM32CubeIDE project (`Inc/`, `Src/`, `Startup/`, linker script) — import it directly into CubeIDE to build and flash.

## License

MIT.
