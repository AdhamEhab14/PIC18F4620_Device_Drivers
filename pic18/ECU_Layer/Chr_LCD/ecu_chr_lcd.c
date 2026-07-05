/* 
 * File:   ecu_chr_lcd.c
 * Author: Adham
 *
 * Created on July 30, 2024, 11:05 PM
 */

#include "ecu_chr_lcd.h"

static Std_ReturnType lcd_send_4bits(const chr_lcd_4bit_t * lcd , uint8 data_command);
static Std_ReturnType lcd_4bit_send_enable_signal(const chr_lcd_4bit_t * lcd);
static Std_ReturnType lcd_4bit_set_cursor(const chr_lcd_4bit_t * lcd , uint8 row , uint8 column);

static Std_ReturnType lcd_8bit_send_enable_signal(const chr_lcd_8bit_t * lcd);
static Std_ReturnType lcd_8bit_set_cursor(const chr_lcd_8bit_t * lcd , uint8 row , uint8 column);


/**
 * @brief initialize the LCD pins 
 * @param lcd pointer to the LCD configuration module
 * @return status of the function
 *          (E_OK) : the function done successfully
 *          (E_NOT_OK) : the function has issue to perform this action
 */
Std_ReturnType lcd_4bit_initialize(const chr_lcd_4bit_t * lcd){
    Std_ReturnType retVal = E_NOT_OK;
    if(NULL == lcd){
        retVal = E_NOT_OK;
    }
    else{
        retVal = gpio_pin_initialize(&(lcd->lcd_rs));
        retVal = gpio_pin_initialize(&(lcd->lcd_en));
        for(uint8 counter = 0 ; counter < 4 ; counter++){
            retVal = gpio_pin_initialize(&(lcd->lcd_data[counter]));
        }
        __delay_ms(20);
        retVal = lcd_4bit_send_command(lcd , _LCD_8BIT_MODE_2_LINE);
        __delay_ms(5);
        retVal = lcd_4bit_send_command(lcd , _LCD_8BIT_MODE_2_LINE);
        __delay_us(150);
        retVal = lcd_4bit_send_command(lcd , _LCD_8BIT_MODE_2_LINE);
        
        retVal = lcd_4bit_send_command(lcd , _LCD_CLEAR);
        retVal = lcd_4bit_send_command(lcd , _LCD_RETURN_HOME);
        retVal = lcd_4bit_send_command(lcd , _LCD_ENTRY_MODE_INC_SHIFT_OFF);
        retVal = lcd_4bit_send_command(lcd , _LCD_DISPLAY_ON_UNDERLINE_OFF_CURSOR_OFF);
        retVal = lcd_4bit_send_command(lcd , _LCD_4BIT_MODE_2_LINE);
        retVal = lcd_4bit_send_command(lcd , 0x80);
        retVal = E_OK;
    }
    return retVal;
}

/**
 * @brief sends a command to the LCD
 * @param lcd pointer to the LCD configuration module
 * @param command
 * @return status of the function
 *          (E_OK) : the function done successfully
 *          (E_NOT_OK) : the function has issue to perform this action
 */
Std_ReturnType lcd_4bit_send_command(const chr_lcd_4bit_t * lcd , uint8 command){
    Std_ReturnType retVal = E_NOT_OK;
    if(NULL == lcd){
        retVal = E_NOT_OK;
    }
    else{
        /* R/W pin connected to the GND -> Logic (0) "Hard Wired" */
        /* Write logic (0) to the register select pin to select the instruction register */
        retVal = gpio_pin_write_logic(&(lcd->lcd_rs) , GPIO_LOW);
        /* Send the higher 4 bits of the command through the (4-pins data lines) */
        retVal = lcd_send_4bits(lcd , command >> 4);
        /* Send the enable signal on the "E" pin */
        retVal = lcd_4bit_send_enable_signal(lcd);
        /* Send the higher 4 bits of the command through the (4-pins data lines) */
        retVal = lcd_send_4bits(lcd , command);
        /* Send the enable signal on the "E" pin */
        retVal = lcd_4bit_send_enable_signal(lcd);
        retVal = E_OK;
    }
    return retVal;
}

