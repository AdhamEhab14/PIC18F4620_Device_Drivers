/*
 * CortexM3_Core_DMA.h
 *
 *  Created on: Jul 12, 2026
 *      Author: Adham
 */

#ifndef DMA_CORTEXM3_CORE_DMA_H_
#define DMA_CORTEXM3_CORE_DMA_H_

/* ---------------------------------------- Includes Start ---------------------------------------- */

#include "Common/Std_Types.h"
#include "Common/HAL_Status.h"

/* ---------------------------------------- Includes End ------------------------------------------ */


/* ---------------------------------------- Macro Declaration Start ------------------------------- */

/* Peripheral memory map. This device (Medium-density STM32F103RB) has DMA1 only - no DMA2 (High-density/
 * Connectivity-line only) */
#define DMA1_BASE				(0x40020000UL)
#define DMA1					((DMA_Type*)DMA1_BASE)				// Pointer to the base address of the DMA1 registers memory block

/* Channel pointer macros - DMA1 has 7 channels (index 0-6 internally, numbered 1-7 on the datasheet) */
#define DMA1_Channel1			(&(DMA1->CHANNEL[0]))
#define DMA1_Channel2			(&(DMA1->CHANNEL[1]))
#define DMA1_Channel3			(&(DMA1->CHANNEL[2]))
#define DMA1_Channel4			(&(DMA1->CHANNEL[3]))
#define DMA1_Channel5			(&(DMA1->CHANNEL[4]))
#define DMA1_Channel6			(&(DMA1->CHANNEL[5]))
#define DMA1_Channel7			(&(DMA1->CHANNEL[6]))


/* ******************* Bit definition for DMA_CCRx register ******************* */
#define DMA_CCR_EN_Pos				(0U)
#define DMA_CCR_EN_Msk				(0x1UL << DMA_CCR_EN_Pos)			/*!< 0x00000001 - Channel enable */
#define DMA_CCR_TCIE_Pos			(1U)
#define DMA_CCR_TCIE_Msk			(0x1UL << DMA_CCR_TCIE_Pos)			/*!< 0x00000002 - Transfer complete interrupt enable */
#define DMA_CCR_HTIE_Pos			(2U)
#define DMA_CCR_HTIE_Msk			(0x1UL << DMA_CCR_HTIE_Pos)			/*!< 0x00000004 - Half transfer interrupt enable */
#define DMA_CCR_TEIE_Pos			(3U)
#define DMA_CCR_TEIE_Msk			(0x1UL << DMA_CCR_TEIE_Pos)			/*!< 0x00000008 - Transfer error interrupt enable */
#define DMA_CCR_DIR_Pos				(4U)
#define DMA_CCR_DIR_Msk				(0x1UL << DMA_CCR_DIR_Pos)			/*!< 0x00000010 - Data transfer direction */
#define DMA_CCR_CIRC_Pos			(5U)
#define DMA_CCR_CIRC_Msk			(0x1UL << DMA_CCR_CIRC_Pos)			/*!< 0x00000020 - Circular mode */
#define DMA_CCR_PINC_Pos			(6U)
#define DMA_CCR_PINC_Msk			(0x1UL << DMA_CCR_PINC_Pos)			/*!< 0x00000040 - Peripheral increment mode */
#define DMA_CCR_MINC_Pos			(7U)
#define DMA_CCR_MINC_Msk			(0x1UL << DMA_CCR_MINC_Pos)			/*!< 0x00000080 - Memory increment mode */
#define DMA_CCR_PSIZE_Pos			(8U)
#define DMA_CCR_PSIZE_Msk			(0x3UL << DMA_CCR_PSIZE_Pos)			/*!< 0x00000300 - Peripheral size */
#define DMA_CCR_MSIZE_Pos			(10U)
#define DMA_CCR_MSIZE_Msk			(0x3UL << DMA_CCR_MSIZE_Pos)			/*!< 0x00000C00 - Memory size */
#define DMA_CCR_PL_Pos				(12U)
#define DMA_CCR_PL_Msk				(0x3UL << DMA_CCR_PL_Pos)			/*!< 0x00003000 - Priority level */
#define DMA_CCR_MEM2MEM_Pos			(14U)
#define DMA_CCR_MEM2MEM_Msk			(0x1UL << DMA_CCR_MEM2MEM_Pos)		/*!< 0x00004000 - Memory to memory mode */


/* ******************* Transfer direction ******************* */
#define DMA_DIR_PERIPH_TO_MEM		(0x00000000U)						// DIR = 0, MEM2MEM = 0
#define DMA_DIR_MEM_TO_PERIPH		(DMA_CCR_DIR_Msk)					// DIR = 1, MEM2MEM = 0
#define DMA_DIR_MEM_TO_MEM			(DMA_CCR_MEM2MEM_Msk)				// DIR = 0, MEM2MEM = 1 (CPAR=source, CMAR=destination)

