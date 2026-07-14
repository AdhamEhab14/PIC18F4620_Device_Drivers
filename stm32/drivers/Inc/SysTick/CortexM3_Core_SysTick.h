/*
 * CortexM3_Core_SysTick.h
 *
 *  Created on: Jul 12, 2026
 *      Author: Adham
 */

#ifndef SYSTICK_CORTEXM3_CORE_SYSTICK_H_
#define SYSTICK_CORTEXM3_CORE_SYSTICK_H_

/* ---------------------------------------- Includes Start ---------------------------------------- */

#include "Common/Std_Types.h"
#include "Common/HAL_Status.h"

/* ---------------------------------------- Includes End ------------------------------------------ */


/* ---------------------------------------- Macro Declaration Start ------------------------------- */

/* Memory Mapping of the Core Hardware (fixed Cortex-M3 address, same on every vendor/device) */
#define SYSTICK_BASE	(0xE000E010UL)
#define SYSTICK			((SysTick_Type*)SYSTICK_BASE)		// Pointer to the base address of the SysTick registers memory block

/* ******************* Bit definition for SYSTICK_CTRL register ******************* */
#define SYSTICK_CTRL_ENABLE_Pos			(0U)
#define SYSTICK_CTRL_ENABLE_Msk			(0x1UL << SYSTICK_CTRL_ENABLE_Pos)		/*!< 0x00000001 */
#define SYSTICK_CTRL_TICKINT_Pos			(1U)
#define SYSTICK_CTRL_TICKINT_Msk			(0x1UL << SYSTICK_CTRL_TICKINT_Pos)		/*!< 0x00000002 */
#define SYSTICK_CTRL_CLKSOURCE_Pos			(2U)
#define SYSTICK_CTRL_CLKSOURCE_Msk			(0x1UL << SYSTICK_CTRL_CLKSOURCE_Pos)	/*!< 0x00000004 */
#define SYSTICK_CTRL_COUNTFLAG_Pos			(16U)
#define SYSTICK_CTRL_COUNTFLAG_Msk			(0x1UL << SYSTICK_CTRL_COUNTFLAG_Pos)	/*!< 0x00010000 */

/* ******************* Bit definition for SYSTICK_LOAD register ******************* */
#define SYSTICK_LOAD_RELOAD_Pos				(0U)
#define SYSTICK_LOAD_RELOAD_Msk				(0xFFFFFFUL << SYSTICK_LOAD_RELOAD_Pos)	/*!< 0x00FFFFFF (24-bit) */
#define SYSTICK_LOAD_RELOAD_MAX				(0x00FFFFFFUL)							// Largest value the 24-bit reload register can hold

/* ******************* CLKSOURCE options ******************* */
#define SYSTICK_CLKSOURCE_AHB_DIV8			(0x00000000U)							// SysTick clocked from HCLK/8
#define SYSTICK_CLKSOURCE_AHB				(SYSTICK_CTRL_CLKSOURCE_Msk)				// SysTick clocked from HCLK (processor clock)

/* ---------------------------------------- Macro Declaration End --------------------------------- */


/* ---------------------------------------- Macro Function Declarations Start --------------------- */

/* ---------------------------------------- Macro Function Declarations End ----------------------- */


/* ---------------------------------------- Data Type Declaration Start --------------------------- */

/* Creating a memory block for the SysTick Registers */
typedef struct{
	volatile uint32_t CTRL;			// Offset: 0x00 (R/W) SysTick Control and Status Register
	volatile uint32_t LOAD;			// Offset: 0x04 (R/W) SysTick Reload Value Register
	volatile uint32_t VAL;				// Offset: 0x08 (R/W) SysTick Current Value Register
	volatile uint32_t CALIB;			// Offset: 0x0C (R/ ) SysTick Calibration Value Register
}SysTick_Type;

/* Function pointer type for the interval callbacks (invoked from SysTick_Handler) */
typedef void (*SysTick_CallbackType)(void);

/* ---------------------------------------- Data Type Declaration End ----------------------------- */


/* ---------------------------------------- Software Interfaces Declaration Start ----------------- */