/**
 * @brief send a character to the LCD to be displayed
 * @param lcd pointer to the LCD configuration module
 * @param data
 * @return status of the function
 *          (E_OK) : the function done successfully
 *          (E_NOT_OK) : the function has issue to perform this action
 */
Std_ReturnType lcd_4bit_send_char_data(const chr_lcd_4bit_t * lcd , uint8 data){
    Std_ReturnType retVal = E_NOT_OK;
    if(NULL == lcd){
        retVal = E_NOT_OK;
    }
    else{
        /* R/W pin connected to the GND -> Logic (0) "Hard Wired" */
        /* Write logic (0) to the register select pin to select the data register */
        retVal = gpio_pin_write_logic(&(lcd->lcd_rs) , GPIO_HIGH);
        /* Send the higher 4 bits of the command through the (4-pins data lines) */
        retVal = lcd_send_4bits(lcd , data >> 4);
        /* Send the enable signal on the "E" pin */
        retVal = lcd_4bit_send_enable_signal(lcd);
        /* Send the higher 4 bits of the command through the (4-pins data lines) */
        retVal = lcd_send_4bits(lcd , data);
        /* Send the enable signal on the "E" pin */
        retVal = lcd_4bit_send_enable_signal(lcd);
        retVal = E_OK;
    }
    return retVal;
}

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
Std_ReturnType lcd_4bit_send_char_data_pos(const chr_lcd_4bit_t * lcd , uint8 row , uint8 column ,uint8 data){
    Std_ReturnType retVal = E_NOT_OK;
    if(NULL == lcd){
        retVal = E_NOT_OK;
    }
    else{
        retVal = lcd_4bit_set_cursor(lcd, row, column);
        retVal = lcd_4bit_send_char_data(lcd , data);
        retVal = E_OK;
    }
    return retVal;
}

/**
 * @brief send a sting to the LCD to be displayed
 * @param lcd pointer to the LCD configuration module
 * @param str
 * @return status of the function
 *          (E_OK) : the function done successfully
 *          (E_NOT_OK) : the function has issue to perform this action
 */
Std_ReturnType lcd_4bit_send_string(const chr_lcd_4bit_t * lcd , uint8 * str){
    Std_ReturnType retVal = E_NOT_OK;
    if((NULL == lcd) || (NULL == str)){
        retVal = E_NOT_OK;
    }
    else{
        while(*str){
            retVal = lcd_4bit_send_char_data(lcd , *str++);
        }
        retVal = E_OK;
    }
    return retVal;
}

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
Std_ReturnType lcd_4bit_send_string_pos(const chr_lcd_4bit_t * lcd , uint8 row , uint8 column ,uint8 * str){
    Std_ReturnType retVal = E_NOT_OK;
    if((NULL == lcd) || (NULL == str)){
        retVal = E_NOT_OK;
    }
    else{
        retVal = lcd_4bit_set_cursor(lcd , row , column);
        while(*str){
            retVal = lcd_4bit_send_char_data(lcd , *str++);
        }
        retVal = E_OK;
    }
    return retVal;
}

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
                                            const uint8 _chr[] , uint8 mem_pos){
    Std_ReturnType retVal = E_NOT_OK;
    if(NULL == lcd){
        retVal = E_NOT_OK;
    }
    else{
        retVal = lcd_4bit_send_command(lcd , (_LCD_CGRAM_START + (mem_pos * 8)));
        for(uint8 counter=0 ; counter <8 ; counter++){
            retVal = lcd_4bit_send_char_data(lcd , _chr[counter]);
        }
        retVal = lcd_4bit_send_char_data_pos(lcd , row , column , mem_pos);
        retVal = E_OK;
    }
    return retVal;
}


/*-----------------------------------------------------------------------------------------*/


/**
 * @brief initialize the LCD pins 
 * @param lcd pointer to the LCD configuration module
 * @return status of the function
 *          (E_OK) : the function done successfully
 *          (E_NOT_OK) : the function has issue to perform this action
 */
