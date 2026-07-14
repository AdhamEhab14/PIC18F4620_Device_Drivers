/*
 * CortexM3_Core_DMA.c
 *
 *  Created on: Jul 12, 2026
 *      Author: Adham
 */

#include "DMA/CortexM3_Core_DMA.h"


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
HAL_StatusType DMA_Init(DMA_Channel_Type *DMAy_Channelx, DMA_InitType *DMA_InitStruct){

	uint32_t ccr = 0U;

	if((DMAy_Channelx == NULL_PTR) || (DMA_InitStruct == NULL_PTR)){
		return HAL_ERROR;
	}else{
		/* Do Nothing */
	}

	if((DMAy_Channelx->CCR & DMA_CCR_EN_Msk) != 0U){
		return HAL_BUSY;
	}else{
		/* Do Nothing */
	}

	ccr = DMA_InitStruct->Direction
			| DMA_InitStruct->PeriphInc
			| DMA_InitStruct->MemInc
			| DMA_InitStruct->PeriphDataAlign
			| DMA_InitStruct->MemDataAlign
			| DMA_InitStruct->Mode
			| DMA_InitStruct->Priority
			| DMA_InitStruct->TCInterrupt
			| DMA_InitStruct->HTInterrupt
			| DMA_InitStruct->TEInterrupt;

	DMAy_Channelx->CCR = ccr;			// EN stays 0 - DMA_Start enables the channel

	return HAL_OK;
}

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
HAL_StatusType DMA_Start(DMA_Channel_Type *DMAy_Channelx, uint32_t PeriphAddress, uint32_t MemAddress, uint16_t DataLength){

	if((DMAy_Channelx == NULL_PTR) || (DataLength == 0U)){
		return HAL_ERROR;
	}else{
		/* Do Nothing */
	}

	if((DMAy_Channelx->CCR & DMA_CCR_EN_Msk) != 0U){
		return HAL_BUSY;
	}else{
		/* Do Nothing */
	}

	DMAy_Channelx->CPAR  = PeriphAddress;
	DMAy_Channelx->CMAR  = MemAddress;
	DMAy_Channelx->CNDTR = DataLength;

	DMAy_Channelx->CCR |= DMA_CCR_EN_Msk;

	return HAL_OK;
}

/**
 * brief	Stops a DMA channel
 * details	Clears the EN bit. Safe to call on an already-stopped channel.
 * param	DMAy_Channelx pointer to the channel to stop (DMA1_Channel1 .. DMA1_Channel7)
 */
void DMA_Stop(DMA_Channel_Type *DMAy_Channelx){
	DMAy_Channelx->CCR &= ~DMA_CCR_EN_Msk;
}

/**
 * brief	Checks whether a DMA channel is currently enabled
 * param	DMAy_Channelx pointer to the channel to check (DMA1_Channel1 .. DMA1_Channel7)
 * return	1 the channel is enabled (transfer in progress, for normal mode)
 * return	0 the channel is disabled
 */
boolean DMA_IsEnabled(DMA_Channel_Type *DMAy_Channelx){
	return ((DMAy_Channelx->CCR & DMA_CCR_EN_Msk) != 0U) ? 1U : 0U;
}

/**
 * brief	Reads the number of data items left to transfer
 * details	CNDTR counts down from the value given to DMA_Start toward 0; in circular mode it reloads
 * 			back to the original count once it reaches 0
 * param	DMAy_Channelx pointer to the channel to read (DMA1_Channel1 .. DMA1_Channel7)
 * return	Remaining data item count
 */
uint16_t DMA_GetRemainingDataCount(DMA_Channel_Type *DMAy_Channelx){
	return (uint16_t)(DMAy_Channelx->CNDTR & 0x0000FFFFU);
}

/**
 * brief	Checks a DMA1 interrupt/status flag for a given channel
 * param	ChannelNumber the channel number, 1-7 (matches the datasheet channel numbering)
 * param	Flag the flag to check. A value of @ref DMA_FLAG_x
 * return	1 the flag is set
 * return	0 the flag is clear
 */
boolean DMA_GetFlagStatus(uint32_t ChannelNumber, uint32_t Flag){

	uint32_t shift = (ChannelNumber - 1U) * 4U;

	return ((DMA1->ISR & (Flag << shift)) != 0U) ? 1U : 0U;
}

/**
 * brief	Clears a DMA1 interrupt/status flag for a given channel
 * param	ChannelNumber the channel number, 1-7 (matches the datasheet channel numbering)
 * param	Flag the flag to clear. A value of @ref DMA_FLAG_x
 */
void DMA_ClearFlag(uint32_t ChannelNumber, uint32_t Flag){

	uint32_t shift = (ChannelNumber - 1U) * 4U;

	DMA1->IFCR = (Flag << shift);
}
