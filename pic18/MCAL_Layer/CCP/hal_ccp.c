/* 
 * File:   hal_ccp.c
 * Author: Adham
 *
 * Created on September 6, 2024, 6:43 PM
 */

#include "hal_ccp.h"

#if CCP1_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
    static void (*CCP1_InterruptHandler)(void) = NULL;
#endif
    
#if CCP2_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
    static void (*CCP2_InterruptHandler)(void) = NULL;
#endif
  
static void CCP_Interrupt_Config(const ccp_t *_ccp_obj);
static void CCP_PWM_Mode_Config(const ccp_t *_ccp_obj);
static void CCP_Mode_Timer_Select(const ccp_t *_ccp_obj);
static Std_ReturnType CCP_Capture_Mode_Config(const ccp_t *_ccp_obj);
static Std_ReturnType CCP_Compare_Mode_Config(const ccp_t *_ccp_obj);

/**
 * @brief Initialize CCP1 module
 * @param _ccp_obj pointer to the CCP module configurations
 * @return status of the function
 *          (E_OK) : the function done successfully
 *          (E_NOT_OK) : the function has issue to perform this action
 */
Std_ReturnType CCP_Init(const ccp_t *_ccp_obj){
    Std_ReturnType retVal = E_NOT_OK;
    if(NULL == _ccp_obj){
        retVal = E_NOT_OK;
    }
    else{
        /* CCP Module Disable */
        if(CCP1_INST == _ccp_obj->ccp_inst){
            CCP1_SET_MODE(CCP_MODULE_DISABLE);
        }
        else if(CCP2_INST == _ccp_obj->ccp_inst){
            CCP2_SET_MODE(CCP_MODULE_DISABLE);
        }
        else { /* Nothing */ }
        
        /* CCP Module Capture Mode Initialization */
        if(CCP_CAPTURE_MODE_SELECTED == _ccp_obj->ccp_mode){
            retVal = CCP_Capture_Mode_Config(_ccp_obj);
        }
        /* CCP Module Compare Mode Initialization */
        else if(CCP_COMPARE_MODE_SELECTED == _ccp_obj->ccp_mode){
            retVal = CCP_Compare_Mode_Config(_ccp_obj);
        }
#if (CCP1_CFG_SELECTED_MODE==CCP_CFG_PWM_MODE_SELECTED) || (CCP2_CFG_SELECTED_MODE==CCP_CFG_PWM_MODE_SELECTED)
        /* CCP Module PWM Mode Initialization */
        else if(CCP_PWM_MODE_SELECTED == _ccp_obj->ccp_mode){
            CCP_PWM_Mode_Config(_ccp_obj);
        }
#endif
        else {/* Nothing */}
        
        /* PIN Configurations */
        retVal = gpio_pin_initialize(&(_ccp_obj->ccp_pin));
        /* Interrupt Configurations for CCP1 and CCP2 Modules */
        CCP_Interrupt_Config(_ccp_obj);

        retVal = E_OK;
    }
    return retVal;
}

/**
 * @brief DeInitialize CCP1 module
 * @param _ccp_obj pointer to the CCP module configurations
 * @return status of the function
 *          (E_OK) : the function done successfully
 *          (E_NOT_OK) : the function has issue to perform this action
 */
Std_ReturnType CCP_DeInit(const ccp_t *_ccp_obj){
    Std_ReturnType retVal = E_NOT_OK;
    if(NULL == _ccp_obj){
        retVal = E_NOT_OK;
    }
    else{
        if(CCP1_INST == _ccp_obj->ccp_inst){
            CCP1_SET_MODE(CCP_MODULE_DISABLE); /* CCP1 Module Disable */
/* Interrupt Configurations */ 
#if CCP1_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
            CCP1_InterruptDisable();
#endif 
        }
        else if(CCP2_INST == _ccp_obj->ccp_inst){
            CCP2_SET_MODE(CCP_MODULE_DISABLE); /* CCP2 Module Disable */
#if CCP2_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
            CCP2_InterruptDisable();
#endif  
        }
        else { /* Nothing */ }
 
        retVal = E_OK;
    }
    return retVal;
}



#if (CCP1_CFG_SELECTED_MODE==CCP_CFG_CAPTURE_MODE_SELECTED) || (CCP2_CFG_SELECTED_MODE==CCP_CFG_CAPTURE_MODE_SELECTED)
/**
 * @brief Check if the captured data is ready 
 * @param capture_status pointer to the captured data
 * @return status o the function
 *          (E_OK) : the function done successfully
 *          (E_NOT_OK) : the function has issue to perform this action
 */
