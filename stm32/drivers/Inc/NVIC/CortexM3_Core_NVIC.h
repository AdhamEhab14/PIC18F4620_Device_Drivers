/*
 * CortexM4_Core_NVIC.h
 *
 *  Created on: May 18, 2026
 *      Author: Adham
 */

#ifndef CORTEXM3_CORE_NVIC_H_
#define CORTEXM3_CORE_NVIC_H_

/* ---------------------------------------- Includes Start ---------------------------------------- */

#include "Common/Std_Types.h"

/* ---------------------------------------- Includes End ------------------------------------------ */


/* ---------------------------------------- Macro Declaration Start ------------------------------- */

/* Memory Mapping of the Core Hardware */
#define NVIC_BASE		(0xE000E100)
#define NVIC			((NVIC_Type*)NVIC_BASE)			// Pointer to the base address of the NVIC registers memory block
#define NVIC_PRIO_BITS	4U								// STM32 uses 4 bits for the priority levels

/* ---------------------------------------- Macro Declaration End --------------------------------- */


/* ---------------------------------------- Macro Function Declarations Start --------------------- */

/* ---------------------------------------- Macro Function Declarations End ----------------------- */


/* ---------------------------------------- Data Type Declaration Start --------------------------- */

/* Creating a memory block for the NVIC Registers */
typedef struct{
	volatile uint32_t ISER[8U];				// Offset: 0x000 (R/W) Interrupt Set Enable Register
	uint32_t RESERVED0[24U];
	volatile uint32_t ICER[8U];				// Offset: 0x080 (R/W) Interrupt Clear Enable Register
	uint32_t RESERVED1[24U];
	volatile uint32_t ISPR[8U];				// Offset: 0x100 (R/W) Interrupt Set Pending Register
	uint32_t RESERVED2[24U];
	volatile uint32_t ICPR[8U];				// Offset: 0x180 (R/W) Interrupt Clear Pending Register
	uint32_t RESERVED3[24U];
	volatile uint32_t IABR[8U];				// Offset: 0x200 (R/W) Interrupt Active Bit Register
	uint32_t RESERVED4[56U];
	volatile uint8_t IP[240U];				// Offset: 0x300 (R/W) Interrupt Priority Register
	uint32_t RESERVED5[644U];
	volatile uint32_t STIR;					// Offset: 0xE00 (W)   Software Trigger Interrupt Register
}NVIC_Type;

