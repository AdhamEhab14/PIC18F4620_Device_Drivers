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

Bare-metal peripheral drivers for the STM32F103RB (Nucleo, LQFP64/Medium-density), built directly on CMSIS register access (no HAL), as a single STM32CubeIDE project: [`stm32/drivers/`](stm32/drivers/).

- **NVIC / SCB** (`Inc/NVIC/`) — Nested Vectored Interrupt Controller + System Control Block (priority grouping).
- **RCC** (`Inc/RCC/`) — oscillators (HSE/HSI/LSE/LSI), PLL, system/bus clock configuration, peripheral clock gating.
- **FLASH** (`Inc/FLASH/`) — wait-state/latency configuration, unlock/lock, page erase, mass erase, half-word/word programming.
- **GPIO** (`Inc/GPIO/`) — pin mode (input/output/analog/AF), pull, speed, read/write/toggle.
- **SysTick** (`Inc/SysTick/`) — millisecond tick with blocking delay (`HAL_Delay`), plus single-shot/periodic callback-driven intervals.
- **DMA** (`Inc/DMA/`) — DMA1 channel configuration and memory-to-memory / peripheral transfers (this device has no DMA2).

All drivers share `Inc/Common/` (`Std_Types.h`, `HAL_Status.h`) and are aggregated by `Inc/CortexM3_Interface.h`. Later drivers build on earlier ones (GPIO/DMA need RCC's clock-enable macros; SysTick's delay accuracy depends on the RCC-configured HCLK), so they live in one project rather than isolated per-peripheral ones. Import `stm32/drivers/` directly into CubeIDE to build and flash.

## License

MIT.
