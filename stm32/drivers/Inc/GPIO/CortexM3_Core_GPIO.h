/*
 * CortexM3_Core_GPIO.h
 *
 *  Created on: Jul 12, 2026
 *      Author: Adham
 */

#ifndef GPIO_CORTEXM3_CORE_GPIO_H_
#define GPIO_CORTEXM3_CORE_GPIO_H_

/* ---------------------------------------- Includes Start ---------------------------------------- */

#include "Common/Std_Types.h"
#include "Common/HAL_Status.h"

/* ---------------------------------------- Includes End ------------------------------------------ */


/* ---------------------------------------- Macro Declaration Start ------------------------------- */

/* Peripheral memory map (GPIOs are on APB2, LQFP64 package exposes ports A-D) */
#define GPIOA_BASE			(0x40010800UL)
#define GPIOB_BASE			(0x40010C00UL)
#define GPIOC_BASE			(0x40011000UL)
#define GPIOD_BASE			(0x40011400UL)

#define GPIOA				((GPIO_Type*)GPIOA_BASE)			// Pointer to the base address of the GPIOA registers memory block
#define GPIOB				((GPIO_Type*)GPIOB_BASE)			// Pointer to the base address of the GPIOB registers memory block
#define GPIOC				((GPIO_Type*)GPIOC_BASE)			// Pointer to the base address of the GPIOC registers memory block
#define GPIOD				((GPIO_Type*)GPIOD_BASE)			// Pointer to the base address of the GPIOD registers memory block


/* ******************* Pin numbers (bit-mask, values may be OR'd together for multi-pin init) ******************* */
#define GPIO_PIN_0			(0x0001U)
#define GPIO_PIN_1			(0x0002U)
#define GPIO_PIN_2			(0x0004U)
#define GPIO_PIN_3			(0x0008U)
#define GPIO_PIN_4			(0x0010U)
#define GPIO_PIN_5			(0x0020U)
#define GPIO_PIN_6			(0x0040U)
#define GPIO_PIN_7			(0x0080U)
#define GPIO_PIN_8			(0x0100U)
#define GPIO_PIN_9			(0x0200U)
#define GPIO_PIN_10			(0x0400U)
#define GPIO_PIN_11			(0x0800U)
#define GPIO_PIN_12			(0x1000U)
#define GPIO_PIN_13			(0x2000U)
#define GPIO_PIN_14			(0x4000U)
#define GPIO_PIN_15			(0x8000U)
#define GPIO_PIN_ALL			(0xFFFFU)

/* ******************* Pin Mode (this driver's own encoding; translated internally to CNF/MODE bits) ******************* */
#define GPIO_MODE_ANALOG		(0x00000000U)			// Analog input                     (CNF=00, MODE=00)
#define GPIO_MODE_INPUT		(0x00000001U)			// Input, CNF decided by the Pull field
#define GPIO_MODE_OUTPUT_PP		(0x00000002U)			// General purpose output push-pull (CNF=00)
#define GPIO_MODE_OUTPUT_OD		(0x00000003U)			// General purpose output open-drain(CNF=01)
#define GPIO_MODE_AF_PP			(0x00000004U)			// Alternate function push-pull     (CNF=10)
#define GPIO_MODE_AF_OD			(0x00000005U)			// Alternate function open-drain    (CNF=11)

/* ******************* Pull configuration (only meaningful when Mode == GPIO_MODE_INPUT) ******************* */
#define GPIO_NOPULL			(0x00000000U)			// Floating input                   (CNF=01)
#define GPIO_PULLUP			(0x00000001U)			// Input with pull-up               (CNF=10, ODR=1)
#define GPIO_PULLDOWN		(0x00000002U)			// Input with pull-down             (CNF=10, ODR=0)

/* ******************* Output Speed (only meaningful for OUTPUT_PP / OUTPUT_OD / AF_PP / AF_OD modes) ******************* */
#define GPIO_SPEED_10MHZ		(0x01U)					// MODE = 01
#define GPIO_SPEED_2MHZ			(0x02U)					// MODE = 10
#define GPIO_SPEED_50MHZ		(0x03U)					// MODE = 11

/* ******************* Pin state (used by GPIO_WritePin / returned by GPIO_ReadPin) ******************* */
#define GPIO_PIN_RESET		(0U)
#define GPIO_PIN_SET			(1U)

/* Bit width of one pin's config field inside CRL/CRH, and how many pins CRL covers */
#define GPIO_CR_FIELD_WIDTH		(4U)
#define GPIO_CR_FIELD_MASK		(0xFU)
#define GPIO_CRL_PIN_COUNT		(8U)

/* ---------------------------------------- Macro Declaration End --------------------------------- */