Std_ReturnType CCP_Is_Captured_Data_Ready(const ccp_t *_ccp_obj, uint8 *_capture_status){
    Std_ReturnType retVal = E_NOT_OK;
    if(NULL == _capture_status){
        retVal = E_NOT_OK;
    }
    else{
        if(CCP1_INST == _ccp_obj->ccp_inst){
            if(CCP_CAPTURE_READY == PIR1bits.CCP1IF){
                *_capture_status = CCP_CAPTURE_READY;
                CCP1_InterruptFlagClear();
            }
            else{
                *_capture_status = CCP_CAPTURE_NOT_READY;
            }
        }
        else if(CCP2_INST == _ccp_obj->ccp_inst){
            if(CCP_CAPTURE_READY == PIR2bits.CCP2IF){
                *_capture_status = CCP_CAPTURE_READY;
                CCP2_InterruptFlagClear();
            }
            else{
                *_capture_status = CCP_CAPTURE_NOT_READY;
            }
        }
        retVal = E_OK;
    }
    return retVal;
}

/**
 * @brief Read a value in capture mode
 * @param capture_value pointer to the data to be read
 * @return status of the function
 *          (E_OK) : the function done successfully
 *          (E_NOT_OK) : the function has issue to perform this action
 */
Std_ReturnType CCP_Capture_Mode_Read_Value(uint16 *capture_value){
    Std_ReturnType retVal = E_NOT_OK;
    CCP_REG_T capture_temp_value = {.ccpr_low = 0, .ccpr_high = 0};
    if(NULL == capture_value){
        retVal = E_NOT_OK;
    }
    else{
        /* Copy captured value */
        capture_temp_value.ccpr_low = CCPR1L;
        capture_temp_value.ccpr_high = CCPR1H;
        /* Write the 16-bit capture value */
        *capture_value = capture_temp_value.ccpr_16Bit;
        retVal = E_OK;
    }
    return retVal;
}

#endif



#if (CCP1_CFG_SELECTED_MODE==CCP_CFG_COMPARE_MODE_SELECTED) || (CCP2_CFG_SELECTED_MODE==CCP_CFG_COMPARE_MODE_SELECTED)
/**
 * @brief Check if the compare operation is complete
 * @param compare_status pointer to the compare status
 * @return status of the function
 *          (E_OK) : the function done successfully
 *          (E_NOT_OK) : the function has issue to perform this action
 */
Std_ReturnType CCP_Is_Compare_Complete(const ccp_t *_ccp_obj, uint8 *_compare_status){
    Std_ReturnType retVal = E_NOT_OK;
    if(NULL == _compare_status){
        retVal = E_NOT_OK;
    }
    else{
        if(CCP1_INST == _ccp_obj->ccp_inst){
            if(CCP_COMPARE_READY == PIR1bits.CCP1IF){
                *_compare_status = CCP_COMPARE_READY;
                CCP1_InterruptFlagClear();
            }
            else{
                *_compare_status = CCP_COMPARE_NOT_READY;
            }
        }
        else if(CCP2_INST == _ccp_obj->ccp_inst){
            if(CCP_COMPARE_READY == PIR2bits.CCP2IF){
            *_compare_status = CCP_COMPARE_READY;
            CCP2_InterruptFlagClear();
            }
            else{
                *_compare_status = CCP_COMPARE_NOT_READY;
            }
        }
        retVal = E_OK;
    }
    return retVal;
}

/**
 * @brief Set a value to be compared with
 * @param capture_value
 * @return status of the function
 *          (E_OK) : the function done successfully
 *          (E_NOT_OK) : the function has issue to perform this action
 */
Std_ReturnType CCP_Compare_Mode_Set_Value(const ccp_t *_ccp_obj, uint16 compare_value){
    Std_ReturnType retVal = E_NOT_OK;
    CCP_REG_T capture_temp_value = {.ccpr_low = 0, .ccpr_high = 0};
    if(NULL == _ccp_obj){
        retVal = E_NOT_OK;
    }
    else{
        /* Copy compared value */
        capture_temp_value.ccpr_16Bit = compare_value;

        if(CCP1_INST == _ccp_obj->ccp_inst){
            CCPR1L = capture_temp_value.ccpr_low;
            CCPR1H = capture_temp_value.ccpr_high;
        }
        else if(CCP2_INST == _ccp_obj->ccp_inst){
            CCPR2L = capture_temp_value.ccpr_low;
            CCPR2H = capture_temp_value.ccpr_high;
        }
        retVal = E_OK;
    }
    return retVal; 
}

