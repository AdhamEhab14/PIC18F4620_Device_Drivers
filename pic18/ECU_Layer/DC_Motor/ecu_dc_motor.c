/* 
 * File:   ecu_dc_motor.c
 * Author: Adham
 *
 * Created on July 20, 2024, 10:20 PM
 */


#include "ecu_dc_motor.h"

/**
 * @brief Initialize the assigned pin to be INPUT and turn the dc motor ON or OFF
 * @param motor pointer to the dc motor module configurations 
 * @return status of the function
 *          (E_OK) : the function done successfully
 *          (E_NOT_OK) : the function has issue to perform this action
 */
Std_ReturnType dc_motor_initialize(const dc_motor_t * motor){
    Std_ReturnType retVal = E_NOT_OK;
    if(NULL == motor){
        retVal = E_NOT_OK;
    }
    else{
        gpio_pin_initialize(&(motor->dc_motor_pin[DC_MOTOR_PIN1]));
        gpio_pin_initialize(&(motor->dc_motor_pin[DC_MOTOR_PIN2]));
        retVal = E_OK;
    }
    return retVal;
}
/**
 * @brief move the motor to the right direction
 * @param motor pointer to the dc motor module configurations 
 * @return status of the function
 *          (E_OK) : the function done successfully
 *          (E_NOT_OK) : the function has issue to perform this action
 */
Std_ReturnType dc_motor_move_right(const dc_motor_t * motor){
    Std_ReturnType retVal = E_NOT_OK;
    if(NULL == motor){
        retVal = E_NOT_OK;
    }
    else{
        gpio_pin_write_logic(&(motor->dc_motor_pin[DC_MOTOR_PIN1]) , GPIO_HIGH);
        gpio_pin_write_logic(&(motor->dc_motor_pin[DC_MOTOR_PIN2]) , GPIO_LOW);
        retVal = E_OK;
    }
    return retVal;
}
/**
 * @brief move the motor to the left direction
 * @param motor pointer to the dc motor module configurations 
 * @return status of the function
 *          (E_OK) : the function done successfully
 *          (E_NOT_OK) : the function has issue to perform this action
 */
Std_ReturnType dc_motor_move_left(const dc_motor_t * motor){
    Std_ReturnType retVal = E_NOT_OK;
    if(NULL == motor){
        retVal = E_NOT_OK;
    }
    else{
        gpio_pin_write_logic(&(motor->dc_motor_pin[DC_MOTOR_PIN1]) , GPIO_LOW);
        gpio_pin_write_logic(&(motor->dc_motor_pin[DC_MOTOR_PIN2]) , GPIO_HIGH);
        retVal = E_OK;
    }
    return retVal;
}
/**
 * @brief stop the motor movement
 * @param motor pointer to the dc motor module configurations 
 * @return status of the function
 *          (E_OK) : the function done successfully
 *          (E_NOT_OK) : the function has issue to perform this action
 */
Std_ReturnType dc_motor_stop(const dc_motor_t * motor){
    Std_ReturnType retVal = E_NOT_OK;
    if(NULL == motor){
        retVal = E_NOT_OK;
    }
    else{
        gpio_pin_write_logic(&(motor->dc_motor_pin[DC_MOTOR_PIN1]) , GPIO_LOW);
        gpio_pin_write_logic(&(motor->dc_motor_pin[DC_MOTOR_PIN2]) , GPIO_LOW);
        retVal = E_OK;
    }
    return retVal;
}