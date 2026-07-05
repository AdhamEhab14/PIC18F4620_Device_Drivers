/* 
 * File:   hal_timer1.c
 * Author: Adham
 *
 * Created on August 26, 2024, 5:06 PM
 */

#include "hal_timer1.h"

#if TIMER1_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
static void(* TMR1_InterruptHandler)(void) = NULL;
#endif

static volatile uint16 timer1_preload = 0;

static inline void Timer1_Mode_Select(const timer1_t *timer1);


/**
 * @brief Initialize Timer1
 * @param timer1 pointer to the timer1 module configurations
 * @return status of the function
 *          (E_OK) : the function done successfully
 *          (E_NOT_OK) : the function has issue to perform this action
 */
Std_ReturnType Timer1_Init(const timer1_t *timer1){
    Std_ReturnType retVal = E_NOT_OK;
    if(NULL == timer1){
        retVal = E_NOT_OK;
    }
    else{
        /* Disable Timer1 */
        TIMER1_MODULE_DISABLE();
        /* Configure Pre-scaler */
        TIMER1_PRESCALER_SELECT(timer1->prescaler_value);
        /* Configure the timer Mode */
        Timer1_Mode_Select(timer1);
        /* Configure Pre-Loaded Value */
        TMR1H = (timer1->timer1_preload_value) >> 8;
        TMR1L = (uint8)(timer1->timer1_preload_value);
        timer1_preload = timer1->timer1_preload_value;
        
        /* Configure the interrupts */
#if TIMER1_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
        TIMER1_InterruptEnable();    /* Enable Timer1 module interrupt */
        TIMER1_InterruptFlagClear(); /* Clear flag */
        TMR1_InterruptHandler = timer1->TMR1_InterruptHandler;
        /* Check priority & Enable Timer1 module interrupt */
        #if INTERRUPT_PRIORITY_LEVELS_ENABLE==INTERRUPT_FEATURE_ENABLE
        INTERRUPT_PriorityLevelEnable();
        if(INTERRUPT_HIGH_PRIORITY == timer1->priority){
            INTERRUPT_GlobalInterruptHighEnable();
            TIMER1_HighPrioritySet();
        }
        else if(INTERRUPT_LOW_PRIORITY == timer1->priority){
            INTERRUPT_GlobalInterruptLowEnable();
            TIMER1_LowPrioritySet();
        }
        #else
        INTERRUPT_GlobalInterruptEnable();
        INTERRUPT_PeripheralInterruptEnable();
        #endif
#endif
        /* Enable Timer1*/
        TIMER1_MODULE_ENABLE();
        
        retVal = E_OK;
    }
    return retVal;
}

/**
 * @brief DeInitialize Timer1
 * @param timer1 pointer to the timer1 module configurations
 * @return status of the function
 *          (E_OK) : the function done successfully
 *          (E_NOT_OK) : the function has issue to perform this action
 */
Std_ReturnType Timer1_DeInit(const timer1_t *timer1){
    Std_ReturnType retVal = E_NOT_OK;
    if(NULL == timer1){
        retVal = E_NOT_OK;
    }
    else{
        /* Disable Timer1 */
        TIMER1_MODULE_DISABLE();
        /* Configure the interrupt */
#if TIMER1_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
        TIMER1_InterruptDisable();
#endif
        retVal = E_OK;
    }
    return retVal;
}

/**
 * @brief Write value in Timer1
 * @param timer1 pointer to the timer1 module configurations
 * @param value
 * @return status of the function
 *          (E_OK) : the function done successfully
 *          (E_NOT_OK) : the function has issue to perform this action
 */
Std_ReturnType Timer1_Write_value(const timer1_t *timer1 , uint16 value){
    Std_ReturnType retVal = E_NOT_OK;
    if(NULL == timer1){
        retVal = E_NOT_OK;
    }
    else{
        TMR1H = (value) >> 8;
        TMR1L = (uint8)(value);
        retVal = E_OK;
    }
    return retVal;
}

/**
 * @brief Read value from Timer1
 * @param timer1 pointer to the timer1 module configurations
 * @param value
 * @return status of the function
 *          (E_OK) : the function done successfully
 *          (E_NOT_OK) : the function has issue to perform this action
 */
Std_ReturnType Timer1_Read_value(const timer1_t *timer1 , uint16 *value){
    Std_ReturnType retVal = E_NOT_OK;
    uint8 l_tmr1l = 0 , l_tmr1h = 0;
    if((NULL == timer1) || (NULL == value)){
        retVal = E_NOT_OK;
    }
    else{
        l_tmr1l = TMR1L;
        l_tmr1h = TMR1H;
        *value = (uint16)((l_tmr1h << 8) + l_tmr1l);
        retVal = E_OK;
    }
    return retVal;
}

/*---------------------------------------- Helper Functions ------------------------------------------*/

static inline void Timer1_Mode_Select(const timer1_t *timer1){
    if(NULL != timer1){
        if(TIMER1_TIMER_MODE == (timer1->timer1_mode)){
            TIMER1_TIMER_MODE_ENABLE();
        }
        else if(TIMER1_COUNTER_MODE == (timer1->timer1_mode)){
            TIMER1_COUNTER_MODE_ENABLE();
            if(TIMER1_ASYNC_COUNTER_MODE == (timer1->timer1_counter_mode)){
                TIMER1_ASYNC_COUNTER_MODE_ENABLE();
            }
            else if(TIMER1_SYNC_COUNTER_MODE == (timer1->timer1_counter_mode)){
                TIMER1_SYNC_COUNTER_MODE_ENABLE();
            }
        }
    }
}

/*---------------------------------------------- ISR --------------------------------------------------*/

void TMR1_ISR(void){
    TIMER1_InterruptFlagClear();
    TMR1H = (timer1_preload) >> 8;
    TMR1L = (uint8)(timer1_preload);
    if(TMR1_InterruptHandler){
        TMR1_InterruptHandler();
    }
}