#endif



#if (CCP1_CFG_SELECTED_MODE==CCP_CFG_PWM_MODE_SELECTED) || (CCP2_CFG_SELECTED_MODE==CCP_CFG_PWM_MODE_SELECTED)
/**
 * @brief Set the duty cycle with a value
 * @param duty
 * @return status of the function
 *          (E_OK) : the function done successfully
 *          (E_NOT_OK) : the function has issue to perform this action
 */
Std_ReturnType CCP_PWM_Set_Duty(const ccp_t *_ccp_obj, const uint8 _duty){
    Std_ReturnType retVal = E_NOT_OK;
    uint16 l_duty_temp = 0;
    
    if(NULL == _ccp_obj){
        retVal = E_NOT_OK;
    }
    else{
        l_duty_temp = (uint16)(4 * (PR2 + 1) * (_duty / 100.0));
        
        if(CCP1_INST == _ccp_obj->ccp_inst){
            CCP1CONbits.DC1B = (uint8)(l_duty_temp & 0x0003);
            CCPR1L = (uint8)(l_duty_temp >> 2);
        }
        else if(CCP2_INST == _ccp_obj->ccp_inst){
            CCP2CONbits.DC2B = (uint8)(l_duty_temp & 0x0003);
            CCPR2L = (uint8)(l_duty_temp >> 2);
        }
        else{ /* Nothing */ }
        
        retVal = E_OK;
    }
    return retVal;
}

/**
 * @brief Start the PWM operation
 * @return status of the function
 *          (E_OK) : the function done successfully
 *          (E_NOT_OK) : the function has issue to perform this action
 */
Std_ReturnType CCP_PWM_Start(const ccp_t *_ccp_obj){
    Std_ReturnType retVal = E_NOT_OK;
    if(NULL == _ccp_obj){
        retVal = E_NOT_OK;
    }
    else{
        if(CCP1_INST == _ccp_obj->ccp_inst){
            CCP1CONbits.CCP1M = CCP_PWM_MODE;
        }
        else if(CCP2_INST == _ccp_obj->ccp_inst){
            CCP2CONbits.CCP2M = CCP_PWM_MODE;
        }
        else{ /* Nothing */ }
        retVal = E_OK;
    }
    return retVal;
}

/**
 * @brief Stop the PWM operation
 * @return status of the function
 *          (E_OK) : the function done successfully
 *          (E_NOT_OK) : the function has issue to perform this action
 */
Std_ReturnType CCP_PWM_Stop(const ccp_t *_ccp_obj){
    Std_ReturnType retVal = E_NOT_OK;
    if(NULL == _ccp_obj){
        retVal = E_NOT_OK;
    }
    else{
        if(CCP1_INST == _ccp_obj->ccp_inst){
            CCP1CONbits.CCP1M = CCP_MODULE_DISABLE;
        }
        else if(CCP2_INST == _ccp_obj->ccp_inst){
            CCP2CONbits.CCP2M = CCP_MODULE_DISABLE;
        }
        else{ /* Nothing */ }
        retVal = E_OK;
    }
    return retVal;
}

#endif


/*-------------------------------Helper Functions-------------------------------*/

#if (CCP1_CFG_SELECTED_MODE==CCP_CFG_PWM_MODE_SELECTED) || (CCP2_CFG_SELECTED_MODE==CCP_CFG_PWM_MODE_SELECTED)
static void CCP_PWM_Mode_Config(const ccp_t *_ccp_obj){
    if(CCP1_INST == _ccp_obj->ccp_inst){
        if(CCP_PWM_MODE == _ccp_obj->ccp_mode_variant){
            CCP1_SET_MODE(CCP_PWM_MODE);
        }
        else {/* Nothing */}
    }
    else if(CCP2_INST == _ccp_obj->ccp_inst){
        if(CCP_PWM_MODE == _ccp_obj->ccp_mode_variant){
            CCP2_SET_MODE(CCP_PWM_MODE);
        }
        else {/* Nothing */}
    }
    else{ /* Nothing */ }
           
    /* PWM Frequency Initialization */
    PR2 = (uint8)((_XTAL_FREQ / (_ccp_obj->PWM_Frequency * 4.0 * _ccp_obj->timer2_prescaler_value * 
                    _ccp_obj->timer2_postscaler_value)) - 1);
}
#endif

