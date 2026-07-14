/*
 * CortexM3_Core_RCC.h
 *
 *  Created on: Jul 12, 2026
 *      Author: Adham
 */

#ifndef RCC_CORTEXM3_CORE_RCC_H_
#define RCC_CORTEXM3_CORE_RCC_H_

/* ---------------------------------------- Includes Start ---------------------------------------- */

#include "Common/Std_Types.h"
#include "Common/HAL_Status.h"

/* ---------------------------------------- Includes End ------------------------------------------ */


/* ---------------------------------------- Macro Declaration Start ------------------------------- */

/* Peripheral memory map */
#define PERIPH_BASE			(0x40000000UL)						// Peripheral base address in the alias region
#define AHBPERIPH_BASE		(PERIPH_BASE + 0x00020000UL)		// AHB peripherals base address = 0x40020000
#define RCC_BASE			(AHBPERIPH_BASE + 0x00001000UL)		// RCC base address        = 0x40021000
#define RCC					((RCC_Type*)RCC_BASE)				// Pointer to the base address of the RCC registers memory block


/* ******************* Bit definition for RCC_APB2ENR register ******************* */
#define RCC_APB2ENR_AFIOEN_Pos		(0U)
#define RCC_APB2ENR_AFIOEN_Msk		(0x1UL << RCC_APB2ENR_AFIOEN_Pos)		/*!< 0x00000001 */
#define RCC_APB2ENR_IOPAEN_Pos		(2U)
#define RCC_APB2ENR_IOPAEN_Msk		(0x1UL << RCC_APB2ENR_IOPAEN_Pos)		/*!< 0x00000004 */
#define RCC_APB2ENR_IOPBEN_Pos		(3U)
#define RCC_APB2ENR_IOPBEN_Msk		(0x1UL << RCC_APB2ENR_IOPBEN_Pos)		/*!< 0x00000008 */
#define RCC_APB2ENR_IOPCEN_Pos		(4U)
#define RCC_APB2ENR_IOPCEN_Msk		(0x1UL << RCC_APB2ENR_IOPCEN_Pos)		/*!< 0x00000010 */
#define RCC_APB2ENR_IOPDEN_Pos		(5U)
#define RCC_APB2ENR_IOPDEN_Msk		(0x1UL << RCC_APB2ENR_IOPDEN_Pos)		/*!< 0x00000020 */

/* ******************* Bit definition for RCC_AHBENR register ******************* */
#define RCC_AHBENR_DMA1EN_Pos		(0U)
#define RCC_AHBENR_DMA1EN_Msk		(0x1UL << RCC_AHBENR_DMA1EN_Pos)		/*!< 0x00000001 */


/* ******************* Bit definition for RCC_CR register ******************* */
#define RCC_CR_HSION_Pos			(0U)
#define RCC_CR_HSION_Msk			(0x1UL << RCC_CR_HSION_Pos)				/*!< 0x00000001 */
#define RCC_CR_HSIRDY_Pos			(1U)
#define RCC_CR_HSIRDY_Msk			(0x1UL << RCC_CR_HSIRDY_Pos)			/*!< 0x00000002 */
#define RCC_CR_HSEON_Pos			(16U)
#define RCC_CR_HSEON_Msk			(0x1UL << RCC_CR_HSEON_Pos)				/*!< 0x00010000 */
#define RCC_CR_HSERDY_Pos			(17U)
#define RCC_CR_HSERDY_Msk			(0x1UL << RCC_CR_HSERDY_Pos)			/*!< 0x00020000 */
#define RCC_CR_HSEBYP_Pos			(18U)
#define RCC_CR_HSEBYP_Msk			(0x1UL << RCC_CR_HSEBYP_Pos)			/*!< 0x00040000 */
#define RCC_CR_PLLON_Pos			(24U)
#define RCC_CR_PLLON_Msk			(0x1UL << RCC_CR_PLLON_Pos)				/*!< 0x01000000 */
#define RCC_CR_PLLRDY_Pos			(25U)
#define RCC_CR_PLLRDY_Msk			(0x1UL << RCC_CR_PLLRDY_Pos)			/*!< 0x02000000 */

