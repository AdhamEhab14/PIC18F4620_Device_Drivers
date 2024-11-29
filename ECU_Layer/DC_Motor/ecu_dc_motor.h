/* 
 * File:   ecu_dc_motor.h
 * Author: Adham
 *
 * Created on July 20, 2024, 10:20 PM
 */

#ifndef ECU_DC_MOTOR_H
#define	ECU_DC_MOTOR_H

/***************************************Includes***************************************/

#include "ecu_dc_motor_cfg.h"
#include "../../MCAL_Layer/GPIO/hal_gpio.h"

/***********************************Macro Declarations**********************************/
 
#define DC_MOTOR_ON_STATUS  0x01U
#define DC_MOTOR_OFF_STATUS 0x00U

#define DC_MOTOR_PIN1       0x00U
#define DC_MOTOR_PIN2       0x01U

/******************************Macro Function Declarations******************************/

/***********************************Datatype Declarations*******************************/

typedef struct{
    pin_config_t dc_motor_pin[2];
}dc_motor_t;

/***********************************Function Declarations*******************************/

/**
 * @brief Initialize the assigned pin to be INPUT and turn the dc motor ON or OFF
 * @param motor pointer to the dc motor module configurations 
 * @return status of the function
 *          (E_OK) : the function done successfully
 *          (E_NOT_OK) : the function has issue to perform this action
 */
Std_ReturnType dc_motor_initialize(const dc_motor_t * motor);
/**
 * @brief move the motor to the right direction
 * @param motor pointer to the dc motor module configurations 
 * @return status of the function
 *          (E_OK) : the function done successfully
 *          (E_NOT_OK) : the function has issue to perform this action
 */
Std_ReturnType dc_motor_move_right(const dc_motor_t * motor);
/**
 * @brief move the motor to the left direction
 * @param motor pointer to the dc motor module configurations 
 * @return status of the function
 *          (E_OK) : the function done successfully
 *          (E_NOT_OK) : the function has issue to perform this action
 */
Std_ReturnType dc_motor_move_left(const dc_motor_t * motor);
/**
 * @brief stop the motor movement
 * @param motor pointer to the dc motor module configurations 
 * @return status of the function
 *          (E_OK) : the function done successfully
 *          (E_NOT_OK) : the function has issue to perform this action
 */
Std_ReturnType dc_motor_stop(const dc_motor_t * motor);

#endif	/* ECU_DC_MOTOR_H */

