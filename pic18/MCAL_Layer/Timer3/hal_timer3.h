/* 
 * File:   hal_timer3.h
 * Author: Adham
 *
 * Created on August 27, 2024, 4:52 PM
 */

#ifndef HAL_TIMER3_H
#define	HAL_TIMER3_H

/***************************************Includes***************************************/

#include "../../MCAL_Layer/GPIO/hal_gpio.h"
#include "../../MCAL_Layer/mcal_std_types.h"
#include "pic18f4620.h"
#include "../../MCAL_Layer/Interrupt/mcal_internal_interrupt.h"

/***********************************Macro Declarations**********************************/

#define TIMER3_TIMER_MODE               0
#define TIMER3_COUNTER_MODE             1

#define TIMER3_ASYNC_COUNTER_MODE       1
#define TIMER3_SYNC_COUNTER_MODE        0

#define TIMER3_PRESCALER_DIV_BY_1       0
#define TIMER3_PRESCALER_DIV_BY_2       1
#define TIMER3_PRESCALER_DIV_BY_4       2
#define TIMER3_PRESCALER_DIV_BY_8       3

#define TIMER3_RW_REG_8BIT_MODE         0
#define TIMER3_RW_REG_16BIT_MODE        1

/******************************Macro Function Declarations******************************/

/* Enable Timer3 module */
#define TIMER3_MODULE_ENABLE() (T3CONbits.TMR3ON = 1)
/* Disable Timer3 module */
#define TIMER3_MODULE_DISABLE() (T3CONbits.TMR3ON = 0)

/* Initialize Timer3 in timer mode */
#define TIMER3_TIMER_MODE_ENABLE() (T3CONbits.TMR3CS = 0)
/* Initialize Timer3 in counter mode */
#define TIMER3_COUNTER_MODE_ENABLE() (T3CONbits.TMR3CS = 1)

/* Timer3 External Clock Input Synchronization */
#define TIMER3_ASYNC_COUNTER_MODE_ENABLE() (T3CONbits.T3SYNC = 1)
#define TIMER3_SYNC_COUNTER_MODE_ENABLE() (T3CONbits.T3SYNC = 0)

/* Timer3 Input Clock Pre-Scaler */
#define TIMER3_PRESCALER_SELECT(_PRESCALER_) (T3CONbits.T3CKPS = _PRESCALER_)

/* 16-Bit Read/Write Mode */
#define TIMER3_RW_REG_8BIT_MODE_ENABLE() (T3CONbits.RD16 = 0)
#define TIMER3_RW_REG_16BIT_MODE_ENABLE() (T3CONbits.RD16 = 1)

/***********************************Datatype Declarations*******************************/

typedef struct{
#if TIMER3_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
    void(* TMR3_InterruptHandler)(void);
    interrupt_priority_cfg priority;
#endif
    uint16 timer3_preload_value;
    uint8 timer3_prescaler : 2;
    uint8 timer3_mode : 1;
    uint8 timer3_counter_mode : 1;
    uint8 timer1_reg_wr_mode : 1;
    uint8 reserved : 3;
}timer3_t;

/***********************************Function Declarations*******************************/

/**
 * @brief Initialize Timer3
 * @param timer3 pointer to the timer3 module configurations
 * @return status of the function
 *          (E_OK) : the function done successfully
 *          (E_NOT_OK) : the function has issue to perform this action
 */
Std_ReturnType Timer3_Init(const timer3_t *timer3);
/**
 * @brief DeInitialize Timer3
 * @param timer3 pointer to the timer3 module configurations
 * @return status of the function
 *          (E_OK) : the function done successfully
 *          (E_NOT_OK) : the function has issue to perform this action
 */
Std_ReturnType Timer3_DeInit(const timer3_t *timer3);
/**
 * @brief Write value in Timer3
 * @param timer3 pointer to the timer3 module configurations
 * @param value
 * @return status of the function
 *          (E_OK) : the function done successfully
 *          (E_NOT_OK) : the function has issue to perform this action
 */
Std_ReturnType Timer3_Write_value(const timer3_t *timer3 , uint16 value);
/**
 * @brief Read value from Timer3
 * @param timer3 pointer to the timer1 module configurations
 * @param value
 * @return status of the function
 *          (E_OK) : the function done successfully
 *          (E_NOT_OK) : the function has issue to perform this action
 */
Std_ReturnType Timer3_Read_value(const timer3_t *timer3 , uint16 *value);

#endif	/* HAL_TIMER3_H */