/* ******************* Bit definition for RCC_BDCR register (LSE lives here) ******************* */
#define RCC_BDCR_LSEON_Pos			(0U)
#define RCC_BDCR_LSEON_Msk			(0x1UL << RCC_BDCR_LSEON_Pos)			/*!< 0x00000001 */
#define RCC_BDCR_LSERDY_Pos			(1U)
#define RCC_BDCR_LSERDY_Msk			(0x1UL << RCC_BDCR_LSERDY_Pos)			/*!< 0x00000002 */
#define RCC_BDCR_LSEBYP_Pos			(2U)
#define RCC_BDCR_LSEBYP_Msk			(0x1UL << RCC_BDCR_LSEBYP_Pos)			/*!< 0x00000004 */

/* ******************* Bit definition for RCC_CSR register (LSI lives here) ******************* */
#define RCC_CSR_LSION_Pos			(0U)
#define RCC_CSR_LSION_Msk			(0x1UL << RCC_CSR_LSION_Pos)			/*!< 0x00000001 */
#define RCC_CSR_LSIRDY_Pos			(1U)
#define RCC_CSR_LSIRDY_Msk			(0x1UL << RCC_CSR_LSIRDY_Pos)			/*!< 0x00000002 */


/* ******************* Bit definition for RCC_CFGR register ******************* */
/* SW: System clock switch [1:0] */
#define RCC_CFGR_SW_Pos				(0U)
#define RCC_CFGR_SW_Msk				(0x3UL << RCC_CFGR_SW_Pos)				/*!< 0x00000003 */
#define RCC_CFGR_SW_HSI				(0x00000000U)							// HSI selected as system clock
#define RCC_CFGR_SW_HSE				(0x00000001U)							// HSE selected as system clock
#define RCC_CFGR_SW_PLL				(0x00000002U)							// PLL selected as system clock
/* SWS: System clock switch status [3:2] */
#define RCC_CFGR_SWS_Pos			(2U)
#define RCC_CFGR_SWS_Msk			(0x3UL << RCC_CFGR_SWS_Pos)				/*!< 0x0000000C */
#define RCC_CFGR_SWS_HSI			(0x00000000U)							// HSI used as system clock
#define RCC_CFGR_SWS_HSE			(0x00000004U)							// HSE used as system clock
#define RCC_CFGR_SWS_PLL			(0x00000008U)							// PLL used as system clock
/* HPRE: AHB prescaler [7:4]  (SYSCLK -> HCLK) */
#define RCC_CFGR_HPRE_Pos			(4U)
#define RCC_CFGR_HPRE_Msk			(0xFUL << RCC_CFGR_HPRE_Pos)			/*!< 0x000000F0 */
#define RCC_CFGR_HPRE_DIV1			(0x00000000U)							// SYSCLK not divided
#define RCC_CFGR_HPRE_DIV2			(0x00000080U)							// SYSCLK / 2
#define RCC_CFGR_HPRE_DIV4			(0x00000090U)							// SYSCLK / 4
#define RCC_CFGR_HPRE_DIV8			(0x000000A0U)							// SYSCLK / 8
#define RCC_CFGR_HPRE_DIV16			(0x000000B0U)							// SYSCLK / 16
#define RCC_CFGR_HPRE_DIV64			(0x000000C0U)							// SYSCLK / 64
#define RCC_CFGR_HPRE_DIV128		(0x000000D0U)							// SYSCLK / 128
#define RCC_CFGR_HPRE_DIV256		(0x000000E0U)							// SYSCLK / 256
#define RCC_CFGR_HPRE_DIV512		(0x000000F0U)							// SYSCLK / 512
/* PPRE1: APB1 low-speed prescaler [10:8]  (HCLK -> PCLK1, max 36 MHz) */
#define RCC_CFGR_PPRE1_Pos			(8U)
#define RCC_CFGR_PPRE1_Msk			(0x7UL << RCC_CFGR_PPRE1_Pos)			/*!< 0x00000700 */
#define RCC_CFGR_PPRE1_DIV1			(0x00000000U)							// HCLK not divided
#define RCC_CFGR_PPRE1_DIV2			(0x00000400U)							// HCLK / 2
#define RCC_CFGR_PPRE1_DIV4			(0x00000500U)							// HCLK / 4
#define RCC_CFGR_PPRE1_DIV8			(0x00000600U)							// HCLK / 8
#define RCC_CFGR_PPRE1_DIV16		(0x00000700U)							// HCLK / 16
/* PPRE2: APB2 high-speed prescaler [13:11]  (HCLK -> PCLK2) */
#define RCC_CFGR_PPRE2_Pos			(11U)
#define RCC_CFGR_PPRE2_Msk			(0x7UL << RCC_CFGR_PPRE2_Pos)			/*!< 0x00003800 */
#define RCC_CFGR_PPRE2_DIV1			(0x00000000U)							// HCLK not divided
#define RCC_CFGR_PPRE2_DIV2			(0x00002000U)							// HCLK / 2
#define RCC_CFGR_PPRE2_DIV4			(0x00002800U)							// HCLK / 4
#define RCC_CFGR_PPRE2_DIV8			(0x00003000U)							// HCLK / 8
#define RCC_CFGR_PPRE2_DIV16		(0x00003800U)							// HCLK / 16
/* PLLSRC: PLL entry clock source [16] */
#define RCC_CFGR_PLLSRC_Pos			(16U)
#define RCC_CFGR_PLLSRC_Msk			(0x1UL << RCC_CFGR_PLLSRC_Pos)			/*!< 0x00010000 */
/* PLLXTPRE: HSE divider for PLL entry [17] */
#define RCC_CFGR_PLLXTPRE_Pos		(17U)
#define RCC_CFGR_PLLXTPRE_Msk		(0x1UL << RCC_CFGR_PLLXTPRE_Pos)		/*!< 0x00020000 */
/* PLLMUL: PLL multiplication factor [21:18] */
#define RCC_CFGR_PLLMUL_Pos			(18U)
#define RCC_CFGR_PLLMUL_Msk			(0xFUL << RCC_CFGR_PLLMUL_Pos)			/*!< 0x003C0000 */


