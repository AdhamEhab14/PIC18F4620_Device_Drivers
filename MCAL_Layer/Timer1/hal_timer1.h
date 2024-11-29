/* 
 * File:   hal_timer1.h
 * Author: Adham
 *
 * Created on August 26, 2024, 5:06 PM
 */

#ifndef HAL_TIMER1_H
#define	HAL_TIMER1_H

/***************************************Includes***************************************/

#include "../../MCAL_Layer/GPIO/hal_gpio.h"
#include "../../MCAL_Layer/mcal_std_types.h"
#include "pic18f4620.h"
#include "../../MCAL_Layer/Interrupt/mcal_internal_interrupt.h"

/***********************************Macro Declarations**********************************/

#define TIMER1_TIMER_MODE               0
#define TIMER1_COUNTER_MODE             1

#define TIMER1_ASYNC_COUNTER_MODE       1
#define TIMER1_SYNC_COUNTER_MODE        0

#define TIMER1_OSCILLATOR_ENABLE        1
#define TIMER1_OSCILLATOR_DISABLE       0

#define TIMER1_PRESCALER_DIV_BY_1       0
#define TIMER1_PRESCALER_DIV_BY_2       1
#define TIMER1_PRESCALER_DIV_BY_4       2
#define TIMER1_PRESCALER_DIV_BY_8       3

#define TIMER1_RW_REG_8BIT_MODE         0
#define TIMER1_RW_REG_16BIT_MODE        1

/******************************Macro Function Declarations******************************/

/* Enable Timer1 module */
#define TIMER1_MODULE_ENABLE() (T1CONbits.TMR1ON = 1)
/* Disable Timer1 module */
#define TIMER1_MODULE_DISABLE() (T1CONbits.TMR1ON = 0)

/* Initialize Timer1 in timer mode */
#define TIMER1_TIMER_MODE_ENABLE() (T1CONbits.TMR1CS = 0)
/* Initialize Timer1 in counter mode */
#define TIMER1_COUNTER_MODE_ENABLE() (T1CONbits.TMR1CS = 1)

/* Timer1 External Clock Input Synchronization */
#define TIMER1_ASYNC_COUNTER_MODE_ENABLE() (T1CONbits.T1SYNC = 1)
#define TIMER1_SYNC_COUNTER_MODE_ENABLE() (T1CONbits.T1SYNC = 0)

/* Enable Timer1 Oscillator */
#define TIMER1_OSC_HW_ENABLE() (T1CONbits.T1OSCEN = 1)
/* Disable Timer1 Oscillator */
#define TIMER1_OSC_HW_DISABLE() (T1CONbits.T1OSCEN = 0)

/* Timer1 Input Clock Pre-Scaler */
#define TIMER1_PRESCALER_SELECT(_PRESCALER_) (T1CONbits.T1CKPS = _PRESCALER_)

/* Timer1 System Clock Status */
#define TIMER1_SYSTEM_CLK_STATUS() (T1CONbits.T1RUN)

/* 16-Bit Read/Write Mode */
#define TIMER1_RW_REG_8BIT_MODE_ENABLE() (T1CONbits.RD16 = 0)
#define TIMER1_RW_REG_16BIT_MODE_ENABLE() (T1CONbits.RD16 = 1)

/***********************************Datatype Declarations*******************************/

typedef struct{
#if TIMER1_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
    void(* TMR1_InterruptHandler)(void);
    interrupt_priority_cfg priority;
#endif
    uint16 timer1_preload_value;
    uint8 prescaler_value : 2;
    uint8 timer1_mode : 1;
    uint8 timer1_counter_mode : 1;
    uint8 timer1_osc_cfg : 1;
    uint8 timer1_reg_wr_mode : 1;
    uint8 : 2;
    
}timer1_t;

/***********************************Function Declarations*******************************/

/**
 * @brief Initialize Timer1
 * @param timer1 pointer to the timer1 module configurations
 * @return status of the function
 *          (E_OK) : the function done successfully
 *          (E_NOT_OK) : the function has issue to perform this action
 */
Std_ReturnType Timer1_Init(const timer1_t *timer1);
/**
 * @brief DeInitialize Timer1
 * @param timer1 pointer to the timer1 module configurations
 * @return status of the function
 *          (E_OK) : the function done successfully
 *          (E_NOT_OK) : the function has issue to perform this action
 */
Std_ReturnType Timer1_DeInit(const timer1_t *timer1);
/**
 * @brief Write value in Timer1
 * @param timer1 pointer to the timer1 module configurations
 * @param value
 * @return status of the function
 *          (E_OK) : the function done successfully
 *          (E_NOT_OK) : the function has issue to perform this action
 */
Std_ReturnType Timer1_Write_value(const timer1_t *timer1 , uint16 value);
/**
 * @brief Read value from Timer1
 * @param timer1 pointer to the timer1 module configurations
 * @param value
 * @return status of the function
 *          (E_OK) : the function done successfully
 *          (E_NOT_OK) : the function has issue to perform this action
 */
Std_ReturnType Timer1_Read_value(const timer1_t *timer1 , uint16 *value);

#endif	/* HAL_TIMER1_H */