typedef enum
{
    /* ------------------- Cortex-M3 Processor Exceptions ------------------- */
    NonMaskableInt_IRQn         = -14,  /*!< 2 Non Maskable Interrupt                             */
    HardFault_IRQn              = -13,  /*!< 3 Cortex-M3 Hard Fault Interrupt                     */
    MemoryManagement_IRQn       = -12,  /*!< 4 Cortex-M3 Memory Management Interrupt              */
    BusFault_IRQn               = -11,  /*!< 5 Cortex-M3 Bus Fault Interrupt                      */
    UsageFault_IRQn             = -10,  /*!< 6 Cortex-M3 Usage Fault Interrupt                    */
    SVCall_IRQn                 = -5,   /*!< 11 Cortex-M3 SV Call Interrupt                       */
    DebugMonitor_IRQn           = -4,   /*!< 12 Cortex-M3 Debug Monitor Interrupt                 */
    PendSV_IRQn                 = -2,   /*!< 14 Cortex-M3 Pend SV Interrupt                       */
    SysTick_IRQn                = -1,   /*!< 15 Cortex-M3 System Tick Interrupt                   */

    /* ------------------- STM32 Connectivity Line Interrupts --------------- */
    WWDG_IRQn                   = 0,    /*!< Window WatchDog Interrupt                            */
    PVD_IRQn                    = 1,    /*!< PVD through EXTI Line detection Interrupt            */
    TAMPER_IRQn                 = 2,    /*!< Tamper Interrupt                                     */
    RTC_IRQn                    = 3,    /*!< RTC global Interrupt                                 */
    FLASH_IRQn                  = 4,    /*!< FLASH global Interrupt                               */
    RCC_IRQn                    = 5,    /*!< RCC global Interrupt                                 */
    EXTI0_IRQn                  = 6,    /*!< EXTI Line0 Interrupt                                 */
    EXTI1_IRQn                  = 7,    /*!< EXTI Line1 Interrupt                                 */
    EXTI2_IRQn                  = 8,    /*!< EXTI Line2 Interrupt                                 */
    EXTI3_IRQn                  = 9,    /*!< EXTI Line3 Interrupt                                 */
    EXTI4_IRQn                  = 10,   /*!< EXTI Line4 Interrupt                                 */
    DMA1_Channel1_IRQn          = 11,   /*!< DMA1 Channel 1 global Interrupt                      */
    DMA1_Channel2_IRQn          = 12,   /*!< DMA1 Channel 2 global Interrupt                      */
    DMA1_Channel3_IRQn          = 13,   /*!< DMA1 Channel 3 global Interrupt                      */
    DMA1_Channel4_IRQn          = 14,   /*!< DMA1 Channel 4 global Interrupt                      */
    DMA1_Channel5_IRQn          = 15,   /*!< DMA1 Channel 5 global Interrupt                      */
    DMA1_Channel6_IRQn          = 16,   /*!< DMA1 Channel 6 global Interrupt                      */
    DMA1_Channel7_IRQn          = 17,   /*!< DMA1 Channel 7 global Interrupt                      */
    ADC1_2_IRQn                 = 18,   /*!< ADC1 and ADC2 global Interrupt                       */
    CAN1_TX_IRQn                = 19,   /*!< CAN1 TX Interrupts                                   */
    CAN1_RX0_IRQn               = 20,   /*!< CAN1 RX0 Interrupts                                  */
    CAN1_RX1_IRQn               = 21,   /*!< CAN1 RX1 Interrupt                                   */
    CAN1_SCE_IRQn               = 22,   /*!< CAN1 SCE Interrupt                                   */
    EXTI9_5_IRQn                = 23,   /*!< EXTI Line[9:5] Interrupts                            */
    TIM1_BRK_IRQn               = 24,   /*!< TIM1 Break Interrupt                                 */
    TIM1_UP_IRQn                = 25,   /*!< TIM1 Update Interrupt                                */
    TIM1_TRG_COM_IRQn           = 26,   /*!< TIM1 Trigger and Commutation Interrupt               */
    TIM1_CC_IRQn                = 27,   /*!< TIM1 Capture Compare Interrupt                       */
    TIM2_IRQn                   = 28,   /*!< TIM2 global Interrupt                                */
    TIM3_IRQn                   = 29,   /*!< TIM3 global Interrupt                                */
    TIM4_IRQn                   = 30,   /*!< TIM4 global Interrupt                                */
    I2C1_EV_IRQn                = 31,   /*!< I2C1 Event Interrupt                                 */
    I2C1_ER_IRQn                = 32,   /*!< I2C1 Error Interrupt                                 */
    I2C2_EV_IRQn                = 33,   /*!< I2C2 Event Interrupt                                 */
    I2C2_ER_IRQn                = 34,   /*!< I2C2 Error Interrupt                                 */
    SPI1_IRQn                   = 35,   /*!< SPI1 global Interrupt                                */
    SPI2_IRQn                   = 36,   /*!< SPI2 global Interrupt                                */
    USART1_IRQn                 = 37,   /*!< USART1 global Interrupt                              */
    USART2_IRQn                 = 38,   /*!< USART2 global Interrupt                              */
    USART3_IRQn                 = 39,   /*!< USART3 global Interrupt                              */
    EXTI15_10_IRQn              = 40,   /*!< EXTI Line[15:10] Interrupts                          */
    RTCAlarm_IRQn               = 41,   /*!< RTC Alarm through EXTI Line Interrupt                */
    OTG_FS_WKUP_IRQn            = 42,   /*!< USB OTG FS Wakeup through EXTI Line Interrupt        */
    /* Reserved                 = 43 to 49  (0x00EC - 0x0104)                               */
    TIM5_IRQn                   = 50,   /*!< TIM5 global Interrupt                                */
    SPI3_IRQn                   = 51,   /*!< SPI3 global Interrupt                                */
    UART4_IRQn                  = 52,   /*!< UART4 global Interrupt                               */
    UART5_IRQn                  = 53,   /*!< UART5 global Interrupt                               */
    TIM6_IRQn                   = 54,   /*!< TIM6 global Interrupt                                */
    TIM7_IRQn                   = 55,   /*!< TIM7 global Interrupt                                */
    DMA2_Channel1_IRQn          = 56,   /*!< DMA2 Channel 1 global Interrupt                      */
    DMA2_Channel2_IRQn          = 57,   /*!< DMA2 Channel 2 global Interrupt                      */
    DMA2_Channel3_IRQn          = 58,   /*!< DMA2 Channel 3 global Interrupt                      */
    DMA2_Channel4_IRQn          = 59,   /*!< DMA2 Channel 4 global Interrupt                      */
    DMA2_Channel5_IRQn          = 60,   /*!< DMA2 Channel 5 global Interrupt                      */
    ETH_IRQn                    = 61,   /*!< Ethernet global Interrupt                            */
    ETH_WKUP_IRQn               = 62,   /*!< Ethernet Wakeup through EXTI Line Interrupt          */
    CAN2_TX_IRQn                = 63,   /*!< CAN2 TX Interrupts                                   */
    CAN2_RX0_IRQn               = 64,   /*!< CAN2 RX0 Interrupts                                  */
    CAN2_RX1_IRQn               = 65,   /*!< CAN2 RX1 Interrupt                                   */
    CAN2_SCE_IRQn               = 66,   /*!< CAN2 SCE Interrupt                                   */
    OTG_FS_IRQn                 = 67,   /*!< USB OTG FS global Interrupt                          */
} IRQn_Type;