/* ******************* Oscillator Type (bit-mask, values may be OR'd together) ******************* */
#define RCC_OSCILLATORTYPE_NONE		(0x00000000U)
#define RCC_OSCILLATORTYPE_HSE		(0x00000001U)
#define RCC_OSCILLATORTYPE_HSI		(0x00000002U)
#define RCC_OSCILLATORTYPE_LSE		(0x00000004U)
#define RCC_OSCILLATORTYPE_LSI		(0x00000008U)

/* ******************* HSE Config ******************* */
#define RCC_HSE_OFF					(0x00000000U)
#define RCC_HSE_ON					(0x00000001U)

/* ******************* HSI Config ******************* */
#define RCC_HSI_OFF					(0x00000000U)
#define RCC_HSI_ON					(0x00000001U)

/* ******************* LSE Config ******************* */
#define RCC_LSE_OFF					(0x00000000U)
#define RCC_LSE_ON					(0x00000001U)

/* ******************* LSI Config ******************* */
#define RCC_LSI_OFF					(0x00000000U)
#define RCC_LSI_ON					(0x00000001U)

/* ******************* PLL State ******************* */
#define RCC_PLL_NONE				(0x00000000U)							// Do not touch the PLL
#define RCC_PLL_OFF					(0x00000001U)							// Turn the PLL off
#define RCC_PLL_ON					(0x00000002U)							// Turn the PLL on

/* ******************* PLL entry clock source (CFGR.PLLSRC + PLLXTPRE) ******************* */
#define RCC_PLLSOURCE_HSI_DIV2		(0x00000000U)							// HSI/2 -> PLL input
#define RCC_PLLSOURCE_HSE			(RCC_CFGR_PLLSRC_Msk)					// HSE   -> PLL input (not divided)
#define RCC_PLLSOURCE_HSE_DIV2		(RCC_CFGR_PLLSRC_Msk | RCC_CFGR_PLLXTPRE_Msk)	// HSE/2 -> PLL input