Std_ReturnType lcd_8bit_initialize(const chr_lcd_8bit_t * lcd){
    Std_ReturnType retVal = E_NOT_OK;
    if(NULL == lcd){
        retVal = E_NOT_OK;
    }
    else{
        retVal = gpio_pin_initialize(&(lcd->lcd_rs));
        retVal = gpio_pin_initialize(&(lcd->lcd_en));
        for(uint8 counter = 0 ; counter < 8 ; counter++){
            retVal = gpio_pin_initialize(&(lcd->lcd_data[counter]));
        }
        __delay_ms(20);
        retVal = lcd_8bit_send_command(lcd , _LCD_8BIT_MODE_2_LINE);
        __delay_ms(5);
        retVal = lcd_8bit_send_command(lcd , _LCD_8BIT_MODE_2_LINE);
        __delay_us(150);
        retVal = lcd_8bit_send_command(lcd , _LCD_8BIT_MODE_2_LINE);
        
        retVal = lcd_8bit_send_command(lcd , _LCD_CLEAR);
        retVal = lcd_8bit_send_command(lcd , _LCD_RETURN_HOME);
        retVal = lcd_8bit_send_command(lcd , _LCD_ENTRY_MODE_INC_SHIFT_OFF);
        retVal = lcd_8bit_send_command(lcd , _LCD_DISPLAY_ON_UNDERLINE_OFF_CURSOR_OFF);
        retVal = lcd_8bit_send_command(lcd , _LCD_8BIT_MODE_2_LINE);
        retVal = lcd_8bit_send_command(lcd , 0x80);
        retVal = E_OK;
    }
    return retVal;
}

/**
 * @brief sends a command to the LCD
 * @param lcd pointer to the LCD configuration module
 * @param command
 * @return status of the function
 *          (E_OK) : the function done successfully
 *          (E_NOT_OK) : the function has issue to perform this action
 */
Std_ReturnType lcd_8bit_send_command(const chr_lcd_8bit_t * lcd , uint8 command){
    Std_ReturnType retVal = E_NOT_OK;
    if(NULL == lcd){
        retVal = E_NOT_OK;
    }
    else{
        /* R/W pin connected to the GND -> Logic (0) "Hard Wired" */
        /* Write logic (0) to the register select pin to select the instruction register */
        retVal = gpio_pin_write_logic(&(lcd->lcd_rs) , GPIO_LOW);
        /* Send the command through the 8-pins data lines */
        for(uint8 counter=0 ; counter<8 ; counter++){
            retVal = gpio_pin_write_logic(&(lcd->lcd_data[counter]) , (command >> counter) & (uint8)0x01);
        }
        /* Send the enable signal on the "E" pin */
        retVal = lcd_8bit_send_enable_signal(lcd);
        retVal = E_OK;
    }
    return retVal;
}

/**
 * @brief send a character to the LCD to be displayed
 * @param lcd pointer to the LCD configuration module
 * @param data
 * @return status of the function
 *          (E_OK) : the function done successfully
 *          (E_NOT_OK) : the function has issue to perform this action
 */
Std_ReturnType lcd_8bit_send_char_data(const chr_lcd_8bit_t * lcd , uint8 data){
    Std_ReturnType retVal = E_NOT_OK;
    if(NULL == lcd){
        retVal = E_NOT_OK;
    }
    else{
        /* R/W pin connected to the GND -> Logic (0) "Hard Wired" */
        /* Write logic (0) to the register select pin to select the data register */
        retVal = gpio_pin_write_logic(&(lcd->lcd_rs) , GPIO_HIGH);
        /* Send the command through the 8-pins data lines */
        for(uint8 counter=0 ; counter<8 ; counter++){
            retVal = gpio_pin_write_logic(&(lcd->lcd_data[counter]) , (data >> counter) & (uint8)0x01);
        }
        /* Send the enable signal on the "E" pin */
        retVal = lcd_8bit_send_enable_signal(lcd);
        retVal = E_OK;
    }
    return retVal;
}

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
Std_ReturnType lcd_8bit_send_char_data_pos(const chr_lcd_8bit_t * lcd , uint8 row , uint8 column ,uint8 data){
    Std_ReturnType retVal = E_NOT_OK;
    if(NULL == lcd){
        retVal = E_NOT_OK;
    }
    else{
        retVal = lcd_8bit_set_cursor(lcd , row , column);
        retVal = lcd_8bit_send_char_data(lcd , data);
        retVal = E_OK;
    }
    return retVal;
}