static void CCP_Interrupt_Config(const ccp_t *_ccp_obj){
/* CCP1 Interrupt Configurations */ 
#if CCP1_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
    CCP1_InterruptEnable();
    CCP1_InterruptFlagClear();
    CCP1_InterruptHandler = _ccp_obj->CCP1_InterruptHandler;
/* Interrupt Priority Configurations */
#if INTERRUPT_PRIORITY_LEVELS_ENABLE==INTERRUPT_FEATURE_ENABLE 
    INTERRUPT_PriorityLevelsEnable();
    if(INTERRUPT_HIGH_PRIORITY == _ccp_obj->CCP1_priority){
        /* Enables all high-priority interrupts */
        INTERRUPT_GlobalInterruptHighEnable();
        CCP1_HighPrioritySet();
    }
    else if(INTERRUPT_LOW_PRIORITY == _ccp_obj->CCP1_priority){
        /* Enables all unmasked peripheral interrupts */
        INTERRUPT_GlobalInterruptLowEnable();
        CCP1_LowPrioritySet();
    }
    else{ /* Nothing */ }
#else
    INTERRUPT_GlobalInterruptEnable();
    INTERRUPT_PeripheralInterruptEnable();
#endif          
#endif    
        
/* CCP2 Interrupt Configurations */ 
#if CCP2_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
    CCP2_InterruptEnable();
    CCP2_InterruptFlagClear();
    CCP2_InterruptHandler = _ccp_obj->CCP2_InterruptHandler;
/* Interrupt Priority Configurations */
#if INTERRUPT_PRIORITY_LEVELS_ENABLE==INTERRUPT_FEATURE_ENABLE 
    INTERRUPT_PriorityLevelsEnable();
    if(INTERRUPT_HIGH_PRIORITY == _ccp_obj->CCP2_priority){
        /* Enables all high-priority interrupts */
        INTERRUPT_GlobalInterruptHighEnable();
        CCP2_HighPrioritySet();
    }
    else if(INTERRUPT_LOW_PRIORITY == _ccp_obj->CCP2_priority){
        /* Enables all unmasked peripheral interrupts */
        INTERRUPT_GlobalInterruptLowEnable();
        CCP2_LowPrioritySet();
    }
    else{ /* Nothing */ }
#else
    INTERRUPT_GlobalInterruptEnable();
    INTERRUPT_PeripheralInterruptEnable();
#endif          
#endif 
}

static void CCP_Mode_Timer_Select(const ccp_t *_ccp_obj){
    if(CCP1_CCP2_TIMER3 == _ccp_obj->ccp_capture_timer){
        /* Timer3 is the capture/compare clock source for the CCP modules */
        T3CONbits.T3CCP1 = 0; 
        T3CONbits.T3CCP2 = 1;
    }
    else if(CCP1_TIMER1_CCP2_TIMER3 == _ccp_obj->ccp_capture_timer){
        /* Timer3 is the capture/compare clock source for CCP2 */
        /* Timer1 is the capture/compare clock source for CCP1 */
        T3CONbits.T3CCP1 = 1;
        T3CONbits.T3CCP2 = 0;
    }
    else if(CCP1_CCP2_TIMER1 == _ccp_obj->ccp_capture_timer){
        /* Timer1 is the capture/compare clock source for the CCP modules */
        T3CONbits.T3CCP1 = 0;
        T3CONbits.T3CCP2 = 0;
    }
    else{ /* Nothing */ }
}

