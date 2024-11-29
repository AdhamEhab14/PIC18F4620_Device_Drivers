/* 
 * File:   ecu_relay.h
 * Author: Adham
 *
 * Created on July 19, 2024, 10:01 PM
 */

#ifndef ECU_RELAY_H
#define	ECU_RELAY_H

/***************************************Includes***************************************/

#include "ecu_relay_cfg.h"
#include "../../MCAL_Layer/GPIO/hal_gpio.h"

/***********************************Macro Declarations**********************************/

#define RELAY_ON_STATUS  0x01U
#define RELAY_OFF_STATUS 0x00U

/******************************Macro Function Declarations******************************/

/***********************************Datatype Declarations*******************************/

typedef struct{
    uint8 relay_port     : 3;
    uint8 relay_pin      : 3;
    uint8 relay_status   : 1;
    uint8 relay_reserved : 1;
}relay_t;

/***********************************Function Declarations*******************************/

/**
 * @brief Initialize the assigned pin to be OUTPUT and turn the relay ON or OFF
 * @param rly pointer to the relay module configurations
 * @return status of the function
 *          (E_OK) : the function done successfully
 *          (E_NOT_OK) : the function has issue to perform this action
 */
Std_ReturnType relay_initialize(const relay_t* rly);
/**
 * @brief Turn the relay ON
 * @param rly pointer to the relay module configurations
 * @return status of the function
 *          (E_OK) : the function done successfully
 *          (E_NOT_OK) : the function has issue to perform this action
 */
Std_ReturnType relay_turn_on(const relay_t* rly);
/**
 * @brief Turn the relay OFF
 * @param rly pointer to the relay module configurations
 * @return status of the function
 *          (E_OK) : the function done successfully
 *          (E_NOT_OK) : the function has issue to perform this action
 */
Std_ReturnType relay_turn_off(const relay_t* rly);

#endif	/* ECU_RELAY_H */