/**
 * @brief send a sting to the LCD to be displayed
 * @param lcd pointer to the LCD configuration module
 * @param str
 * @return status of the function
 *          (E_OK) : the function done successfully
 *          (E_NOT_OK) : the function has issue to perform this action
 */
Std_ReturnType lcd_8bit_send_string(const chr_lcd_8bit_t * lcd , uint8 * str){
    Std_ReturnType retVal = E_NOT_OK;
    if((NULL == lcd) || (NULL == str)){
        retVal = E_NOT_OK;
    }
    else{
        while(*str){
            retVal = lcd_8bit_send_char_data(lcd , *str++);
        }
        retVal = E_OK;
    }
    return retVal;
}

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
Std_ReturnType lcd_8bit_send_string_pos(const chr_lcd_8bit_t * lcd , uint8 row , uint8 column ,uint8 * str){
    Std_ReturnType retVal = E_NOT_OK;
    if((NULL == lcd) || (NULL == str)){
        retVal = E_NOT_OK;
    }
    else{
        retVal = lcd_8bit_set_cursor(lcd, row, column);
        while(*str){
            retVal = lcd_8bit_send_char_data(lcd , *str++);
        }
        retVal = E_OK;
    }
    return retVal;
}

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
                                            const uint8 _chr[] , uint8 mem_pos){
    Std_ReturnType retVal = E_NOT_OK;
    if(NULL == lcd){
        retVal = E_NOT_OK;
    }
    else{
        retVal = lcd_8bit_send_command(lcd , (_LCD_CGRAM_START + (mem_pos * 8)));
        for(uint8 counter=0 ; counter <8 ; counter++){
            retVal = lcd_8bit_send_char_data(lcd , _chr[counter]);
        }
        retVal = lcd_8bit_send_char_data_pos(lcd , row , column , mem_pos);
        retVal = E_OK;
    }
    return retVal;
}



/*-----------------------------------------------------------------------------------------*/



/**
 * @brief convert uint8 to string 
 * @param value
 * @param str
 * @return status of the function
 *          (E_OK) : the function done successfully
 *          (E_NOT_OK) : the function has issue to perform this action
 */
Std_ReturnType convert_uint8_to_string(uint8 value , uint8 *str){
    Std_ReturnType retVal = E_NOT_OK;
    uint8 temp_str[4] = {0};
    uint8 data_counter = 0;
    if(NULL == str){
        retVal = E_NOT_OK;
    }
    else{
        memset(str , ' ' , 3);
        str[3] = '\0';
        sprintf((char *)temp_str , "%i" , value);
        while (temp_str[data_counter] != '\0'){
            str[data_counter] = temp_str[data_counter];
            data_counter++;
        }
        retVal = E_OK;
    }
    return retVal;
}

/**
 * @brief convert uint16 to string
 * @param value
 * @param str
 * @return status of the function
 *          (E_OK) : the function done successfully
 *          (E_NOT_OK) : the function has issue to perform this action
 */
Std_ReturnType convert_uint16_to_string(uint16 value , uint8 *str){
    Std_ReturnType retVal = E_NOT_OK;
    uint8 temp_str[6] = {0};
    uint8 data_counter = 0;
    if(NULL == str){
        retVal = E_NOT_OK;
    }
    else{
        memset(str , ' ' , 5);
        str[5] = '\0';
        sprintf((char *)temp_str , "%i" , value);
        while (temp_str[data_counter] != '\0'){
            str[data_counter] = temp_str[data_counter];
            data_counter++;
        }
        retVal = E_OK;
    }
    return retVal;
}

/**
 * @brief convert uint32 to string
 * @param value
 * @param str
 * @return status of the function
 *          (E_OK) : the function done successfully
 *          (E_NOT_OK) : the function has issue to perform this action
 */
