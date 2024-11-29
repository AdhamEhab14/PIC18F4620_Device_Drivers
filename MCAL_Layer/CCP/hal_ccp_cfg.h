/* 
 * File:   hal_ccp_cfg.h
 * Author: Adham
 *
 * Created on September 6, 2024, 6:43 PM
 */

#ifndef HAL_CCP_CFG_H
#define	HAL_CCP_CFG_H

/***************************************Includes***************************************/

/***********************************Macro Declarations**********************************/

#define CCP_CFG_CAPTURE_MODE_SELECTED  0x00
#define CCP_CFG_COMPARE_MODE_SELECTED  0x01
#define CCP_CFG_PWM_MODE_SELECTED      0x02

#define CCP1_CFG_SELECTED_MODE (CCP_CFG_COMPARE_MODE_SELECTED)
#define CCP2_CFG_SELECTED_MODE (CCP_CFG_PWM_MODE_SELECTED)

/******************************Macro Function Declarations******************************/

/***********************************Datatype Declarations*******************************/

/***********************************Function Declarations*******************************/

#endif	/* HAL_CCP_CFG_H */

