/* 
 * File:   mcal_interrupt_manager.h
 * Author: Adham
 *
 * Created on August 7, 2024, 9:54 PM
 */

#ifndef MCAL_INTERRUPT_MANAGER_H
#define	MCAL_INTERRUPT_MANAGER_H

/***************************************Includes***************************************/

#include "mcal_interrupt_config.h"

/***********************************Macro Declarations**********************************/

/******************************Macro Function Declarations******************************/

/***********************************Datatype Declarations*******************************/

void INT0_ISR(void);
void INT1_ISR(void);
void INT2_ISR(void);
void RB4_ISR(uint8 RB4_Source);
void RB5_ISR(uint8 RB5_Source);
void RB6_ISR(uint8 RB6_Source);
void RB7_ISR(uint8 RB7_Source);
void ADC_ISR(void);
void TMR0_ISR(void);
void TMR1_ISR(void);
void TMR2_ISR(void);
void TMR3_ISR(void);
void CCP1_ISR(void);
void CCP2_ISR(void);
void EUSART_Tx_ISR(void);
void EUSART_Rx_ISR(void);
void MSSP_SPI_ISR(void);
void MSSP_I2C_ISR(void);
void MSSP_I2C_BC_ISR(void);

/***********************************Function Declarations*******************************/

#endif	/* MCAL_INTERRUPT_MANAGER_H */

