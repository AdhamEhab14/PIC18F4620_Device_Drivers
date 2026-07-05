/* 
 * File:   hal_timer3.c
 * Author: Adham
 *
 * Created on August 27, 2024, 4:52 PM
 */

#include "hal_timer3.h"

#if TIMER3_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
static void(* TMR3_InterruptHandler)(void) = NULL;
#endif

static inline void Timer3_Mode_Select(const timer3_t *timer3);

static volatile uint16 timer3_preload = 0;


/**
 * @brief Initialize Timer3
 * @param timer3 pointer to the timer3 module configurations
 * @return status of the function
 *          (E_OK) : the function done successfully
 *          (E_NOT_OK) : the function has issue to perform this action
 */
Std_ReturnType Timer3_Init(const timer3_t *timer3){
    Std_ReturnType retVal = E_NOT_OK;
    if(NULL == timer3){
        retVal = E_NOT_OK;
    }
    else{
        /* Disable Timer3 */
        TIMER3_MODULE_DISABLE();
        /* Configure Pre-scaler */  
        TIMER3_PRESCALER_SELECT(timer3->timer3_prescaler);
        /* Configure the timer Mode */
        Timer3_Mode_Select(timer3);
        /* Configure Pre-Loaded Value */
        TMR3H = (timer3->timer3_preload_value) >> 8;
        TMR3L = (uint8)(timer3->timer3_preload_value);
        timer3_preload = timer3->timer3_preload_value;
        /* Configure the interrupts */
#if TIMER3_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
        TIMER3_InterruptEnable();    /* Enable Timer3 module interrupt */
        TIMER3_InterruptFlagClear(); /* Clear flag */
        TMR3_InterruptHandler = timer3->TMR3_InterruptHandler;
        /* Check priority & Enable Timer3 module interrupt */
        #if INTERRUPT_PRIORITY_LEVELS_ENABLE==INTERRUPT_FEATURE_ENABLE
        INTERRUPT_PriorityLevelEnable();
        if(INTERRUPT_HIGH_PRIORITY == timer3->priority){
            INTERRUPT_GlobalInterruptHighEnable();
            TIMER3_HighPrioritySet();
        }
        else if(INTERRUPT_LOW_PRIORITY == timer3->priority){
            INTERRUPT_GlobalInterruptLowEnable();
            TIMER3_LowPrioritySet();
        }
        #else
        INTERRUPT_GlobalInterruptEnable();
        INTERRUPT_PeripheralInterruptEnable();
        #endif
#endif
        /* Enable Timer3*/
        TIMER3_MODULE_ENABLE();
        
        retVal = E_OK;
    }
    return retVal;
}

/**
 * @brief DeInitialize Timer3
 * @param timer3 pointer to the timer3 module configurations
 * @return status of the function
 *          (E_OK) : the function done successfully
 *          (E_NOT_OK) : the function has issue to perform this action
 */
Std_ReturnType Timer3_DeInit(const timer3_t *timer3){
    Std_ReturnType retVal = E_NOT_OK;
    if(NULL == timer3){
        retVal = E_NOT_OK;
    }
    else{
        /* Disable Timer3 */
        TIMER3_MODULE_DISABLE();
        /* Configure the interrupt */
#if TIMER3_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
        TIMER3_InterruptDisable();
#endif
        retVal = E_OK;
    }
    return retVal;
}

/**
 * @brief Write value in Timer3
 * @param timer3 pointer to the timer3 module configurations
 * @param value
 * @return status of the function
 *          (E_OK) : the function done successfully
 *          (E_NOT_OK) : the function has issue to perform this action
 */
Std_ReturnType Timer3_Write_value(const timer3_t *timer3 , uint16 value){
    Std_ReturnType retVal = E_NOT_OK;
    if(NULL == timer3){
        retVal = E_NOT_OK;
    }
    else{
        TMR3H = (value) >> 8;
        TMR3L = (uint8)(value);
        retVal = E_OK;
    }
    return retVal;
}

/**
 * @brief Read value from Timer3
 * @param timer3 pointer to the timer1 module configurations
 * @param value
 * @return status of the function
 *          (E_OK) : the function done successfully
 *          (E_NOT_OK) : the function has issue to perform this action
 */
Std_ReturnType Timer3_Read_value(const timer3_t *timer3 , uint16 *value){
    Std_ReturnType retVal = E_NOT_OK;
    uint8 l_tmr3l = 0 , l_tmr3h = 0;
    if(NULL == timer3){
        retVal = E_NOT_OK;
    }
    else{
        l_tmr3l = TMR3L;
        l_tmr3h = TMR3H;
        *value = (uint16)((l_tmr3h << 8) + l_tmr3l);
        retVal = E_OK;
    }
    return retVal;
}


/*------------------------------------ Helper Functions -------------------------------------------*/


static inline void Timer3_Mode_Select(const timer3_t *timer3){
    if(NULL != timer3){
        if(TIMER3_TIMER_MODE == (timer3->timer3_mode)){
            TIMER3_TIMER_MODE_ENABLE();
        }
        else if(TIMER3_COUNTER_MODE == (timer3->timer3_mode)){
            TIMER3_COUNTER_MODE_ENABLE();
            if(TIMER3_ASYNC_COUNTER_MODE == (timer3->timer3_counter_mode)){
                TIMER3_ASYNC_COUNTER_MODE_ENABLE();
            }
            else if(TIMER3_SYNC_COUNTER_MODE == (timer3->timer3_counter_mode)){
                TIMER3_SYNC_COUNTER_MODE_ENABLE();
            }
        }
    }
}

/*---------------------------------------------- ISR --------------------------------------------------*/

void TMR3_ISR(void){
    TIMER3_InterruptFlagClear();
    TMR3H = (timer3_preload) >> 8;
    TMR3L = (uint8)(timer3_preload);
    if(TMR3_InterruptHandler){
        TMR3_InterruptHandler();
    }
}
