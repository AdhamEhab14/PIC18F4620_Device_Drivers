/**
 ******************************************************************************
 * @file           : main.c
 * @author         : Adham Ehab
 * @brief          : Main program body
 ******************************************************************************
 */

#include "CortexM3_Interface.h"


void SystemClock_Config(void)
{
	HAL_StatusType  RetStatus = HAL_OK;
	RCC_OscInitType RCC_OscInitStruct = {0};
	RCC_ClkInitType RCC_ClkInitStruct = {0};

	/* 1) Turn the HSE on and run the PLL from it at x9  ->  8 MHz x 9 = 72 MHz */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState       = RCC_HSE_ON;
	RCC_OscInitStruct.PLL.PLLState   = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource  = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLMUL     = RCC_PLL_MUL9;

	RetStatus = HAL_RCC_OscConfig(&RCC_OscInitStruct);
	if(HAL_ERROR == RetStatus){
		/* Oscillator / PLL config failed - put a breakpoint here */
	}else{
		/* Nothing */
	}

	/* 2) Switch SYSCLK to the PLL @ 72 MHz. APB1 must stay <= 36 MHz so it gets /2; AHB and APB2 run full speed.
	 *    72 MHz needs 2 flash wait states, applied automatically before the switch. */
	RCC_ClkInitStruct.ClockType      = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
	RCC_ClkInitStruct.SYSCLKSource   = RCC_SYSCLKSOURCE_PLL;
	RCC_ClkInitStruct.AHBCLKDivider  = RCC_SYSCLK_AHB_DIV1;		// HCLK  = 72 MHz
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_APB1_DIV2;		// PCLK1 = 36 MHz (max allowed)
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_APB2_DIV1;		// PCLK2 = 72 MHz

	RetStatus = HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2);
	if(HAL_ERROR == RetStatus){
		/* Clock config failed - put a breakpoint here */
	}else{
		/* Nothing */
	}
}

#define LED_PORT		GPIOA
#define LED_PIN			GPIO_PIN_5		/* adjust to whichever pin your board's LED is wired to */

/* SysTick reload (LOAD) is only 24 bits wide, so one interval maxes out at 0xFFFFFF ticks
 * (~233ms @ 72 MHz). 200ms and 100ms below both comfortably fit. */
#define BLINK_PERIOD_TICKS		(14400000U)		/* 72 MHz x 0.2s -> toggle the LED every 200ms  */
#define SINGLE_DELAY_TICKS		(7200000U)		/* 72 MHz x 0.1s -> fire once, 100ms after the blink stops */
#define BLINK_TOGGLE_TARGET		(10U)			/* stop the periodic blink after this many toggles (~2s) */

/* volatile so the optimizer keeps them - watch these in Live Expressions while the board runs */
volatile uint32_t ToggleCount      = 0U;		// incremented by Blink_Callback on every periodic firing
volatile uint32_t ElapsedSnapshot  = 0U;		// SysTick_GetElapsedTicks(), sampled every main-loop pass
volatile uint32_t RemainingSnapshot= 0U;		// SysTick_GetRemainingTicks(), sampled every main-loop pass
volatile boolean  SingleStarted    = 0U;		// latches once we've kicked off the single-shot phase
volatile boolean  SingleFired      = 0U;		// set by Single_Callback when the single-shot elapses

/* Periodic callback: toggles the LED and counts how many times it has fired */
void Blink_Callback(void){
	GPIO_TogglePin(LED_PORT, LED_PIN);
	ToggleCount++;
}

/* Single-shot callback: latches the LED solid ON so you can see the single-shot actually fired */
void Single_Callback(void){
	SingleFired = 1U;
	GPIO_WritePin(LED_PORT, LED_PIN, GPIO_PIN_SET);
}

/* DMA memory-to-memory smoke test buffers/results - volatile so the debugger sees real values and the
 * optimizer can't reason the transfer away */
volatile uint32_t DMA_SrcBuffer[8]  = {0x11111111U, 0x22222222U, 0x33333333U, 0x44444444U,
                                        0x55555555U, 0x66666666U, 0x77777777U, 0x88888888U};
