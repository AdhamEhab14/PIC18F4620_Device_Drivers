/* 
 * File:   ecu_chr_lcd.h
 * Author: Adham
 *
 * Created on July 30, 2024, 11:05 PM
 */

#ifndef ECU_CHR_LCD_H
#define	ECU_CHR_LCD_H

/***************************************Includes***************************************/

#include "ecu_chr_lcd_cfg.h"
#include "../../MCAL_Layer/GPIO/hal_gpio.h"

/***********************************Macro Declarations**********************************/

#define _LCD_CLEAR                                  0x01
#define _LCD_RETURN_HOME                            0x02
#define _LCD_ENTRY_MODE_DEC_SHIFT_OFF               0x04
#define _LCD_ENTRY_MODE_DEC_SHIFT_ON                0x05
#define _LCD_ENTRY_MODE_INC_SHIFT_OFF               0x06
#define _LCD_ENTRY_MODE_INC_SHIFT_ON                0x07
#define _LCD_CURSOR_MOVE_SHIFT_LEFT                 0x10
#define _LCD_CURSOR_MOVE_SHIFT_RIGHT                0x14
#define _LCD_DISPLAY_SHIFT_LEFT                     0x18
#define _LCD_DISPLAY_SHIFT_RIGHT                    0x1C
#define _LCD_DISPLAY_ON_UNDERLINE_OFF_CURSOR_OFF    0x0C
#define _LCD_DISPLAY_ON_UNDERLINE_OFF_CURSOR_ON     0x0D
#define _LCD_DISPLAY_ON_UNDERLINE_ON_CURSOR_OFF     0x0E
#define _LCD_DISPLAY_ON_UNDERLINE_ON_CURSOR_ON      0x0F
#define _LCD_DISPLAY_OFF_CURSOR_OFF                 0x08
#define _LCD_8BIT_MODE_2_LINE                       0x38
#define _LCD_4BIT_MODE_2_LINE                       0x28

#define _LCD_CGRAM_START 0x40
#define _LCD_DDRAM_START 0x80

#define ROW1 1
#define ROW2 2
#define ROW3 3
#define ROW4 4


/******************************Macro Function Declarations******************************/

/***********************************Datatype Declarations*******************************/

typedef struct{
    pin_config_t lcd_rs;
    pin_config_t lcd_en;
    pin_config_t lcd_data[4];
}chr_lcd_4bit_t;

typedef struct{
    pin_config_t lcd_rs;
    pin_config_t lcd_en;
    pin_config_t lcd_data[8];
}chr_lcd_8bit_t;

/***********************************Function Declarations*******************************/

/**
 * @brief initialize the LCD pins 
 * @param lcd pointer to the LCD configuration module
 * @return status of the function
 *          (E_OK) : the function done successfully
 *          (E_NOT_OK) : the function has issue to perform this action
 */
Std_ReturnType lcd_4bit_initialize(const chr_lcd_4bit_t * lcd);
/**
 * @brief sends a command to the LCD
 * @param lcd pointer to the LCD configuration module
 * @param command
 * @return status of the function
 *          (E_OK) : the function done successfully
 *          (E_NOT_OK) : the function has issue to perform this action
 */
Std_ReturnType lcd_4bit_send_command(const chr_lcd_4bit_t * lcd , uint8 command);
/**
 * @brief send a character to the LCD to be displayed
 * @param lcd pointer to the LCD configuration module
 * @param data
 * @return status of the function
 *          (E_OK) : the function done successfully
 *          (E_NOT_OK) : the function has issue to perform this action
 */
Std_ReturnType lcd_4bit_send_char_data(const chr_lcd_4bit_t * lcd , uint8 data);
/**
 * @brief send a character to the LCD to be displayed in a specific position
 * @param lcd pointer to the LCD configuration module
 * @param row
 * @param column
 * @param data
 * @return status of the function
 *          (E_OK) : the function done successfully
 *          (E_NOT_OK) : the function has issue to perform this action
 */
Std_ReturnType lcd_4bit_send_char_data_pos(const chr_lcd_4bit_t * lcd , uint8 row , uint8 column ,uint8 data);
/**
 * @brief send a sting to the LCD to be displayed
 * @param lcd pointer to the LCD configuration module
 * @param str
 * @return status of the function
 *          (E_OK) : the function done successfully
 *          (E_NOT_OK) : the function has issue to perform this action
 */
Std_ReturnType lcd_4bit_send_string(const chr_lcd_4bit_t * lcd , uint8 * str);
/**
 * @brief send a sting to the LCD to be displayed in a specific position
 * @param lcd pointer to the LCD configuration module
 * @param row
 * @param column
 * @param str
 * @return status of the function
 *          (E_OK) : the function done successfully
 *          (E_NOT_OK) : the function has issue to perform this action
 */