Std_ReturnType convert_uint32_to_string(uint32 value , uint8 *str){
    Std_ReturnType retVal = E_NOT_OK;
    uint8 temp_str[11] = {0};
    uint8 data_counter = 0;
    if(NULL == str){
        retVal = E_NOT_OK;
    }
    else{   
        memset(str , ' ' , 10);
        str[10] = '\0';
        sprintf((char *)temp_str , "%i" , value);
        while (temp_str[data_counter] != '\0'){
            str[data_counter] = temp_str[data_counter];
            data_counter++;
        }
        retVal = E_OK;
    }
    return retVal;
}



static Std_ReturnType lcd_send_4bits(const chr_lcd_4bit_t * lcd , uint8 data_command){
    Std_ReturnType retVal = E_NOT_OK;
    if(NULL == lcd){
        retVal = E_NOT_OK;
    }
    else{
        retVal = gpio_pin_write_logic(&(lcd->lcd_data[0]) , (data_command >> 0) & (uint8)0x01);
        retVal = gpio_pin_write_logic(&(lcd->lcd_data[1]) , (data_command >> 1) & (uint8)0x01);
        retVal = gpio_pin_write_logic(&(lcd->lcd_data[2]) , (data_command >> 2) & (uint8)0x01);
        retVal = gpio_pin_write_logic(&(lcd->lcd_data[3]) , (data_command >> 3) & (uint8)0x01);
        retVal = E_OK;
    }
    
    return retVal;
}

static Std_ReturnType lcd_4bit_send_enable_signal(const chr_lcd_4bit_t * lcd){
    Std_ReturnType retVal = E_NOT_OK;
    if(NULL == lcd){
        retVal = E_NOT_OK;
    }
    else{
        retVal = gpio_pin_write_logic(&(lcd->lcd_en) , GPIO_HIGH);
        __delay_us(5);
        retVal = gpio_pin_write_logic(&(lcd->lcd_en) , GPIO_LOW);
        retVal = E_OK;
    }
    return retVal;
}

static Std_ReturnType lcd_8bit_send_enable_signal(const chr_lcd_8bit_t * lcd){
    Std_ReturnType retVal = E_NOT_OK;
    if(NULL == lcd){
        retVal = E_NOT_OK;
    }
    else{
        retVal = gpio_pin_write_logic(&(lcd->lcd_en) , GPIO_HIGH);
        __delay_us(5);
        retVal = gpio_pin_write_logic(&(lcd->lcd_en) , GPIO_LOW);
        retVal = E_OK;
    }
    return retVal;
}

static Std_ReturnType lcd_4bit_set_cursor(const chr_lcd_4bit_t * lcd , uint8 row , uint8 column){
    Std_ReturnType retVal = E_NOT_OK;
    if(NULL == lcd){
        retVal = E_NOT_OK;
    }
    else{
        column--;
        switch(row){
            case ROW1 : retVal = lcd_4bit_send_command(lcd , (0x80 + column)); break;
            case ROW2 : retVal = lcd_4bit_send_command(lcd , (0xc0 + column)); break;
            case ROW3 : retVal = lcd_4bit_send_command(lcd , (0x94 + column)); break;
            case ROW4 : retVal = lcd_4bit_send_command(lcd , (0xd4 + column)); break;
            default : ;
        }
        retVal = E_OK;
    }
    return retVal;
}

static Std_ReturnType lcd_8bit_set_cursor(const chr_lcd_8bit_t * lcd , uint8 row , uint8 column){
    Std_ReturnType retVal = E_NOT_OK;
    if(NULL == lcd){
        retVal = E_NOT_OK;
    }
    else{
        column--;
        switch(row){
            case ROW1 : retVal = lcd_8bit_send_command(lcd , (0x80 + column)); break;
            case ROW2 : retVal = lcd_8bit_send_command(lcd , (0xc0 + column)); break;
            case ROW3 : retVal = lcd_8bit_send_command(lcd , (0x94 + column)); break;
            case ROW4 : retVal = lcd_8bit_send_command(lcd , (0xd4 + column)); break;
            default : ;
        }
        retVal = E_OK;
    }
    return retVal;
}

