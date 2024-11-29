/* 
 * File:   hal_timer2.c
 * Author: Adham
 *
 * Created on August 27, 2024, 4:27 PM
 */

#include "hal_timer2.h"

#if TIMER2_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
static void(* TMR2_InterruptHandler)(void) = NULL;
#endif

static volatile uint8 timer2_preload = 0;

/**
 * @brief Initialize Timer2
 * @param timer2 pointer to the timer2 module configurations
 * @return status of the function
 *          (E_OK) : the function done successfully
 *          (E_NOT_OK) : the function has issue to perform this action
 */
Std_ReturnType Timer2_Init(const timer2_t *timer2){
    Std_ReturnType retVal = E_NOT_OK;
    if(NULL == timer2){
        retVal = E_NOT_OK;
    }
    else{
        /* Disable Timer2 */
        TIMER2_MODULE_DISABLE();
        /* Configure Pre-scaler */
        TIMER2_PRESCALER_SELECT(timer2->timer2_prescaler);
        /* Configure Post-scaler */
        TIMER2_POSTSCALER_SELECT(timer2->timer2_postscaler);
        /* Configure Pre-Loaded Value */
        TMR2 = timer2->timer2_preload_value;
        timer2_preload = timer2->timer2_preload_value;
        /* Configure the interrupts */
#if TIMER2_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
        TIMER2_InterruptEnable();    /* Enable Timer2 module interrupt */
        TIMER2_InterruptFlagClear(); /* Clear flag */
        TMR2_InterruptHandler = timer2->TMR2_InterruptHandler;
        /* Check priority & Enable Timer2 module interrupt */
        #if INTERRUPT_PRIORITY_LEVELS_ENABLE==INTERRUPT_FEATURE_ENABLE
        INTERRUPT_PriorityLevelEnable();
        if(INTERRUPT_HIGH_PRIORITY == timer2->priority){
            INTERRUPT_GlobalInterruptHighEnable();
            TIMER2_HighPrioritySet();
        }
        else if(INTERRUPT_LOW_PRIORITY == timer2->priority){
            INTERRUPT_GlobalInterruptLowEnable();
            TIMER2_LowPrioritySet();
        }
        #else
        INTERRUPT_GlobalInterruptEnable();
        INTERRUPT_PeripheralInterruptEnable();
        #endif
#endif
        /* Enable Timer0*/
        TIMER2_MODULE_ENABLE();
        retVal = E_OK;
    }
    return retVal;
}

/**
 * @brief DeInitialize Timer2
 * @param timer2 pointer to the timer2 module configurations
 * @return status of the function
 *          (E_OK) : the function done successfully
 *          (E_NOT_OK) : the function has issue to perform this action
 */
Std_ReturnType Timer2_DeInit(const timer2_t *timer2){
    Std_ReturnType retVal = E_NOT_OK;
    if(NULL == timer2){
        retVal = E_NOT_OK;
    }
    else{
        /* Disable Timer2 */
        TIMER2_MODULE_DISABLE();
        /* Configure the interrupt */
#if TIMER2_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
        TIMER2_InterruptDisable();
#endif
        retVal = E_OK;
    }
    return retVal;
}

/**
 * @brief Write value in Timer2
 * @param timer1 pointer to the timer2 module configurations
 * @param value
 * @return status of the function
 *          (E_OK) : the function done successfully
 *          (E_NOT_OK) : the function has issue to perform this action
 */
Std_ReturnType Timer2_Write_value(const timer2_t *timer2 , uint8 value){
    Std_ReturnType retVal = E_NOT_OK;
    if(NULL == timer2){
        retVal = E_NOT_OK;
    }
    else{
        TMR2 = value;
        retVal = E_OK;
    }
    return retVal;
}

/**
 * @brief Read value from Timer2
 * @param timer2 pointer to the timer2 module configurations
 * @param value
 * @return status of the function
 *          (E_OK) : the function done successfully
 *          (E_NOT_OK) : the function has issue to perform this action
 */
Std_ReturnType Timer2_Read_value(const timer2_t *timer2 , uint8 *value){
    Std_ReturnType retVal = E_NOT_OK;
    if((NULL == timer2) || (NULL == value)){
        retVal = E_NOT_OK;
    }
    else{
        *value = TMR2;
        retVal = E_OK;
    }
    return retVal;
}


/*---------------------------------------------- ISR --------------------------------------------------*/

void TMR2_ISR(void){
    TIMER2_InterruptFlagClear();
    TMR2 = timer2_preload;
    if(TMR2_InterruptHandler){
        TMR2_InterruptHandler();
    }
}