volatile uint32_t DMA_DstBuffer[8]  = {0};
volatile HAL_StatusType DMA_InitStatus  = HAL_ERROR;		// expected HAL_OK
volatile HAL_StatusType DMA_StartStatus = HAL_ERROR;		// expected HAL_OK
volatile boolean        DMA_TestPassed  = 0U;				// expected 1 - set after comparing Src vs Dst

int main(void)
{
	GPIO_InitType GPIO_InitStruct = {0};
	DMA_InitType  DMA_InitStruct  = {0};
	uint32_t      i               = 0U;

	RCC_GPIOA_CLK_ENABLE();

	SystemClock_Config();

	/* HCLK is 72 MHz after SystemClock_Config - feed that to SysTick so its period is a true 1ms */
	SysTick_Init(72000000U);

	/* Configure the LED pin as push-pull output */
	GPIO_InitStruct.Pin   = LED_PIN;
	GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Speed = GPIO_SPEED_2MHZ;
	GPIO_Init(LED_PORT, &GPIO_InitStruct);

	/* DMA memory-to-memory smoke test: copy DMA_SrcBuffer into DMA_DstBuffer via DMA1 Channel1, then
	 * verify byte-for-byte. In DMA_DIR_MEM_TO_MEM mode, CPAR is the source and CMAR is the destination -
	 * both sides still need their increment bit set to walk through the 8-word array. */
	RCC_DMA1_CLK_ENABLE();

	DMA_InitStruct.Direction       = DMA_DIR_MEM_TO_MEM;
	DMA_InitStruct.PeriphInc       = DMA_PINC_ENABLE;
	DMA_InitStruct.MemInc          = DMA_MINC_ENABLE;
	DMA_InitStruct.PeriphDataAlign = DMA_PDATAALIGN_WORD;
	DMA_InitStruct.MemDataAlign    = DMA_MDATAALIGN_WORD;
	DMA_InitStruct.Mode            = DMA_MODE_NORMAL;
	DMA_InitStruct.Priority        = DMA_PRIORITY_HIGH;
	DMA_InitStruct.TCInterrupt     = DMA_IT_DISABLE;
	DMA_InitStruct.HTInterrupt     = DMA_IT_DISABLE;
	DMA_InitStruct.TEInterrupt     = DMA_IT_DISABLE;

	DMA_InitStatus  = DMA_Init(DMA1_Channel1, &DMA_InitStruct);
	DMA_StartStatus = DMA_Start(DMA1_Channel1, (uint32_t)DMA_SrcBuffer, (uint32_t)DMA_DstBuffer, 8U);

	while(DMA_GetFlagStatus(1U, DMA_FLAG_TCIF) == 0U){
		/* Busy-wait for the transfer-complete flag; 8 words at high priority finishes almost instantly */
	}
	DMA_ClearFlag(1U, DMA_FLAG_TCIF);
	DMA_Stop(DMA1_Channel1);

	DMA_TestPassed = 1U;
	for(i = 0U; i < 8U; i++){
		if(DMA_SrcBuffer[i] != DMA_DstBuffer[i]){
			DMA_TestPassed = 0U;
		}else{
			/* Do Nothing */
		}
	}

	/* Phase 1: replace the 1ms tick with a 200ms periodic interval that blinks the LED via callback */
	SysTick_PeriodicInterval(BLINK_PERIOD_TICKS, Blink_Callback);

 	while(1){

		/* Sample tick progress on every pass - pause the debugger anytime to see live values */
		ElapsedSnapshot   = SysTick_GetElapsedTicks();
		RemainingSnapshot = SysTick_GetRemainingTicks();

		/* Phase 2: once the LED has blinked enough times, stop the periodic interval and fire
		 * a single-shot 100ms later that latches the LED solid ON. If it lights up and stays on,
		 * PeriodicInterval, StopInterval, and SingleInterval have all been exercised. */
		if((ToggleCount >= BLINK_TOGGLE_TARGET) && (SingleStarted == 0U)){
			SingleStarted = 1U;
			SysTick_StopInterval();
			GPIO_WritePin(LED_PORT, LED_PIN, GPIO_PIN_RESET);
			SysTick_SingleInterval(SINGLE_DELAY_TICKS, Single_Callback);
		}
	}
}