/* ******************* Peripheral / Memory increment mode ******************* */
#define DMA_PINC_DISABLE			(0x00000000U)
#define DMA_PINC_ENABLE				(DMA_CCR_PINC_Msk)
#define DMA_MINC_DISABLE			(0x00000000U)
#define DMA_MINC_ENABLE				(DMA_CCR_MINC_Msk)

/* ******************* Peripheral data alignment (transfer size on the peripheral side) ******************* */
#define DMA_PDATAALIGN_BYTE			(0x00000000U)
#define DMA_PDATAALIGN_HALFWORD		(0x1UL << DMA_CCR_PSIZE_Pos)
#define DMA_PDATAALIGN_WORD			(0x2UL << DMA_CCR_PSIZE_Pos)

/* ******************* Memory data alignment (transfer size on the memory side) ******************* */
#define DMA_MDATAALIGN_BYTE			(0x00000000U)
#define DMA_MDATAALIGN_HALFWORD		(0x1UL << DMA_CCR_MSIZE_Pos)
#define DMA_MDATAALIGN_WORD			(0x2UL << DMA_CCR_MSIZE_Pos)

/* ******************* Transfer mode ******************* */
#define DMA_MODE_NORMAL				(0x00000000U)						// One-shot transfer
#define DMA_MODE_CIRCULAR			(DMA_CCR_CIRC_Msk)					// Auto-reloads CNDTR/CMAR when it reaches 0

/* ******************* Channel priority ******************* */
#define DMA_PRIORITY_LOW				(0x0UL << DMA_CCR_PL_Pos)
#define DMA_PRIORITY_MEDIUM			(0x1UL << DMA_CCR_PL_Pos)
#define DMA_PRIORITY_HIGH			(0x2UL << DMA_CCR_PL_Pos)
#define DMA_PRIORITY_VERYHIGH		(0x3UL << DMA_CCR_PL_Pos)

/* ******************* Interrupt enable selectors (for DMA_InitType's IT fields) ******************* */
#define DMA_IT_DISABLE				(0x00000000U)
#define DMA_TCIE_ENABLE				(DMA_CCR_TCIE_Msk)
#define DMA_HTIE_ENABLE				(DMA_CCR_HTIE_Msk)
#define DMA_TEIE_ENABLE				(DMA_CCR_TEIE_Msk)

/* ******************* Flag selectors (for DMA_GetFlagStatus / DMA_ClearFlag; combined with a channel number 1-7) ******************* */
#define DMA_FLAG_GIF					(0x1U)								// Global interrupt flag
#define DMA_FLAG_TCIF				(0x2U)								// Transfer complete flag
#define DMA_FLAG_HTIF				(0x4U)								// Half transfer flag
#define DMA_FLAG_TEIF				(0x8U)								// Transfer error flag

/* ---------------------------------------- Macro Declaration End --------------------------------- */


/* ---------------------------------------- Macro Function Declarations Start --------------------- */

/* ---------------------------------------- Macro Function Declarations End ----------------------- */


/* ---------------------------------------- Data Type Declaration Start --------------------------- */

/* One DMA channel's register block (CCR/CNDTR/CPAR/CMAR + one reserved word, 0x14 bytes wide) */
typedef struct{
	volatile uint32_t CCR;				// Channel Configuration Register
	volatile uint32_t CNDTR;			// Channel Number of Data to Transfer Register
	volatile uint32_t CPAR;			// Channel Peripheral Address Register
	volatile uint32_t CMAR;			// Channel Memory Address Register
	uint32_t RESERVED;
}DMA_Channel_Type;

/* Creating a memory block for the DMA controller: 2 global registers + 7 channel blocks */
typedef struct{
	volatile uint32_t ISR;				// Offset: 0x00 (R/ ) Interrupt Status Register
	volatile uint32_t IFCR;			// Offset: 0x04 ( /W) Interrupt Flag Clear Register
	DMA_Channel_Type  CHANNEL[7];		// Offset: 0x08.. Channel 1-7 register blocks (index 0 = Channel1)
}DMA_Type;

/* DMA channel configuration structure (filled by the user, passed to DMA_Init) */
typedef struct{
	uint32_t Direction;				// Transfer direction. A value of @ref DMA_DIR_x
	uint32_t PeriphInc;				// Peripheral address increment. A value of @ref DMA_PINC_x
	uint32_t MemInc;					// Memory address increment. A value of @ref DMA_MINC_x
	uint32_t PeriphDataAlign;			// Peripheral transfer size. A value of @ref DMA_PDATAALIGN_x
	uint32_t MemDataAlign;				// Memory transfer size. A value of @ref DMA_MDATAALIGN_x
	uint32_t Mode;						// Normal or circular. A value of @ref DMA_MODE_x
	uint32_t Priority;					// Arbitration priority. A value of @ref DMA_PRIORITY_x
	uint32_t TCInterrupt;				// Transfer-complete interrupt. DMA_IT_DISABLE or DMA_TCIE_ENABLE
	uint32_t HTInterrupt;				// Half-transfer interrupt. DMA_IT_DISABLE or DMA_HTIE_ENABLE
	uint32_t TEInterrupt;				// Transfer-error interrupt. DMA_IT_DISABLE or DMA_TEIE_ENABLE
}DMA_InitType;

