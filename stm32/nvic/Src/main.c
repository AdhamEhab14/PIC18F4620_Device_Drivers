/**
 ******************************************************************************
 * @file           : main.c
 * @author         : Adham Ehab
 * @brief          : Main program body
 ******************************************************************************
 */

#include "CortexM3_Core_NVIC.h"
#include "CortexM3_Core_SCB.h"

static volatile uint8_t USART3_IRQHandler_flag = 0;
static volatile uint8_t EXTI15_10_IRQHandler_flag = 0;

int main(void)
{

	SCB_SetPriorityGrouping(SCB_PRIORITY_GROUP_2);

	NVIC_EnableIRQ(USART3_IRQn);		// 4
	NVIC_EnableIRQ(EXTI15_10_IRQn);		// 0

	NVIC_SetPriority(USART3_IRQn, 4);
	NVIC_SetPriority(EXTI15_10_IRQn, 0);

	NVIC_SetPendingIRQ(USART3_IRQn);


 	while(1){

	}
}


void USART3_IRQHandler(void){
	USART3_IRQHandler_flag = 1;

	NVIC_SetPendingIRQ(EXTI15_10_IRQn);

	USART3_IRQHandler_flag = 2;
}

void EXTI15_10_IRQHandler(void){
	EXTI15_10_IRQHandler_flag = 1;
}

