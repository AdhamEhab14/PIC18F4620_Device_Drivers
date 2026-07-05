/* 
 * File:   hal_spi.c
 * Author: Adham
 *
 * Created on September 11, 2024, 6:29 PM
 */

#include "hal_spi.h"

#if MSSP_SPI_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
    static void (*SPI_InterruptHandler)(void) = NULL;
#endif

static void MSSP_SPI_Interrupt_Init(const SPI_Config *Config);
static void MSSP_SPI_Msster_Mode_GPIO_PIN_Configurations(const SPI_Config *Config);
static void MSSP_SPI_Slave_Mode_GPIO_PIN_Configurations(const SPI_Config *Config);


/**
 * @brief Initialize SPI module
 * @param Config pointer to the SPI module configurations
 * @return status of the function
 *          (E_OK) : the function done successfully
 *          (E_NOT_OK) : the function has issue to perform this action
 */
Std_ReturnType SPI_Init(const SPI_Config *Config){
    Std_ReturnType retVal = E_NOT_OK;
    
    if(NULL == Config){
        retVal = E_NOT_OK;
    }
    else{
        /* Disable SPI Module */
        SPI_DISABLE_MODULE();
        /* MSSP SPI Mode Select */
        SSPCON1bits.SSPM = Config->spi_mode;
        /* GPIO PIN Configurations */
        if((Config->spi_mode == SPI_MASTER_FOSC_DIV4) || (Config->spi_mode == SPI_MASTER_FOSC_DIV16) ||
            (Config->spi_mode == SPI_MASTER_FOSC_DIV64) || (Config->spi_mode == SPI_MASTER_FOSC_TMR2)){
            MSSP_SPI_Msster_Mode_GPIO_PIN_Configurations(Config);
        }
        else if((Config->spi_mode == SPI_SLAVE_SS_ENABLE) || (Config->spi_mode == SPI_SLAVE_SS_DISABLE)){
            MSSP_SPI_Slave_Mode_GPIO_PIN_Configurations(Config);
        }
        /* Clock Polarity Select */
        SSPCON1bits.CKP = Config->spi_config.ClockPolarity;
        /* SPI Sample Select */
        SSPSTATbits.SMP = Config->spi_config.SampleSelect;
        /* SPI Clock Select */
         SSPSTATbits.CKE = Config->spi_config.ClockSelect;
        /* MSSP SPI Interrupt Configurations*/
        MSSP_SPI_Interrupt_Init(Config);
        /* Enable SPI Module */
        SPI_ENABLE_MODULE();
    }
    
    return retVal;
}

/**
 * @brief DeInitialize SPI module
 * @param Config pointer to the SPI module configurations
 * @return status of the function
 *          (E_OK) : the function done successfully
 *          (E_NOT_OK) : the function has issue to perform this action
 */
Std_ReturnType SPI_DeInit(const SPI_Config *Config){
    Std_ReturnType retVal = E_NOT_OK;
    
    if(NULL == Config){
        retVal = E_NOT_OK;
    }
    else{
        /* Disable SPI Module */
        SPI_DISABLE_MODULE();
/* Interrupt Configurations */        
#if MSSP_SPI_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
        MSSP_SPI_InterruptDisable();
#endif
    }
    
    return retVal;
}

/**
 * @brief Sending a byte by blocking the CPU till the byte is sent
 * @param Config pointer to the SPI module configurations
 * @param _data
 * @return status of the function
 *          (E_OK) : the function done successfully
 *          (E_NOT_OK) : the function has issue to perform this action
 */
Std_ReturnType SPI_Send_Byte(const SPI_Config *Config, const uint8 _data){
    Std_ReturnType retVal = E_NOT_OK;
    
    if(NULL == Config){
        retVal = E_NOT_OK;
    }
    else{
        SSPBUF = _data;
        while(!(PIR1bits.SSPIF)); /* Waiting to transmit */
        PIR1bits.SSPIF = 0;
    }
    
    return retVal;
}

/**
 * @brief Reading a byte without blocking the CPU till the byte is read
 * @param Config pointer to the SPI module configurations
 * @param _data
 * @return status of the function
 *          (E_OK) : the function done successfully
 *          (E_NOT_OK) : the function has issue to perform this action
 */
Std_ReturnType SPI_Read_Byte(const SPI_Config *Config, uint8 *_data){
    Std_ReturnType retVal = E_NOT_OK;
    
    if((NULL == Config) || (NULL == _data)){
        retVal = E_NOT_OK;
    }
    else{
        while(SSPSTATbits.BF == 0); /* Receive not complete, SSPBUF is empty */
        *_data = SSPBUF;
    }
    
    return retVal;
}

/**
 * @brief Sending a string without blocking the CPU till the byte is sent
 * @param Config pointer to the SPI module configurations
 * @param _data
 * @return status of the function
 *          (E_OK) : the function done successfully
 *          (E_NOT_OK) : the function has issue to perform this action
 */
