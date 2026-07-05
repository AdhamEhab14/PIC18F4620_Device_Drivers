/* 
 * File:   ecu_relay.c
 * Author: Adham
 *
 * Created on July 19, 2024, 10:03 PM
 */


#include "ecu_relay.h"


/**
 * @brief Initialize the assigned pin to be OUTPUT and turn the relay ON or OFF
 * @param rly pointer to the relay module configurations
 * @return status of the function
 *          (E_OK) : the function done successfully
 *          (E_NOT_OK) : the function has issue to perform this action
 */
Std_ReturnType relay_initialize(const relay_t* rly){
    Std_ReturnType retVal = E_NOT_OK;
    if(NULL == rly){
        retVal = E_NOT_OK;
    }
    else{
        pin_config_t relay_obj = {.port = rly->relay_port , 
                                  .pin = rly->relay_pin ,
                                  .direction = GPIO_DIRECTION_OUTPUT,
                                  .logic = rly->relay_status};
        gpio_pin_initialize(&relay_obj);
        retVal = E_OK;
    }
    return retVal;
}
/**
 * @brief Turn the relay ON
 * @param rly pointer to the relay module configurations
 * @return status of the function
 *          (E_OK) : the function done successfully
 *          (E_NOT_OK) : the function has issue to perform this action
 */
Std_ReturnType relay_turn_on(const relay_t* rly){
    Std_ReturnType retVal = E_NOT_OK;
    if(NULL == rly){
        retVal = E_NOT_OK;
    }
    else{
        pin_config_t relay_obj = {.port = rly->relay_port , 
                                  .pin = rly->relay_pin ,
                                  .direction = GPIO_DIRECTION_OUTPUT,
                                  .logic = rly->relay_status};
        gpio_pin_write_logic(&relay_obj , GPIO_HIGH);
        retVal = E_OK;
    }
    return retVal;
}
/**
 * @brief Turn the relay OFF
 * @param rly pointer to the relay module configurations
 * @return status of the function
 *          (E_OK) : the function done successfully
 *          (E_NOT_OK) : the function has issue to perform this action
 */
Std_ReturnType relay_turn_off(const relay_t* rly){
    Std_ReturnType retVal = E_NOT_OK;
    if(NULL == rly){
        retVal = E_NOT_OK;
    }
    else{
        pin_config_t relay_obj = {.port = rly->relay_port , 
                                  .pin = rly->relay_pin ,
                                  .direction = GPIO_DIRECTION_OUTPUT,
                                  .logic = rly->relay_status};
        gpio_pin_write_logic(&relay_obj , GPIO_LOW);
        retVal = E_OK;
    }
    return retVal;
}