/* ---------------------------------------- Macro Function Declarations Start --------------------- */

/* ---------------------------------------- Macro Function Declarations End ----------------------- */


/* ---------------------------------------- Data Type Declaration Start --------------------------- */

/* Creating a memory block for the GPIO Registers */
typedef struct{
	volatile uint32_t CRL;				// Offset: 0x00 (R/W) Port Configuration Register Low  (pins 0-7)
	volatile uint32_t CRH;				// Offset: 0x04 (R/W) Port Configuration Register High (pins 8-15)
	volatile uint32_t IDR;				// Offset: 0x08 (R/ ) Port Input Data Register
	volatile uint32_t ODR;				// Offset: 0x0C (R/W) Port Output Data Register
	volatile uint32_t BSRR;			// Offset: 0x10 ( /W) Port Bit Set/Reset Register
	volatile uint32_t BRR;				// Offset: 0x14 ( /W) Port Bit Reset Register
	volatile uint32_t LCKR;			// Offset: 0x18 (R/W) Port Configuration Lock Register
}GPIO_Type;

/* GPIO pin configuration structure (filled by the user, passed to GPIO_Init) */
typedef struct{
	uint32_t Pin;				// The pin(s) to configure. A combination (OR) of @ref GPIO_PIN_x
	uint32_t Mode;				// The pin mode. A value of @ref GPIO_MODE_x
	uint32_t Pull;				// Pull-up/down/none. A value of @ref GPIO_x, only used when Mode == GPIO_MODE_INPUT
	uint32_t Speed;				// Output speed. A value of @ref GPIO_SPEED_x, only used for output/AF modes
}GPIO_InitType;

/* ---------------------------------------- Data Type Declaration End ----------------------------- */


/* ---------------------------------------- Software Interfaces Declaration Start ----------------- */

/**
 * brief	Initializes one or more pins of a GPIO port according to the parameters in GPIO_InitStruct
 * details	Writes the CNF/MODE field of every pin selected in GPIO_InitStruct->Pin (CRL for pins 0-7, CRH for
 * 			pins 8-15). For GPIO_MODE_INPUT with GPIO_PULLUP/GPIO_PULLDOWN, also sets/clears the matching ODR
 * 			bit to select the pull direction, as required by the F1 GPIO peripheral.
 * param	GPIOx pointer to the GPIO port (GPIOA .. GPIOD)
 * param	GPIO_InitStruct pointer to a GPIO_InitType structure that contains the pin configuration
 * return	HAL_OK    the pin(s) were configured successfully
 * return	HAL_ERROR either pointer is NULL
 * note		The port's clock (RCC_GPIOx_CLK_ENABLE) and, for AF modes, RCC_AFIO_CLK_ENABLE must be enabled first
 */
HAL_StatusType GPIO_Init(GPIO_Type *GPIOx, GPIO_InitType *GPIO_InitStruct);

/**
 * brief	Writes a logic state to an output pin
 * details	Uses the BSRR register, which sets or clears the requested pin atomically (single register write,
 * 			no read-modify-write), so it is safe to use from both main-line code and interrupts
 * param	GPIOx pointer to the GPIO port (GPIOA .. GPIOD)
 * param	Pin the pin to write. A value of @ref GPIO_PIN_x (single pin)
 * param	PinState the state to write. GPIO_PIN_SET or GPIO_PIN_RESET
 */
void GPIO_WritePin(GPIO_Type *GPIOx, uint32_t Pin, uint32_t PinState);

/**
 * brief	Reads the current logic state of an input (or output) pin
 * param	GPIOx pointer to the GPIO port (GPIOA .. GPIOD)
 * param	Pin the pin to read. A value of @ref GPIO_PIN_x (single pin)
 * return	GPIO_PIN_SET   the pin reads high
 * return	GPIO_PIN_RESET the pin reads low
 */
uint32_t GPIO_ReadPin(GPIO_Type *GPIOx, uint32_t Pin);

/**
 * brief	Toggles the current output state of a pin
 * details	Performs a read-modify-write on ODR; if an ISR can also write the same pin's ODR bit, prefer
 * 			GPIO_WritePin with the ISR-observed state instead to avoid a race
 * param	GPIOx pointer to the GPIO port (GPIOA .. GPIOD)
 * param	Pin the pin to toggle. A value of @ref GPIO_PIN_x (single pin, or several OR'd together)
 */
void GPIO_TogglePin(GPIO_Type *GPIOx, uint32_t Pin);

/* ---------------------------------------- Software Interfaces Declaration End ------------------- */

#endif /* GPIO_CORTEXM3_CORE_GPIO_H_ */
