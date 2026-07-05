/* 
 * File:   hal_timer2.h
 * Author: Adham
 *
 * Created on August 27, 2024, 4:27 PM
 */

#ifndef HAL_TIMER2_H
#define	HAL_TIMER2_H

/***************************************Includes***************************************/

#include "pic18f4620.h"
#include "../../MCAL_Layer/GPIO/hal_gpio.h"
#include "../../MCAL_Layer/mcal_std_types.h"
#include "../../MCAL_Layer/Interrupt/mcal_internal_interrupt.h"

/***********************************Macro Declarations**********************************/

/* Timer2 Input Post-Scaler*/
#define TIMER2_POSTSCALER_DIV_BY_1       0
#define TIMER2_POSTSCALER_DIV_BY_2       1
#define TIMER2_POSTSCALER_DIV_BY_3       2
#define TIMER2_POSTSCALER_DIV_BY_4       3
#define TIMER2_POSTSCALER_DIV_BY_5       4
#define TIMER2_POSTSCALER_DIV_BY_6       5
#define TIMER2_POSTSCALER_DIV_BY_7       6
#define TIMER2_POSTSCALER_DIV_BY_8       7
#define TIMER2_POSTSCALER_DIV_BY_9       8
#define TIMER2_POSTSCALER_DIV_BY_10      9
#define TIMER2_POSTSCALER_DIV_BY_11      10
#define TIMER2_POSTSCALER_DIV_BY_12      11
#define TIMER2_POSTSCALER_DIV_BY_13      12
#define TIMER2_POSTSCALER_DIV_BY_14      13
#define TIMER2_POSTSCALER_DIV_BY_15      14
#define TIMER2_POSTSCALER_DIV_BY_16      15

/* Timer2 Input Pre-Scaler*/
#define TIMER2_PRESCALER_DIV_BY_1       0
#define TIMER2_PRESCALER_DIV_BY_4       1
#define TIMER2_PRESCALER_DIV_BY_16      2

/******************************Macro Function Declarations******************************/

/* Enable Timer2 module */
#define TIMER2_MODULE_ENABLE() (T2CONbits.TMR2ON = 1)
/* Disable Timer2 module */
#define TIMER2_MODULE_DISABLE() (T2CONbits.TMR2ON = 0)

/* Timer2 Input Clock Pre-Scaler */
#define TIMER2_PRESCALER_SELECT(_PRESCALER_) (T2CONbits.T2CKPS = _PRESCALER_)

/* Timer2 Input Clock Post-Scaler */
#define TIMER2_POSTSCALER_SELECT(_POSTSCALER_) (T2CONbits.TOUTPS = _POSTSCALER_)

/***********************************Datatype Declarations*******************************/

typedef struct{
#if TIMER2_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
    void(* TMR2_InterruptHandler)(void);
    interrupt_priority_cfg priority;
#endif
    uint8 timer2_preload_value;
    uint8 timer2_postscaler : 4;
    uint8 timer2_prescaler : 2;
    uint8 reserved : 2;
}timer2_t;

/***********************************Function Declarations*******************************/

/**
 * @brief Initialize Timer2
 * @param timer2 pointer to the timer2 module configurations
 * @return status of the function
 *          (E_OK) : the function done successfully
 *          (E_NOT_OK) : the function has issue to perform this action
 */
Std_ReturnType Timer2_Init(const timer2_t *timer2);
/**
 * @brief DeInitialize Timer2
 * @param timer2 pointer to the timer2 module configurations
 * @return status of the function
 *          (E_OK) : the function done successfully
 *          (E_NOT_OK) : the function has issue to perform this action
 */
Std_ReturnType Timer2_DeInit(const timer2_t *timer2);
/**
 * @brief Write value in Timer2
 * @param timer1 pointer to the timer2 module configurations
 * @param value
 * @return status of the function
 *          (E_OK) : the function done successfully
 *          (E_NOT_OK) : the function has issue to perform this action
 */
Std_ReturnType Timer2_Write_value(const timer2_t *timer2 , uint8 value);
/**
 * @brief Read value from Timer2
 * @param timer2 pointer to the timer2 module configurations
 * @param value
 * @return status of the function
 *          (E_OK) : the function done successfully
 *          (E_NOT_OK) : the function has issue to perform this action
 */
Std_ReturnType Timer2_Read_value(const timer2_t *timer2 , uint8 *value);

#endif	/* HAL_TIMER2_H */