/* ******************* PLL multiplication factor (value written to CFGR.PLLMUL) ******************* */
#define RCC_PLL_MUL2				(0x00000000U)
#define RCC_PLL_MUL3				(0x00040000U)
#define RCC_PLL_MUL4				(0x00080000U)
#define RCC_PLL_MUL5				(0x000C0000U)
#define RCC_PLL_MUL6				(0x00100000U)
#define RCC_PLL_MUL7				(0x00140000U)
#define RCC_PLL_MUL8				(0x00180000U)
#define RCC_PLL_MUL9				(0x001C0000U)							// 8 MHz HSE x9 = 72 MHz
#define RCC_PLL_MUL10				(0x00200000U)
#define RCC_PLL_MUL11				(0x00240000U)
#define RCC_PLL_MUL12				(0x00280000U)
#define RCC_PLL_MUL13				(0x002C0000U)
#define RCC_PLL_MUL14				(0x00300000U)
#define RCC_PLL_MUL15				(0x00340000U)
#define RCC_PLL_MUL16				(0x00380000U)

/* ******************* Oscillator ready timeouts (software loop counts) ******************* */
#define RCC_HSE_TIMEOUT_VALUE		(0x00005000U)
#define RCC_HSI_TIMEOUT_VALUE		(0x00002000U)
#define RCC_LSE_TIMEOUT_VALUE		(0x00005000U)
#define RCC_LSI_TIMEOUT_VALUE		(0x00002000U)
#define RCC_PLL_TIMEOUT_VALUE		(0x00005000U)


/* ******************* Clock Type to configure (bit-mask, values may be OR'd together) ******************* */
#define RCC_CLOCKTYPE_SYSCLK		(0x00000001U)							// Configure the SYSCLK source
#define RCC_CLOCKTYPE_HCLK			(0x00000002U)							// Configure the AHB  prescaler (HCLK)
#define RCC_CLOCKTYPE_PCLK1			(0x00000004U)							// Configure the APB1 prescaler (PCLK1)
#define RCC_CLOCKTYPE_PCLK2			(0x00000008U)							// Configure the APB2 prescaler (PCLK2)

/* ******************* System Clock Source (value written to CFGR.SW) ******************* */
#define RCC_SYSCLKSOURCE_HSI		RCC_CFGR_SW_HSI							// HSI drives SYSCLK
#define RCC_SYSCLKSOURCE_HSE		RCC_CFGR_SW_HSE							// HSE drives SYSCLK
#define RCC_SYSCLKSOURCE_PLL		RCC_CFGR_SW_PLL							// PLL drives SYSCLK

/* ******************* AHB Clock Divider (SYSCLK -> HCLK) ******************* */
#define RCC_SYSCLK_AHB_DIV1			RCC_CFGR_HPRE_DIV1
#define RCC_SYSCLK_AHB_DIV2			RCC_CFGR_HPRE_DIV2
#define RCC_SYSCLK_AHB_DIV4			RCC_CFGR_HPRE_DIV4
#define RCC_SYSCLK_AHB_DIV8			RCC_CFGR_HPRE_DIV8
#define RCC_SYSCLK_AHB_DIV16		RCC_CFGR_HPRE_DIV16
#define RCC_SYSCLK_AHB_DIV64		RCC_CFGR_HPRE_DIV64
#define RCC_SYSCLK_AHB_DIV128		RCC_CFGR_HPRE_DIV128
#define RCC_SYSCLK_AHB_DIV256		RCC_CFGR_HPRE_DIV256
#define RCC_SYSCLK_AHB_DIV512		RCC_CFGR_HPRE_DIV512

/* ******************* APB1 Clock Divider (HCLK -> PCLK1) ******************* */
#define RCC_HCLK_APB1_DIV1			RCC_CFGR_PPRE1_DIV1
#define RCC_HCLK_APB1_DIV2			RCC_CFGR_PPRE1_DIV2
#define RCC_HCLK_APB1_DIV4			RCC_CFGR_PPRE1_DIV4
#define RCC_HCLK_APB1_DIV8			RCC_CFGR_PPRE1_DIV8
#define RCC_HCLK_APB1_DIV16			RCC_CFGR_PPRE1_DIV16

/* ******************* APB2 Clock Divider (HCLK -> PCLK2) ******************* */
#define RCC_HCLK_APB2_DIV1			RCC_CFGR_PPRE2_DIV1
#define RCC_HCLK_APB2_DIV2			RCC_CFGR_PPRE2_DIV2
#define RCC_HCLK_APB2_DIV4			RCC_CFGR_PPRE2_DIV4
#define RCC_HCLK_APB2_DIV8			RCC_CFGR_PPRE2_DIV8
#define RCC_HCLK_APB2_DIV16			RCC_CFGR_PPRE2_DIV16