/* ---------------------------------------- Data Type Declaration End ----------------------------- */


/* ---------------------------------------- Software Interfaces Declaration Start ----------------- */

/**
 * brief	Enalbe interrupt
 * details	Enables a device specific interrupt in the NVIC interrupt controller
 * param	IRQn device specific interrupt number
 * note		IRQn can't be negative
 */
void NVIC_EnableIRQ(IRQn_Type IRQn);
/**
 * brief	Disable interrupt
 * details	Disable a device specific interrupt in the NVIC interrupt controller
 * param	IRQn device specific interrupt number
 * note		IRQn can't be negative
 */
void NVIC_DisableIRQ(IRQn_Type IRQn);
/**
 * brief	Set interrupt pending
 * details	Set pending bit for a device specific interrupt in the NVIC interrupt controller
 * param	IRQn device specific interrupt number
 * note		IRQn can't be negative
 */
void NVIC_SetPendingIRQ(IRQn_Type IRQn);
/**
 * brief	Clear interrupt pending
 * details	Clear pending bit for a device specific interrupt in the NVIC interrupt controller
 * param	IRQn device specific interrupt number
 * note		IRQn can't be negative
 */
void NVIC_ClearPendingIRQ(IRQn_Type IRQn);
/**
 * brief	Get active interrupt
 * details	Reads the active register in the NVIC and returns the active bit for the device specific interrupt in the NVIC interrupt contoller
 * param	IRQn device specific interrupt number
 * return	0 Interrupt status is not active
 * return	1 Interrupt status is active
 * note		IRQn can't be negative
 */
uint32_t NVIC_GetActive(IRQn_Type IRQn);
/**
 * brief	Set interrupt priority
 * details	Sets the priority of a device specific interrupt or a processor exception.
 * 			The interrupt number can be positive to specify a device specific interrupt or negative to specify processor exception
 * param	IRQn interrupt number
 * param	Priority to set
 */
void NVIC_SetPriority(IRQn_Type IRQn, uint32_t priority);
/**
 * brief	Get interrupt priority
 * details	Reads the priority of a device specific interrupt or a processor exception.
 * 			The interrupt number can be positive to specify a device specific interrupt or negative to specify processor exception
 * param	IRQn interrupt number
 * return	Interrupt priority
 */
uint32_t NVIC_GetPriority(IRQn_Type IRQn);
/* ---------------------------------------- Software Interfaces Declaration End ------------------- */

#endif /* CORTEXM3_CORE_NVIC_H_ */
