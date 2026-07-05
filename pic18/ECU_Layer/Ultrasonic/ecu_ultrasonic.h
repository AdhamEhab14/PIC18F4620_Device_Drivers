/* 
 * File:   ecu_ultrasonic.h
 * Author: Adham
 *
 * Created on September 11, 2024, 6:42 PM
 */

#ifndef ECU_ULTRASONIC_H
#define	ECU_ULTRASONIC_H

/****************************************Includes***************************************/

#include "ecu_ultrasonic_cfg.h"
#include "../../MCAL_Layer/GPIO/hal_gpio.h"

/***********************************Macro Declarations**********************************/

#define SOUND_SPEED_CM_SECOND       34000

/******************************Macro Function Declarations******************************/

/***********************************Datatype Declarations*******************************/

typedef struct{
    pin_config_t Trigger_Pin;
    pin_config_t Echo_Pin; 
}ultrasonic_t;

/***********************************Function Declarations*******************************/

Std_ReturnType Ultrasonic_Init(const ultrasonic_t *_config);
Std_ReturnType Ultrasonic_Calculate_Distance(const ultrasonic_t *_config, uint16 *Distance);

#endif	/* ECU_ULTRASONIC_H */