/* ******************* System clock switch timeout (software loop count) ******************* */
#define RCC_SYSCLK_TIMEOUT_VALUE	(0x00005000U)

/* ---------------------------------------- Macro Declaration End --------------------------------- */


/* ---------------------------------------- Macro Function Declarations Start --------------------- */

/* GPIO Port Clock Enable (GPIOs are on the APB2 bus on STM32F1) */
#define RCC_GPIOA_CLK_ENABLE()		(RCC->APB2ENR |= (RCC_APB2ENR_IOPAEN_Msk))
#define RCC_GPIOB_CLK_ENABLE()		(RCC->APB2ENR |= (RCC_APB2ENR_IOPBEN_Msk))
#define RCC_GPIOC_CLK_ENABLE()		(RCC->APB2ENR |= (RCC_APB2ENR_IOPCEN_Msk))
#define RCC_GPIOD_CLK_ENABLE()		(RCC->APB2ENR |= (RCC_APB2ENR_IOPDEN_Msk))

/* GPIO Port Clock Disable */
#define RCC_GPIOA_CLK_DISABLE()		(RCC->APB2ENR &= ~(RCC_APB2ENR_IOPAEN_Msk))
#define RCC_GPIOB_CLK_DISABLE()		(RCC->APB2ENR &= ~(RCC_APB2ENR_IOPBEN_Msk))
#define RCC_GPIOC_CLK_DISABLE()		(RCC->APB2ENR &= ~(RCC_APB2ENR_IOPCEN_Msk))
#define RCC_GPIOD_CLK_DISABLE()		(RCC->APB2ENR &= ~(RCC_APB2ENR_IOPDEN_Msk))

/* Alternate Function IO Clock Enable / Disable (needed for EXTI / pin remap) */
#define RCC_AFIO_CLK_ENABLE()		(RCC->APB2ENR |= (RCC_APB2ENR_AFIOEN_Msk))
#define RCC_AFIO_CLK_DISABLE()		(RCC->APB2ENR &= ~(RCC_APB2ENR_AFIOEN_Msk))

/* DMA1 Clock Enable / Disable (DMA1 is on AHBENR; this device has no DMA2 - Medium-density part) */
#define RCC_DMA1_CLK_ENABLE()		(RCC->AHBENR |= (RCC_AHBENR_DMA1EN_Msk))
#define RCC_DMA1_CLK_DISABLE()		(RCC->AHBENR &= ~(RCC_AHBENR_DMA1EN_Msk))

/* ---------------------------------------- Macro Function Declarations End ----------------------- */


/* ---------------------------------------- Data Type Declaration Start --------------------------- */

/* Creating a memory block for the RCC Registers */
typedef struct{
	volatile uint32_t CR;				// Offset: 0x00 (R/W) Clock Control Register
	volatile uint32_t CFGR;			// Offset: 0x04 (R/W) Clock Configuration Register
	volatile uint32_t CIR;				// Offset: 0x08 (R/W) Clock Interrupt Register
	volatile uint32_t APB2RSTR;		// Offset: 0x0C (R/W) APB2 Peripheral Reset Register
	volatile uint32_t APB1RSTR;		// Offset: 0x10 (R/W) APB1 Peripheral Reset Register
	volatile uint32_t AHBENR;			// Offset: 0x14 (R/W) AHB Peripheral Clock Enable Register
	volatile uint32_t APB2ENR;			// Offset: 0x18 (R/W) APB2 Peripheral Clock Enable Register
	volatile uint32_t APB1ENR;			// Offset: 0x1C (R/W) APB1 Peripheral Clock Enable Register
	volatile uint32_t BDCR;			// Offset: 0x20 (R/W) Backup Domain Control Register
	volatile uint32_t CSR;				// Offset: 0x24 (R/W) Control/Status Register
}RCC_Type;