static Std_ReturnType CCP_Capture_Mode_Config(const ccp_t *_ccp_obj){
    Std_ReturnType ret = E_OK;
    
    if(CCP1_INST == _ccp_obj->ccp_inst){
        /* CCP1 Module Capture variants Mode Initialization */
        switch(_ccp_obj->ccp_mode_variant){
            case CCP_CAPTURE_MODE_1_FALLING_EDGE : CCP1_SET_MODE(CCP_CAPTURE_MODE_1_FALLING_EDGE); break;
            case CCP_CAPTURE_MODE_1_RISING_EDGE  : CCP1_SET_MODE(CCP_CAPTURE_MODE_1_RISING_EDGE);  break;
            case CCP_CAPTURE_MODE_4_RISING_EDGE  : CCP1_SET_MODE(CCP_CAPTURE_MODE_4_RISING_EDGE);  break;
            case CCP_CAPTURE_MODE_16_RISING_EDGE : CCP1_SET_MODE(CCP_CAPTURE_MODE_16_RISING_EDGE); break;
            default : ret = E_NOT_OK; /* Not supported variant */
        }
    }
    else if(CCP2_INST == _ccp_obj->ccp_inst){
        /* CCP2 Module Capture variants Mode Initialization */
        switch(_ccp_obj->ccp_mode_variant){
            case CCP_CAPTURE_MODE_1_FALLING_EDGE : CCP2_SET_MODE(CCP_CAPTURE_MODE_1_FALLING_EDGE); break;
            case CCP_CAPTURE_MODE_1_RISING_EDGE  : CCP2_SET_MODE(CCP_CAPTURE_MODE_1_RISING_EDGE);  break;
            case CCP_CAPTURE_MODE_4_RISING_EDGE  : CCP2_SET_MODE(CCP_CAPTURE_MODE_4_RISING_EDGE);  break;
            case CCP_CAPTURE_MODE_16_RISING_EDGE : CCP2_SET_MODE(CCP_CAPTURE_MODE_16_RISING_EDGE); break;
            default : ret = E_NOT_OK; /* Not supported variant */
        }
    }
    else{ /* Nothing */ }
    
    CCP_Mode_Timer_Select(_ccp_obj);
    
    return ret;
}

static Std_ReturnType CCP_Compare_Mode_Config(const ccp_t *_ccp_obj){
    Std_ReturnType ret = E_OK;
    
    if(CCP1_INST == _ccp_obj->ccp_inst){
        /* CCP1 Module Compare variants Mode Initialization */
        switch(_ccp_obj->ccp_mode_variant){
            case CCP_COMPARE_MODE_SET_PIN_LOW      : CCP1_SET_MODE(CCP_COMPARE_MODE_SET_PIN_LOW);      break;
            case CCP_COMPARE_MODE_SET_PIN_HIGH     : CCP1_SET_MODE(CCP_COMPARE_MODE_SET_PIN_HIGH);     break;
            case CCP_COMPARE_MODE_TOGGLE_ON_MATCH  : CCP1_SET_MODE(CCP_COMPARE_MODE_TOGGLE_ON_MATCH);  break;
            case CCP_COMPARE_MODE_GEN_SW_INTERRUPT : CCP1_SET_MODE(CCP_COMPARE_MODE_GEN_SW_INTERRUPT); break;
            case CCP_COMPARE_MODE_GEN_EVENT        : CCP1_SET_MODE(CCP_COMPARE_MODE_GEN_EVENT);        break;
            default : ret = E_NOT_OK; /* Not supported variant */
        }
    }
    else if(CCP2_INST == _ccp_obj->ccp_inst){
        /* CCP1 Module Compare variants Mode Initialization */
        switch(_ccp_obj->ccp_mode_variant){
            case CCP_COMPARE_MODE_SET_PIN_LOW      : CCP2_SET_MODE(CCP_COMPARE_MODE_SET_PIN_LOW);      break;
            case CCP_COMPARE_MODE_SET_PIN_HIGH     : CCP2_SET_MODE(CCP_COMPARE_MODE_SET_PIN_HIGH);     break;
            case CCP_COMPARE_MODE_TOGGLE_ON_MATCH  : CCP2_SET_MODE(CCP_COMPARE_MODE_TOGGLE_ON_MATCH);  break;
            case CCP_COMPARE_MODE_GEN_SW_INTERRUPT : CCP2_SET_MODE(CCP_COMPARE_MODE_GEN_SW_INTERRUPT); break;
            case CCP_COMPARE_MODE_GEN_EVENT        : CCP2_SET_MODE(CCP_COMPARE_MODE_GEN_EVENT);        break;
            default : ret = E_NOT_OK; /* Not supported variant */
        }
    }
    else{ /* Nothing */ }
    
    CCP_Mode_Timer_Select(_ccp_obj);
    
    return ret;
}


/*---------------------------------------ISR--------------------------------------*/


void CCP1_ISR(void){
    CCP1_InterruptFlagClear();
    if(CCP1_InterruptHandler){
        CCP1_InterruptHandler();
    }
    else{ /* Nothing */ }
}

void CCP2_ISR(void){
    CCP2_InterruptFlagClear();
    if(CCP2_InterruptHandler){
        CCP2_InterruptHandler();
    }
    else{ /* Nothing */ }
}
