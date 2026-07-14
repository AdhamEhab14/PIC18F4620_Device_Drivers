/*
 * CortexM3_Core_RCC.c
 *
 *  Created on: Jul 12, 2026
 *      Author: Adham
 */

#include "RCC/CortexM3_Core_RCC.h"
#include "FLASH/CortexM3_Core_FLASH.h"


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
HAL_StatusType HAL_RCC_OscConfig(RCC_OscInitType *RCC_OscInitStruct){

	uint32_t timeout = 0U;

	/* Check the structure pointer */
	if(RCC_OscInitStruct == NULL_PTR){
		return HAL_ERROR;
	}

	/*------------------------------- HSE Configuration ------------------------*/
	if(((RCC_OscInitStruct->OscillatorType) & RCC_OSCILLATORTYPE_HSE) == RCC_OSCILLATORTYPE_HSE){
		if(RCC_OscInitStruct->HSEState == RCC_HSE_ON){
			/* Turn the HSE oscillator ON */
			RCC->CR |= RCC_CR_HSEON_Msk;
			/* Wait until HSE is ready (HSERDY = 1) or the timeout elapses */
			timeout = RCC_HSE_TIMEOUT_VALUE;
			while((RCC->CR & RCC_CR_HSERDY_Msk) == 0U){
				if(timeout-- == 0U){
					return HAL_TIMEOUT;
				}
			}
		}else{
			/* Turn the HSE oscillator OFF */
			RCC->CR &= ~RCC_CR_HSEON_Msk;
			/* Wait until HSE is stopped (HSERDY = 0) or the timeout elapses */
			timeout = RCC_HSE_TIMEOUT_VALUE;
			while((RCC->CR & RCC_CR_HSERDY_Msk) != 0U){
				if(timeout-- == 0U){
					return HAL_TIMEOUT;
				}
			}
		}
	}else{
		/* Do Nothing */
	}

	/*------------------------------- HSI Configuration ------------------------*/
	if(((RCC_OscInitStruct->OscillatorType) & RCC_OSCILLATORTYPE_HSI) == RCC_OSCILLATORTYPE_HSI){
		if(RCC_OscInitStruct->HSIState == RCC_HSI_ON){
			/* Turn the HSI oscillator ON */
			RCC->CR |= RCC_CR_HSION_Msk;
			/* Wait until HSI is ready (HSIRDY = 1) or the timeout elapses */
			timeout = RCC_HSI_TIMEOUT_VALUE;
			while((RCC->CR & RCC_CR_HSIRDY_Msk) == 0U){
				if(timeout-- == 0U){
					return HAL_TIMEOUT;
				}
			}
		}else{
			/* Turn the HSI oscillator OFF */
			RCC->CR &= ~RCC_CR_HSION_Msk;
			/* Wait until HSI is stopped (HSIRDY = 0) or the timeout elapses */
			timeout = RCC_HSI_TIMEOUT_VALUE;
			while((RCC->CR & RCC_CR_HSIRDY_Msk) != 0U){
				if(timeout-- == 0U){
					return HAL_TIMEOUT;
				}
			}
		}
	}else{
		/* Do Nothing */
	}

	/*------------------------------- LSE Configuration ------------------------*/
	/* Note: write access to BDCR must already be enabled (PWR->CR DBP bit) */
	if(((RCC_OscInitStruct->OscillatorType) & RCC_OSCILLATORTYPE_LSE) == RCC_OSCILLATORTYPE_LSE){
		if(RCC_OscInitStruct->LSEState == RCC_LSE_ON){
			/* Turn the LSE oscillator ON */
			RCC->BDCR |= RCC_BDCR_LSEON_Msk;
			/* Wait until LSE is ready (LSERDY = 1) or the timeout elapses */
			timeout = RCC_LSE_TIMEOUT_VALUE;
			while((RCC->BDCR & RCC_BDCR_LSERDY_Msk) == 0U){
				if(timeout-- == 0U){
					return HAL_TIMEOUT;
				}
			}
		}else{
			/* Turn the LSE oscillator OFF */
			RCC->BDCR &= ~RCC_BDCR_LSEON_Msk;
			/* Wait until LSE is stopped (LSERDY = 0) or the timeout elapses */
			timeout = RCC_LSE_TIMEOUT_VALUE;
			while((RCC->BDCR & RCC_BDCR_LSERDY_Msk) != 0U){
				if(timeout-- == 0U){
					return HAL_TIMEOUT;
				}
			}
		}
	}else{
		/* Do Nothing */
	}

	/*------------------------------- LSI Configuration ------------------------*/
	if(((RCC_OscInitStruct->OscillatorType) & RCC_OSCILLATORTYPE_LSI) == RCC_OSCILLATORTYPE_LSI){
		if(RCC_OscInitStruct->LSIState == RCC_LSI_ON){
			/* Turn the LSI oscillator ON */
			RCC->CSR |= RCC_CSR_LSION_Msk;
			/* Wait until LSI is ready (LSIRDY = 1) or the timeout elapses */
			timeout = RCC_LSI_TIMEOUT_VALUE;
			while((RCC->CSR & RCC_CSR_LSIRDY_Msk) == 0U){
				if(timeout-- == 0U){
					return HAL_TIMEOUT;
				}
			}
		}else{
			/* Turn the LSI oscillator OFF */
			RCC->CSR &= ~RCC_CSR_LSION_Msk;
			/* Wait until LSI is stopped (LSIRDY = 0) or the timeout elapses */
			timeout = RCC_LSI_TIMEOUT_VALUE;
			while((RCC->CSR & RCC_CSR_LSIRDY_Msk) != 0U){
				if(timeout-- == 0U){
					return HAL_TIMEOUT;
				}
			}
		}
	}else{
		/* Do Nothing */
	}

	/*------------------------------- PLL Configuration ------------------------*/
	if(RCC_OscInitStruct->PLL.PLLState != RCC_PLL_NONE){
		/* The PLL cannot be reconfigured while it is driving the system clock */
		if((RCC->CFGR & RCC_CFGR_SWS_Msk) == RCC_CFGR_SWS_PLL){
			return HAL_ERROR;
		}else{
			/* Disable the PLL first (its source / multiplier are only writable while it is off) */
			RCC->CR &= ~RCC_CR_PLLON_Msk;
			/* Wait until the PLL is fully stopped (PLLRDY = 0) or the timeout elapses */
			timeout = RCC_PLL_TIMEOUT_VALUE;
			while((RCC->CR & RCC_CR_PLLRDY_Msk) != 0U){
				if(timeout-- == 0U){
					return HAL_TIMEOUT;
				}
			}

			if(RCC_OscInitStruct->PLL.PLLState == RCC_PLL_ON){
				/* Program the PLL input source (PLLSRC + PLLXTPRE) and the multiplier (PLLMUL) */
				RCC->CFGR = (RCC->CFGR & ~(RCC_CFGR_PLLSRC_Msk | RCC_CFGR_PLLXTPRE_Msk | RCC_CFGR_PLLMUL_Msk))
							| (RCC_OscInitStruct->PLL.PLLSource)
							| (RCC_OscInitStruct->PLL.PLLMUL);
				/* Enable the PLL */
				RCC->CR |= RCC_CR_PLLON_Msk;
				/* Wait until the PLL is locked (PLLRDY = 1) or the timeout elapses */
				timeout = RCC_PLL_TIMEOUT_VALUE;
				while((RCC->CR & RCC_CR_PLLRDY_Msk) == 0U){
					if(timeout-- == 0U){
						return HAL_TIMEOUT;
					}
				}
			}else{
				/* PLL requested OFF - already stopped above, nothing else to do */
			}
		}
	}else{
		/* Do Nothing */
	}

	return HAL_OK;
}


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
HAL_StatusType HAL_RCC_ClockConfig(RCC_ClkInitType *RCC_ClkInitStruct, uint32_t FLatency){

	uint32_t timeout = 0U;

	/* Check the structure pointer */
	if(RCC_ClkInitStruct == NULL_PTR){
		return HAL_ERROR;
	}

	/*------------------------- Flash latency (raise it BEFORE increasing the clock) -------*/
	if(FLatency > (FLASH->ACR & FLASH_ACR_LATENCY_Msk)){
		FLASH_SetLatency(FLatency);
	}else{
		/* Do Nothing (a decrease is applied after the switch, below) */
	}

	/*------------------------- HCLK Configuration (AHB prescaler) -------------------------*/
	if(((RCC_ClkInitStruct->ClockType) & RCC_CLOCKTYPE_HCLK) == RCC_CLOCKTYPE_HCLK){
		/* Clear the HPRE field, then write the requested AHB divider */
		RCC->CFGR = (RCC->CFGR & ~RCC_CFGR_HPRE_Msk) | (RCC_ClkInitStruct->AHBCLKDivider);
	}else{
		/* Do Nothing */
	}

	/*------------------------- SYSCLK Source Configuration -------------------------------*/
	if(((RCC_ClkInitStruct->ClockType) & RCC_CLOCKTYPE_SYSCLK) == RCC_CLOCKTYPE_SYSCLK){
		/* Clear the SW field, then write the requested system clock source */
		RCC->CFGR = (RCC->CFGR & ~RCC_CFGR_SW_Msk) | (RCC_ClkInitStruct->SYSCLKSource);

		/* Wait until the hardware confirms the switch: SWS (bits [3:2]) must equal SW (bits [1:0]) */
		timeout = RCC_SYSCLK_TIMEOUT_VALUE;
		while((RCC->CFGR & RCC_CFGR_SWS_Msk) != ((RCC_ClkInitStruct->SYSCLKSource) << RCC_CFGR_SWS_Pos)){
			if(timeout-- == 0U){
				return HAL_TIMEOUT;
			}
		}
	}else{
		/* Do Nothing */
	}

	/*------------------------- Flash latency (lower it AFTER decreasing the clock) --------*/
	if(FLatency < (FLASH->ACR & FLASH_ACR_LATENCY_Msk)){
		FLASH_SetLatency(FLatency);
	}else{
		/* Do Nothing (an increase was already applied before the switch, above) */
	}

	/*------------------------- PCLK1 Configuration (APB1 prescaler) ----------------------*/
	if(((RCC_ClkInitStruct->ClockType) & RCC_CLOCKTYPE_PCLK1) == RCC_CLOCKTYPE_PCLK1){
		/* Clear the PPRE1 field, then write the requested APB1 divider */
		RCC->CFGR = (RCC->CFGR & ~RCC_CFGR_PPRE1_Msk) | (RCC_ClkInitStruct->APB1CLKDivider);
	}else{
		/* Do Nothing */
	}

	/*------------------------- PCLK2 Configuration (APB2 prescaler) ----------------------*/
	if(((RCC_ClkInitStruct->ClockType) & RCC_CLOCKTYPE_PCLK2) == RCC_CLOCKTYPE_PCLK2){
		/* Clear the PPRE2 field, then write the requested APB2 divider */
		RCC->CFGR = (RCC->CFGR & ~RCC_CFGR_PPRE2_Msk) | (RCC_ClkInitStruct->APB2CLKDivider);
	}else{
		/* Do Nothing */
	}

	return HAL_OK;
}