/* PLL configuration sub-structure (embedded inside RCC_OscInitType) */
typedef struct{
	uint32_t PLLState;				// The new state of the PLL. A value of @ref RCC_PLL_x
	uint32_t PLLSource;				// The PLL input clock source. A value of @ref RCC_PLLSOURCE_x
	uint32_t PLLMUL;				// The PLL multiplication factor. A value of @ref RCC_PLL_MULx
}RCC_PLLInitType;

/* RCC Oscillator configuration structure (filled by the user, passed to HAL_RCC_OscConfig) */
typedef struct{
	uint32_t OscillatorType;		// The oscillators to be configured. A combination (OR) of @ref RCC_OSCILLATORTYPE_x
	uint32_t HSEState;				// The new state of the HSE. A value of @ref RCC_HSE_x
	uint32_t HSIState;				// The new state of the HSI. A value of @ref RCC_HSI_x
	uint32_t LSEState;				// The new state of the LSE. A value of @ref RCC_LSE_x
	uint32_t LSIState;				// The new state of the LSI. A value of @ref RCC_LSI_x
	RCC_PLLInitType PLL;			// The PLL configuration (set PLL.PLLState to RCC_PLL_NONE to leave it untouched)
}RCC_OscInitType;

/* RCC Clock configuration structure (SYSCLK source + AHB/APB1/APB2 prescalers) */
typedef struct{
	uint32_t ClockType;				// The clocks to configure. A combination (OR) of @ref RCC_CLOCKTYPE_x
	uint32_t SYSCLKSource;			// The source that drives SYSCLK. A value of @ref RCC_SYSCLKSOURCE_x
	uint32_t AHBCLKDivider;			// The AHB  (HCLK)  divider. A value of @ref RCC_SYSCLK_AHB_DIVx
	uint32_t APB1CLKDivider;		// The APB1 (PCLK1) divider. A value of @ref RCC_HCLK_APB1_DIVx
	uint32_t APB2CLKDivider;		// The APB2 (PCLK2) divider. A value of @ref RCC_HCLK_APB2_DIVx
}RCC_ClkInitType;

/* ---------------------------------------- Data Type Declaration End ----------------------------- */


/* ---------------------------------------- Software Interfaces Declaration Start ----------------- */

/**
 * brief	Initializes the RCC oscillators according to the specified parameters in the RCC_OscInitType structure
 * details	Enables/disables and waits for the ready flag of each oscillator (HSE, HSI, LSE, LSI) that is selected
 * 			in the OscillatorType field. Only the oscillators listed in OscillatorType are touched.
 * param	RCC_OscInitStruct pointer to an RCC_OscInitType structure that contains the configuration for the oscillators
 * return	HAL_OK      the selected oscillators were configured successfully
 * return	HAL_ERROR   the passed pointer is NULL
 * return	HAL_TIMEOUT an oscillator did not reach the expected ready state in time
 * note		Configuring the LSE requires backup-domain write access to be enabled first (PWR->CR DBP bit)
 */
HAL_StatusType HAL_RCC_OscConfig(RCC_OscInitType *RCC_OscInitStruct);

/**
 * brief	Configures the system clock source and the AHB / APB1 / APB2 bus prescalers
 * details	Sets the AHB prescaler, switches SYSCLK to the selected source (waiting for the switch to take effect),
 * 			then sets the APB1 and APB2 prescalers. Only the fields selected in ClockType are applied.
 * param	RCC_ClkInitStruct pointer to an RCC_ClkInitType structure that contains the clock configuration
 * return	HAL_OK      the clocks were configured successfully
 * return	HAL_ERROR   the passed pointer is NULL
 * return	HAL_TIMEOUT the system clock switch did not take effect in time (is the chosen source ON and READY?)
 * note		The oscillator chosen as SYSCLKSource must already be running - configure it first with HAL_RCC_OscConfig
 * param	FLatency the flash wait-states to program. A value of @ref FLASH_LATENCY_x (needed above 24 MHz)
 * note		If FLatency is higher than the current setting it is applied BEFORE the switch, otherwise AFTER
 */
HAL_StatusType HAL_RCC_ClockConfig(RCC_ClkInitType *RCC_ClkInitStruct, uint32_t FLatency);

/* ---------------------------------------- Software Interfaces Declaration End ------------------- */

#endif /* RCC_CORTEXM3_CORE_RCC_H_ */