/**
 * brief	Configure and start the SysTick timer to tick once every millisecond
 * details	Computes the reload value from the given core clock frequency, clocks SysTick from HCLK (no /8),
 * 			enables its interrupt, and starts the counter. The 1ms interrupt drives HAL_Delay/HAL_GetTick.
 * param	SystemCoreClock the current HCLK frequency in Hz (e.g. 72000000 after switching to the PLL)
 * return	HAL_OK    the timer was configured and started successfully
 * return	HAL_ERROR the required reload value does not fit the 24-bit LOAD register (SystemCoreClock too high)
 */
HAL_StatusType SysTick_Init(uint32_t SystemCoreClock);

/**
 * brief	Blocking delay
 * details	Busy-waits until the millisecond tick counter (incremented by SysTick_Handler) has advanced by
 * 			at least Delay milliseconds
 * param	Delay the number of milliseconds to wait
 * note		SysTick_Init must be called first, and interrupts must be enabled (no __disable_irq without a later enable)
 */
void HAL_Delay(uint32_t Delay);

/**
 * brief	Get tick count
 * details	Reads the free-running millisecond counter maintained by SysTick_Handler
 * return	Number of milliseconds elapsed since SysTick_Init was called
 */
uint32_t HAL_GetTick(void);

/**
 * brief	Configure SysTick to fire once after the given number of ticks, then stop automatically
 * details	Reprograms LOAD/VAL to the requested period and registers Callback to be invoked from
 * 			SysTick_Handler when it elapses. After firing once, SysTick_Handler disables the timer
 * 			and clears the callback - no further action needed to stop it.
 * param	Ticks the interval length in raw SysTick counts (clock cycles at the configured CLKSOURCE)
 * param	Callback function to invoke (from interrupt context) when the interval elapses
 * return	HAL_OK    the interval was configured and started successfully
 * return	HAL_ERROR Ticks is 0, Ticks does not fit the 24-bit LOAD register, or Callback is NULL
 * note		This reprograms the same hardware timer used by HAL_Delay/HAL_GetTick - while a single/periodic
 * 			interval is active, the 1ms tick is not running; call SysTick_Init again to restore it
 */
HAL_StatusType SysTick_SingleInterval(uint32_t Ticks, SysTick_CallbackType Callback);

/**
 * brief	Configure SysTick to fire repeatedly, once every given number of ticks
 * details	Reprograms LOAD/VAL to the requested period and registers Callback to be invoked from
 * 			SysTick_Handler on every elapse. SysTick auto-reloads in hardware, so no software reload
 * 			is needed between firings; call SysTick_StopInterval to cancel it.
 * param	Ticks the interval length in raw SysTick counts (clock cycles at the configured CLKSOURCE)
 * param	Callback function to invoke (from interrupt context) on every elapse
 * return	HAL_OK    the interval was configured and started successfully
 * return	HAL_ERROR Ticks is 0, Ticks does not fit the 24-bit LOAD register, or Callback is NULL
 * note		This reprograms the same hardware timer used by HAL_Delay/HAL_GetTick - while a single/periodic
 * 			interval is active, the 1ms tick is not running; call SysTick_Init again to restore it
 */
HAL_StatusType SysTick_PeriodicInterval(uint32_t Ticks, SysTick_CallbackType Callback);

/**
 * brief	Stop the currently active single/periodic interval
 * details	Disables the SysTick counter and its interrupt, and clears any registered callback
 * note		Also stops the 1ms tick if that is what was running - call SysTick_Init to resume it
 */
void SysTick_StopInterval(void);

/**
 * brief	Get the number of ticks elapsed in the current interval
 * details	Computed as LOAD - VAL; meaningful for the interval configured by SysTick_Init,
 * 			SysTick_SingleInterval, or SysTick_PeriodicInterval, whichever is currently active
 * return	Ticks elapsed since the counter last reloaded
 */
uint32_t SysTick_GetElapsedTicks(void);

/**
 * brief	Get the number of ticks remaining in the current interval
 * details	Directly reads the SysTick down-counter (VAL); does not require an interval to be active
 * return	Ticks left before the counter reaches 0 and the current interval elapses
 */
uint32_t SysTick_GetRemainingTicks(void);

/* ---------------------------------------- Software Interfaces Declaration End ------------------- */

#endif /* SYSTICK_CORTEXM3_CORE_SYSTICK_H_ */
