/*
 * CortexM4_Core_NVIC.c
 *
 *  Created on: May 18, 2026
 *      Author: Adham
 */

#include "NVIC/CortexM3_Core_NVIC.h"


/**
 * brief	Enalbe interrupt
 * details	Enables a device specific interrupt in the NVIC interrupt controller
 * param	IRQn device specific interrupt number
 * note		IRQn can't be negative
 */
void NVIC_EnableIRQ(IRQn_Type IRQn){
	if((uint32_t)IRQn >= 0){
		// from the IRQn we need to get the register number (0->7) and the bit number (0->31)
		NVIC->ISER[((uint32_t)IRQn) >> 5] = (1UL << ((uint32_t)IRQn & 0x1F));
	}else{
		/* Do Nothing */
	}
}

/**
 * brief	Disable interrupt
 * details	Disable a device specific interrupt in the NVIC interrupt controller
 * param	IRQn device specific interrupt number
 * note		IRQn can't be negative
 */
void NVIC_DisableIRQ(IRQn_Type IRQn){
	if((uint32_t)IRQn >= 0){
		// from the IRQn we need to get the register number (0->7) and the bit number (0->31)
		NVIC->ICER[((uint32_t)IRQn) >> 5] = (1UL << ((uint32_t)IRQn & 0x1F));
	}else{
		/* Do Nothing */
	}
}

/**
 * brief	Set interrupt pending
 * details	Set pending bit for a device specific interrupt in the NVIC interrupt controller
 * param	IRQn device specific interrupt number
 * note		IRQn can't be negative
 */
void NVIC_SetPendingIRQ(IRQn_Type IRQn){
	if((uint32_t)IRQn >= 0){
		// from the IRQn we need to get the register number (0->7) and the bit number (0->31)
		NVIC->ISPR[((uint32_t)IRQn) >> 5] = (1UL << ((uint32_t)IRQn & 0x1F));
	}else{
		/* Do Nothing */
	}
}

/**
 * brief	Clear interrupt pending
 * details	Clear pending bit for a device specific interrupt in the NVIC interrupt controller
 * param	IRQn device specific interrupt number
 * note		IRQn can't be negative
 */
void NVIC_ClearPendingIRQ(IRQn_Type IRQn){
	if((uint32_t)IRQn >= 0){
		// from the IRQn we need to get the register number (0->7) and the bit number (0->31)
		NVIC->ICPR[((uint32_t)IRQn) >> 5] = (1UL << ((uint32_t)IRQn & 0x1F));
	}else{
		/* Do Nothing */
	}
}

/**
 * brief	Get active interrupt
 * details	Reads the active register in the NVIC and returns the active bit for the device specific interrupt in the NVIC interrupt contoller
 * param	IRQn device specific interrupt number
 * return	0 Interrupt status is not active
 * return	1 Interrupt status is active
 * note		IRQn can't be negative
 */
uint32_t NVIC_GetActive(IRQn_Type IRQn){

	if((uint32_t)IRQn >= 0){
		// from the IRQn we need to get the register number (0->7) and the bit number (0->31)
		// we use & with the IRQn bit to mask out other bits and then see if it's not equal to zero then the interrupt is active
		return ((NVIC->IABR[((uint32_t)IRQn) >> 5] & (1UL << (((uint32_t)IRQn) & 0x1F))) != (uint32_t)0) ? 1UL : 0UL ;
	}else{
		return 0;
	}
}

/**
 * brief	Set interrupt priority
 * details	Sets the priority of a device specific interrupt or a processor exception.
 * 			The interrupt number can be positive to specify a device specific interrupt or negative to specify processor exception
 * param	IRQn interrupt number
 * param	Priority to set
 */
void NVIC_SetPriority(IRQn_Type IRQn, uint32_t priority){
	if((uint32_t)IRQn >= 0){
		NVIC->IP[(uint32_t)IRQn] = (uint8_t)((priority << (8U - NVIC_PRIO_BITS)) & (uint32_t)0xFF);
	}else{
		/* Do Nothing */
	}
}

/**
 * brief	Get interrupt priority
 * details	Reads the priority of a device specific interrupt or a processor exception.
 * 			The interrupt number can be positive to specify a device specific interrupt or negative to specify processor exception
 * param	IRQn interrupt number
 * return	Interrupt priority
 */
uint32_t NVIC_GetPriority(IRQn_Type IRQn){
	if((uint32_t)IRQn >= 0){
		return ((NVIC->IP[(uint32_t)IRQn]) >> (8U - NVIC_PRIO_BITS));
	}else{
		return 0;
	}
}
