/*
 * CortexM3_Core_SysTick.c
 *
 *  Created on: Jul 12, 2026
 *      Author: Adham
 */

#include "SysTick/CortexM3_Core_SysTick.h"


/* Free-running millisecond counter, incremented once per SysTick interrupt */
static volatile uint32_t msTicks = 0U;

/* Callback registered by SysTick_SingleInterval / SysTick_PeriodicInterval, invoked from SysTick_Handler */
static volatile SysTick_CallbackType IntervalCallback = NULL_PTR;

/* 1 while the active interval is single-shot (handler stops the timer after one firing), 0 for periodic/ms-tick */
static volatile boolean IsSingleShot = 0U;


/**
 * brief	Shared setup for SysTick_SingleInterval / SysTick_PeriodicInterval
 * details	Validates the parameters, then programs LOAD/VAL and starts the timer with its interrupt enabled
 * param	Ticks the interval length in raw SysTick counts
 * param	Callback function to invoke from SysTick_Handler
 * param	SingleShot 1 to stop automatically after the first firing, 0 to keep auto-reloading
 * return	HAL_OK / HAL_ERROR, see SysTick_SingleInterval / SysTick_PeriodicInterval
 */
static HAL_StatusType SysTick_StartInterval(uint32_t Ticks, SysTick_CallbackType Callback, boolean SingleShot){

	/* Ticks must be non-zero, must fit the 24-bit LOAD register once decremented, and Callback must be valid */
	if((Ticks == 0U) || ((Ticks - 1U) > SYSTICK_LOAD_RELOAD_MAX) || (Callback == NULL_PTR)){
		return HAL_ERROR;
	}else{
		/* Do Nothing */
	}

	IntervalCallback = Callback;
	IsSingleShot     = SingleShot;

	SYSTICK->LOAD = Ticks - 1U;
	SYSTICK->VAL  = 0U;									// Clear the current counter (also clears COUNTFLAG)
	SYSTICK->CTRL = SYSTICK_CTRL_CLKSOURCE_Msk				// Clock from HCLK (processor clock, not HCLK/8)
					| SYSTICK_CTRL_TICKINT_Msk				// Enable the SysTick exception
					| SYSTICK_CTRL_ENABLE_Msk;				// Start the counter

	return HAL_OK;
}


/**
 * brief	Configure and start the SysTick timer to tick once every millisecond
 * details	Computes the reload value from the given core clock frequency, clocks SysTick from HCLK (no /8),
 * 			enables its interrupt, and starts the counter. The 1ms interrupt drives HAL_Delay/HAL_GetTick.
 * param	SystemCoreClock the current HCLK frequency in Hz (e.g. 72000000 after switching to the PLL)
 * return	HAL_OK    the timer was configured and started successfully
 * return	HAL_ERROR the required reload value does not fit the 24-bit LOAD register (SystemCoreClock too high)
 */
HAL_StatusType SysTick_Init(uint32_t SystemCoreClock){

	uint32_t reload = (SystemCoreClock / 1000U) - 1U;

	/* The LOAD register is only 24 bits wide */
	if(reload > SYSTICK_LOAD_RELOAD_MAX){
		return HAL_ERROR;
	}else{
		/* Do Nothing */
	}

	msTicks = 0U;

	SYSTICK->LOAD = reload;								// Set the 1ms period
	SYSTICK->VAL  = 0U;									// Clear the current counter (also clears COUNTFLAG)
	SYSTICK->CTRL = SYSTICK_CTRL_CLKSOURCE_Msk				// Clock from HCLK (processor clock, not HCLK/8)
					| SYSTICK_CTRL_TICKINT_Msk				// Enable the SysTick exception
					| SYSTICK_CTRL_ENABLE_Msk;				// Start the counter

	return HAL_OK;
}

/**
 * brief	Blocking delay
 * details	Busy-waits until the millisecond tick counter (incremented by SysTick_Handler) has advanced by
 * 			at least Delay milliseconds
 * param	Delay the number of milliseconds to wait
 * note		SysTick_Init must be called first, and interrupts must be enabled (no __disable_irq without a later enable)
 */
void HAL_Delay(uint32_t Delay){

	uint32_t startTick = msTicks;

	while((msTicks - startTick) < Delay){
		/* Busy-wait; msTicks is advanced by SysTick_Handler */
	}
}

/**
 * brief	Get tick count
 * details	Reads the free-running millisecond counter maintained by SysTick_Handler
 * return	Number of milliseconds elapsed since SysTick_Init was called
 */
uint32_t HAL_GetTick(void){
	return msTicks;
}

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
HAL_StatusType SysTick_SingleInterval(uint32_t Ticks, SysTick_CallbackType Callback){
	return SysTick_StartInterval(Ticks, Callback, 1U);
}

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
HAL_StatusType SysTick_PeriodicInterval(uint32_t Ticks, SysTick_CallbackType Callback){
	return SysTick_StartInterval(Ticks, Callback, 0U);
}

/**
 * brief	Stop the currently active single/periodic interval
 * details	Disables the SysTick counter and its interrupt, and clears any registered callback
 * note		Also stops the 1ms tick if that is what was running - call SysTick_Init to resume it
 */
void SysTick_StopInterval(void){
	SYSTICK->CTRL   &= ~(SYSTICK_CTRL_ENABLE_Msk | SYSTICK_CTRL_TICKINT_Msk);
	IntervalCallback = NULL_PTR;
	IsSingleShot     = 0U;
}

/**
 * brief	Get the number of ticks elapsed in the current interval
 * details	Computed as LOAD - VAL; meaningful for the interval configured by SysTick_Init,
 * 			SysTick_SingleInterval, or SysTick_PeriodicInterval, whichever is currently active
 * return	Ticks elapsed since the counter last reloaded
 */
uint32_t SysTick_GetElapsedTicks(void){
	return (SYSTICK->LOAD - SYSTICK->VAL);
}

/**
 * brief	Get the number of ticks remaining in the current interval
 * details	Directly reads the SysTick down-counter (VAL); does not require an interval to be active
 * return	Ticks left before the counter reaches 0 and the current interval elapses
 */
uint32_t SysTick_GetRemainingTicks(void){
	return SYSTICK->VAL;
}

/**
 * brief	SysTick interrupt handler
 * details	Fires once every configured period. Always advances the millisecond tick counter used by
 * 			HAL_Delay/HAL_GetTick (accurate only while SysTick is still running its 1ms period), then
 * 			invokes the registered interval callback, if any. If the active interval is single-shot,
 * 			stops the timer after this firing.
 * note		Name is fixed by the startup file's weak vector table entry - do not rename
 */
void SysTick_Handler(void){

	msTicks++;

	if(IntervalCallback != NULL_PTR){
		IntervalCallback();
	}else{
		/* Do Nothing */
	}

	if(IsSingleShot == 1U){
		SYSTICK->CTRL &= ~(SYSTICK_CTRL_ENABLE_Msk | SYSTICK_CTRL_TICKINT_Msk);
		IntervalCallback = NULL_PTR;
		IsSingleShot     = 0U;
	}else{
		/* Do Nothing */
	}
}


