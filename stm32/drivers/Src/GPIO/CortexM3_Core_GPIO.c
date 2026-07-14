/*
 * CortexM3_Core_GPIO.c
 *
 *  Created on: Jul 12, 2026
 *      Author: Adham
 */

#include "GPIO/CortexM3_Core_GPIO.h"


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
HAL_StatusType GPIO_Init(GPIO_Type *GPIOx, GPIO_InitType *GPIO_InitStruct){

	uint32_t pinPos    = 0U;
	uint32_t pinMask   = 0U;
	uint32_t cnfMode   = 0U;
	uint32_t shift     = 0U;
	volatile uint32_t *pConfigReg = NULL_PTR;

	/* Check the pointers */
	if((GPIOx == NULL_PTR) || (GPIO_InitStruct == NULL_PTR)){
		return HAL_ERROR;
	}else{
		/* Do Nothing */
	}

	for(pinPos = 0U; pinPos < 16U; pinPos++){

		pinMask = (0x1UL << pinPos);

		if((GPIO_InitStruct->Pin & pinMask) == pinMask){

			/* Build the 4-bit CNF:MODE field for this pin */
			switch(GPIO_InitStruct->Mode){

				case GPIO_MODE_ANALOG:
					cnfMode = 0x0U;								// CNF = 00, MODE = 00
					break;

				case GPIO_MODE_INPUT:
					if(GPIO_InitStruct->Pull == GPIO_NOPULL){
						cnfMode = 0x4U;							// CNF = 01 (floating), MODE = 00
					}else{
						cnfMode = 0x8U;							// CNF = 10 (pull-up/down), MODE = 00
					}
					break;

				case GPIO_MODE_OUTPUT_PP:
					cnfMode = (0x0U << 2U) | (GPIO_InitStruct->Speed & 0x3U);		// CNF = 00
					break;

				case GPIO_MODE_OUTPUT_OD:
					cnfMode = (0x1U << 2U) | (GPIO_InitStruct->Speed & 0x3U);		// CNF = 01
					break;

				case GPIO_MODE_AF_PP:
					cnfMode = (0x2U << 2U) | (GPIO_InitStruct->Speed & 0x3U);		// CNF = 10
					break;

				case GPIO_MODE_AF_OD:
					cnfMode = (0x3U << 2U) | (GPIO_InitStruct->Speed & 0x3U);		// CNF = 11
					break;

				default:
					return HAL_ERROR;

			}

			/* Pins 0-7 live in CRL, pins 8-15 live in CRH; each pin occupies 4 bits within its register */
			if(pinPos < GPIO_CRL_PIN_COUNT){
				pConfigReg = &(GPIOx->CRL);
				shift      = pinPos * GPIO_CR_FIELD_WIDTH;
			}else{
				pConfigReg = &(GPIOx->CRH);
				shift      = (pinPos - GPIO_CRL_PIN_COUNT) * GPIO_CR_FIELD_WIDTH;
			}

			/* Clear this pin's 4-bit field, then write the new CNF:MODE value */
			*pConfigReg = (*pConfigReg & ~(GPIO_CR_FIELD_MASK << shift)) | (cnfMode << shift);

			/* For pulled inputs, ODR selects the pull direction: 1 = pull-up, 0 = pull-down (BSRR/BRR = atomic) */
			if((GPIO_InitStruct->Mode == GPIO_MODE_INPUT) && (GPIO_InitStruct->Pull != GPIO_NOPULL)){
				if(GPIO_InitStruct->Pull == GPIO_PULLUP){
					GPIOx->BSRR = pinMask;
				}else{
					GPIOx->BRR = pinMask;
				}
			}else{
				/* Do Nothing */
			}

		}else{
			/* Do Nothing */
		}
	}

	return HAL_OK;
}

/**
 * brief	Writes a logic state to an output pin
 * details	Uses the BSRR register, which sets or clears the requested pin atomically (single register write,
 * 			no read-modify-write), so it is safe to use from both main-line code and interrupts
 * param	GPIOx pointer to the GPIO port (GPIOA .. GPIOD)
 * param	Pin the pin to write. A value of @ref GPIO_PIN_x (single pin)
 * param	PinState the state to write. GPIO_PIN_SET or GPIO_PIN_RESET
 */
void GPIO_WritePin(GPIO_Type *GPIOx, uint32_t Pin, uint32_t PinState){

	if(PinState == GPIO_PIN_SET){
		GPIOx->BSRR = Pin;				// Lower 16 bits of BSRR set the corresponding ODR bit
	}else{
		GPIOx->BRR = Pin;				// BRR clears the corresponding ODR bit
	}
}

/**
 * brief	Reads the current logic state of an input (or output) pin
 * param	GPIOx pointer to the GPIO port (GPIOA .. GPIOD)
 * param	Pin the pin to read. A value of @ref GPIO_PIN_x (single pin)
 * return	GPIO_PIN_SET   the pin reads high
 * return	GPIO_PIN_RESET the pin reads low
 */
uint32_t GPIO_ReadPin(GPIO_Type *GPIOx, uint32_t Pin){

	return ((GPIOx->IDR & Pin) != 0U) ? GPIO_PIN_SET : GPIO_PIN_RESET;
}

/**
 * brief	Toggles the current output state of a pin
 * details	Performs a read-modify-write on ODR; if an ISR can also write the same pin's ODR bit, prefer
 * 			GPIO_WritePin with the ISR-observed state instead to avoid a race
 * param	GPIOx pointer to the GPIO port (GPIOA .. GPIOD)
 * param	Pin the pin to toggle. A value of @ref GPIO_PIN_x (single pin, or several OR'd together)
 */
void GPIO_TogglePin(GPIO_Type *GPIOx, uint32_t Pin){

	GPIOx->ODR ^= Pin;
}
