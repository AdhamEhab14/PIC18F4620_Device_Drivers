/*
 * CortexM3_Core_SCB.c
 *
 *  Created on: May 18, 2026
 *      Author: Adham
 */

#include "CortexM3_Core_SCB.h"

/**
 * brief	Set Priority Grouping
 * details	Sets the priority grouping field using the required unlock sequence.
 * 			The parameter PriorityGroup is assigned to the field SCB->AIRCR [10:8] (only values from 0:7 are used)
 * param	Priority grouping field
 */
void SCB_SetPriorityGrouping(uint32_t PriorityGrouping){
	uint32_t Register_Value = 0;
	uint32_t PriorityGrouping_Temp = ((uint32_t)PriorityGrouping & (uint32_t)0x07);

	Register_Value = SCB->AIRCR;												// read the SCB-> AIRCR register before writing anything
	Register_Value &= ~(SCB_AIRCR_PRIGROUP_MASK | SCB_AIRCR_VECTKEYSTAT_MASK);	// clear the bits to be modified using masks

	Register_Value = (Register_Value | ((uint32_t)0x05FA << SCB_AIRCR_VECTKEYSTAT_POS) |
					 (PriorityGrouping_Temp << SCB_AIRCR_PRIGROUP_POS));

	SCB->AIRCR = Register_Value;
}

/**
 * brief	Get Priority Grouping
 * details	Reads the priority grouping field from the SCB register.
 * return	Priority Grouping Filed (SCB->AIRCR [10:8])
 */
uint32_t SCB_GetPriorityGrouping(void){
	return (((uint32_t)SCB->AIRCR & (uint32_t)SCB_AIRCR_PRIGROUP_MASK) >> SCB_AIRCR_PRIGROUP_POS);
}