/* ---------------------------------------- Data Type Declaration End ----------------------------- */


/* ---------------------------------------- Software Interfaces Declaration Start ----------------- */

/**
 * brief	Configures a DMA channel according to the parameters in DMA_InitStruct
 * details	Writes the CCR register (direction, increment modes, data sizes, transfer mode, priority, and
 * 			interrupt enables). Does not touch CPAR/CNDTR/CMAR or the EN bit - use DMA_Start for that.
 * param	DMAy_Channelx pointer to the channel to configure (DMA1_Channel1 .. DMA1_Channel7)
 * param	DMA_InitStruct pointer to a DMA_InitType structure that contains the channel configuration
 * return	HAL_OK    the channel was configured successfully
 * return	HAL_ERROR either pointer is NULL
 * return	HAL_BUSY  the channel is currently enabled (CCR fields cannot be changed while EN=1; call DMA_Stop first)
 * note		RCC_DMA1_CLK_ENABLE() must be called before using any DMA1 channel
 */
HAL_StatusType DMA_Init(DMA_Channel_Type *DMAy_Channelx, DMA_InitType *DMA_InitStruct);

/**
 * brief	Programs the transfer addresses/length and starts a DMA channel
 * details	Writes CPAR, CMAR, and CNDTR, then sets the EN bit. CPAR is always the peripheral-side address
 * 			and CMAR the memory-side address, except in DMA_DIR_MEM_TO_MEM mode where CPAR is the source
 * 			and CMAR is the destination.
 * param	DMAy_Channelx pointer to the channel to start (DMA1_Channel1 .. DMA1_Channel7)
 * param	PeriphAddress the peripheral-side address (or source address, for memory-to-memory)
 * param	MemAddress the memory-side address (or destination address, for memory-to-memory)
 * param	DataLength number of data items to transfer (1-65535; 0 is rejected)
 * return	HAL_OK    the channel was started successfully
 * return	HAL_ERROR the pointer is NULL, or DataLength is 0
 * return	HAL_BUSY  the channel is already enabled - call DMA_Stop first
 * note		DMA_Init must be called first to configure the channel
 */
HAL_StatusType DMA_Start(DMA_Channel_Type *DMAy_Channelx, uint32_t PeriphAddress, uint32_t MemAddress, uint16_t DataLength);

/**
 * brief	Stops a DMA channel
 * details	Clears the EN bit. Safe to call on an already-stopped channel.
 * param	DMAy_Channelx pointer to the channel to stop (DMA1_Channel1 .. DMA1_Channel7)
 */
void DMA_Stop(DMA_Channel_Type *DMAy_Channelx);

/**
 * brief	Checks whether a DMA channel is currently enabled
 * param	DMAy_Channelx pointer to the channel to check (DMA1_Channel1 .. DMA1_Channel7)
 * return	1 the channel is enabled (transfer in progress, for normal mode)
 * return	0 the channel is disabled
 */
boolean DMA_IsEnabled(DMA_Channel_Type *DMAy_Channelx);

/**
 * brief	Reads the number of data items left to transfer
 * details	CNDTR counts down from the value given to DMA_Start toward 0; in circular mode it reloads
 * 			back to the original count once it reaches 0
 * param	DMAy_Channelx pointer to the channel to read (DMA1_Channel1 .. DMA1_Channel7)
 * return	Remaining data item count
 */
uint16_t DMA_GetRemainingDataCount(DMA_Channel_Type *DMAy_Channelx);

/**
 * brief	Checks a DMA1 interrupt/status flag for a given channel
 * param	ChannelNumber the channel number, 1-7 (matches the datasheet channel numbering)
 * param	Flag the flag to check. A value of @ref DMA_FLAG_x
 * return	1 the flag is set
 * return	0 the flag is clear
 */
boolean DMA_GetFlagStatus(uint32_t ChannelNumber, uint32_t Flag);

/**
 * brief	Clears a DMA1 interrupt/status flag for a given channel
 * param	ChannelNumber the channel number, 1-7 (matches the datasheet channel numbering)
 * param	Flag the flag to clear. A value of @ref DMA_FLAG_x
 */
void DMA_ClearFlag(uint32_t ChannelNumber, uint32_t Flag);

/* ---------------------------------------- Software Interfaces Declaration End ------------------- */

#endif /* DMA_CORTEXM3_CORE_DMA_H_ */