Std_ReturnType lcd_4bit_send_string_pos(const chr_lcd_4bit_t * lcd , uint8 row , uint8 column ,uint8 * str);
/**
 * @brief sends a custom character to be displayed on the LCD
 * @param lcd pointer to the LCD configuration module
 * @param row
 * @param column
 * @param _chr
 * @param mem_pos
 * @return status of the function
 *          (E_OK) : the function done successfully
 *          (E_NOT_OK) : the function has issue to perform this action
 */
Std_ReturnType lcd_4bit_send_custom_char(const chr_lcd_4bit_t * lcd , uint8 row , uint8 column , 
                                            const uint8 _chr[] , uint8 mem_pos);




/**
 * @brief initialize the LCD pins 
 * @param lcd pointer to the LCD configuration module
 * @return status of the function
 *          (E_OK) : the function done successfully
 *          (E_NOT_OK) : the function has issue to perform this action
 */
Std_ReturnType lcd_8bit_initialize(const chr_lcd_8bit_t * lcd);
/**
 * @brief sends a command to the LCD
 * @param lcd pointer to the LCD configuration module
 * @param command
 * @return status of the function
 *          (E_OK) : the function done successfully
 *          (E_NOT_OK) : the function has issue to perform this action
 */
Std_ReturnType lcd_8bit_send_command(const chr_lcd_8bit_t * lcd , uint8 command);
/**
 * @brief send a character to the LCD to be displayed
 * @param lcd pointer to the LCD configuration module
 * @param data
 * @return status of the function
 *          (E_OK) : the function done successfully
 *          (E_NOT_OK) : the function has issue to perform this action
 */
Std_ReturnType lcd_8bit_send_char_data(const chr_lcd_8bit_t * lcd , uint8 data);
/**
 * @brief send a character to the LCD to be displayed in a specific position
 * @param lcd pointer to the LCD configuration module
 * @param row
 * @param column
 * @param data
 * @return status of the function
 *          (E_OK) : the function done successfully
 *          (E_NOT_OK) : the function has issue to perform this action
 */
Std_ReturnType lcd_8bit_send_char_data_pos(const chr_lcd_8bit_t * lcd , uint8 row , uint8 column ,uint8 data);
/**
 * @brief send a sting to the LCD to be displayed
 * @param lcd pointer to the LCD configuration module
 * @param str
 * @return status of the function
 *          (E_OK) : the function done successfully
 *          (E_NOT_OK) : the function has issue to perform this action
 */
Std_ReturnType lcd_8bit_send_string(const chr_lcd_8bit_t * lcd , uint8 * str);
/**
 * @brief send a sting to the LCD to be displayed in a specific position
 * @param lcd pointer to the LCD configuration module
 * @param row
 * @param column
 * @param str
 * @return status of the function
 *          (E_OK) : the function done successfully
 *          (E_NOT_OK) : the function has issue to perform this action
 */
Std_ReturnType lcd_8bit_send_string_pos(const chr_lcd_8bit_t * lcd , uint8 row , uint8 column ,uint8 * str);
/**
 * @brief sends a custom character to be displayed on the LCD
 * @param lcd pointer to the LCD configuration module
 * @param row
 * @param column
 * @param _chr
 * @param mem_pos
 * @return status of the function
 *          (E_OK) : the function done successfully
 *          (E_NOT_OK) : the function has issue to perform this action
 */
Std_ReturnType lcd_8bit_send_custom_char(const chr_lcd_8bit_t * lcd , uint8 row , uint8 column ,
                                            const uint8 _chr[] , uint8 mem_pos);






/**
 * @brief convert uint8 to string 
 * @param value
 * @param str
 * @return status of the function
 *          (E_OK) : the function done successfully
 *          (E_NOT_OK) : the function has issue to perform this action
 */
Std_ReturnType convert_uint8_to_string(uint8 value , uint8 *str);
/**
 * @brief convert uint16 to string
 * @param value
 * @param str
 * @return status of the function
 *          (E_OK) : the function done successfully
 *          (E_NOT_OK) : the function has issue to perform this action
 */
Std_ReturnType convert_uint16_to_string(uint16 value , uint8 *str);
/**
 * @brief convert uint32 to string
 * @param value
 * @param str
 * @return status of the function
 *          (E_OK) : the function done successfully
 *          (E_NOT_OK) : the function has issue to perform this action
 */
Std_ReturnType convert_uint32_to_string(uint32 value , uint8 *str);

#endif	/* ECU_CHR_LCD_H */