Std_ReturnType SPI_Send_Byte_NonBlocking(const SPI_Config *Config, const uint8 _data){
    Std_ReturnType retVal = E_NOT_OK;
    
    if(NULL == Config){
        retVal = E_NOT_OK;
    }
    else{
        SSPBUF = _data;
        if(0 == PIR1bits.SSPIF){
            PIR1bits.SSPIF = 0;
            retVal = E_OK;
        }
        else{
            retVal = E_NOT_OK;
        }
    }
    
    return retVal;
}

/**
 * @brief Reading a byte without blocking the CPU till the byte is read
 * @param Config pointer to the SPI module configurations
 * @param _data
 * @return status of the function
 *          (E_OK) : the function done successfully
 *          (E_NOT_OK) : the function has issue to perform this action
 */
Std_ReturnType SPI_Read_Byte_NonBlocking(const SPI_Config *Config, uint8 *_data){
    Std_ReturnType retVal = E_NOT_OK;
    
    if((NULL == Config) || (NULL == _data)){
        retVal = E_NOT_OK;
    }
    else{
        if(SSPSTATbits.BF == 0){
            *_data = SSPBUF;
            retVal = E_OK;
        }
        else{
            retVal = E_NOT_OK;
        }
    }
    
    return retVal;
}

/*--------------------------------------- ISR -----------------------------------------*/

void MSSP_SPI_ISR(void){
#if MSSP_SPI_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
    MSSP_SPI_InterruptFlagClear();
    if(SPI_InterruptHandler){
        SPI_InterruptHandler();
    }
#endif
}

/*--------------------------------------- Helper Functions -----------------------------------------*/


static void MSSP_SPI_Interrupt_Init(const SPI_Config *Config){
/* Interrupt Configurations */        
#if MSSP_SPI_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
    MSSP_SPI_InterruptEnable();
    MSSP_SPI_InterruptFlagClear();
    SPI_InterruptHandler = Config->MSSP_SPI_InterruptHandler;
/* Interrupt Priority Configurations */
#if INTERRUPT_PRIORITY_LEVELS_ENABLE==INTERRUPT_FEATURE_ENABLE 
    INTERRUPT_PriorityLevelsEnable();
    if(INTERRUPT_HIGH_PRIORITY == Config->priority){
        /* Enables all high-priority interrupts */
        INTERRUPT_GlobalInterruptHighEnable();
        MSSP_SPI_HighPrioritySet();
    }
    else if(INTERRUPT_LOW_PRIORITY == Config->priority){
        /* Enables all unmasked peripheral interrupts */
        INTERRUPT_GlobalInterruptLowEnable();
        MSSP_SPI_LowPrioritySet();
    }
    else{ /* Nothing */ }
#else
    INTERRUPT_GlobalInterruptEnable();
    INTERRUPT_PeripheralInterruptEnable();
#endif      
#endif        
}

static void MSSP_SPI_Msster_Mode_GPIO_PIN_Configurations(const SPI_Config *Config){
    Std_ReturnType retVal = E_NOT_OK;
    pin_config_t SPI_SDO = {.port = PORTC_INDEX, .pin = GPIO_PIN5, .direction = GPIO_DIRECTION_OUTPUT};
    pin_config_t SPI_SDI = {.port = PORTC_INDEX, .pin = GPIO_PIN4, .direction = GPIO_DIRECTION_INPUT};
    pin_config_t SPI_CLK = {.port = PORTC_INDEX, .pin = GPIO_PIN3, .direction = GPIO_DIRECTION_OUTPUT};
    
    retVal = gpio_pin_direction_initialize(&SPI_SDO); 
    retVal = gpio_pin_direction_initialize(&SPI_SDI);
    retVal = gpio_pin_direction_initialize(&SPI_CLK);
}

static void MSSP_SPI_Slave_Mode_GPIO_PIN_Configurations(const SPI_Config *Config){
    Std_ReturnType retVal = E_NOT_OK;
    pin_config_t SPI_SDO = {.port = PORTC_INDEX, .pin = GPIO_PIN5, .direction = GPIO_DIRECTION_OUTPUT};
    pin_config_t SPI_SDI = {.port = PORTC_INDEX, .pin = GPIO_PIN4, .direction = GPIO_DIRECTION_INPUT};
    pin_config_t SPI_CLK = {.port = PORTC_INDEX, .pin = GPIO_PIN3, .direction = GPIO_DIRECTION_INPUT};
    pin_config_t SPI_SS = {.port = PORTA_INDEX, .pin = GPIO_PIN5, .direction = GPIO_DIRECTION_INPUT};
    
    retVal = gpio_pin_direction_initialize(&SPI_SDO); 
    retVal = gpio_pin_direction_initialize(&SPI_SDI);
    retVal = gpio_pin_direction_initialize(&SPI_CLK); 
    
    if(SPI_SLAVE_SS_ENABLE == Config->spi_mode){
        retVal = gpio_pin_direction_initialize(&SPI_SS);
    }
}