/*!*******************************************************************************
 * @file dma2d_operations.c
 *
 * @brief This file contains implementations of screen operations using dma2d
 *
 * @author Enovation Controls
 *
 * @copyright Copyright 2024 by Enovation Controls
 *            This file and/or parts of it are subject to Enovation Controls
 *            software license terms (SoLiT, Version 1.16.2).
 *            With the use of this software you accept the SoLiT.
 *            Without written approval of Enovation Controls this
 *            software may not be copied, redistributed or
 *            used in any other way than stated in the conditions of the SoLiT.
 *
 *******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "dma2d_operations.h"
#include "stm32l4xx_hal.h"

/* Defines  ------------------------------------------------------------------*/

/* Typedefs ------------------------------------------------------------------*/
extern DMA2D_HandleTypeDef hdma2d;

/* Private variables ---------------------------------------------------------*/
static uint32_t dma2dBuffer[LCD_WIDTH * PIXEL_BYTESIZE / sizeof(uint32_t)];

/* Private functions ---------------------------------------------------------*/
static bool checkRegion(ScreenRegion *region) {
	return ((LCD_WIDTH > region->x && region->x >= 0)
			&& (LCD_HEIGHT > region->y && region->y >= 0)
			&& (LCD_WIDTH >= region->width && region->width > 2)
			&& (LCD_HEIGHT >= region->height && region->height > 2));
}

/* Public interface ----------------------------------------------------------*/

uint32_t DMA2D_GetBufferAddress() {
	return (uint32_t) dma2dBuffer;
}

HAL_StatusTypeDef DMA2D_Start(uint32_t src, uint32_t dest, uint32_t src_offset,
		uint32_t dest_offset) {
	/* Process locked */
	__HAL_LOCK(&hdma2d);

	WRITE_REG(DMA2D->FGMAR, src);
	WRITE_REG(DMA2D->OMAR, dest);
	WRITE_REG(DMA2D->FGOR, src_offset);
	WRITE_REG(DMA2D->OOR, dest_offset);

	/* Change DMA2D peripheral state */
	hdma2d.State = HAL_DMA2D_STATE_BUSY;

	/* Enable the Peripheral */
	__HAL_DMA2D_ENABLE(&hdma2d);

	return HAL_BUSY;
}

void DMA2D_HFlip(uint32_t fb) {
	uint32_t srcAddr = 0;
	uint32_t destAddr = 0;
	uint32_t srcOffset = 0;
	uint32_t destOffset = 0;

	// This nlr setup is constant for all operations on columns.
	// Treat each pixel as a line with width=1, so we can apply "offset" values for
	// source and destination
	WRITE_REG(hdma2d.Instance->NLR, (LCD_HEIGHT| (1 << DMA2D_NLR_PL_Pos)));

	for (uint16_t column = 0; column < LCD_WIDTH / 2; column++) {
		// addresses
		uint32_t left_column_addr = fb + (column) * PIXEL_BYTESIZE;
		uint32_t right_column_addr = fb
				+ (LCD_WIDTH - 1 - column) * PIXEL_BYTESIZE;

		// copy left column to buffer
		srcOffset = LCD_WIDTH - 1;
		destOffset = 0;
		srcAddr = left_column_addr;
		destAddr = (uint32_t) dma2dBuffer;

		DMA2D_Start(srcAddr, destAddr, srcOffset, destOffset);
		HAL_DMA2D_PollForTransfer(&hdma2d, 1);

		// copy right column to left column
		// srcOffset as above
		srcOffset = LCD_WIDTH - 1;
		destOffset = LCD_WIDTH - 1;
		srcAddr = right_column_addr;
		destAddr = left_column_addr;

		DMA2D_Start(srcAddr, destAddr, srcOffset, destOffset);
		HAL_DMA2D_PollForTransfer(&hdma2d, 1);

		// copy buffered left column to the right
		destOffset = LCD_WIDTH - 1;
		srcAddr = (uint32_t) dma2dBuffer;
		destAddr = right_column_addr;
		srcOffset = 0;

		// destOffset as above
		DMA2D_Start(srcAddr, destAddr, srcOffset, destOffset);
		HAL_DMA2D_PollForTransfer(&hdma2d, 1);
	}
}

void DMA2D_VFlip(uint32_t fb) {
	uint32_t srcAddr = 0;
	uint32_t destAddr = 0;

	// For simply copying rows, we can use the HAL API for DMA2D and don't
	// need to set any offset values.
	for (uint16_t row = 0; row < LCD_HEIGHT / 2; row++) {
		// copy top row to buffer
		srcAddr = fb + row * LCD_WIDTH * PIXEL_BYTESIZE;
		destAddr = (uint32_t) dma2dBuffer;
		HAL_DMA2D_Start(&hdma2d, srcAddr, destAddr, LCD_WIDTH, 1);
		HAL_DMA2D_PollForTransfer(&hdma2d, 1);

		// bottom row to top
		destAddr = srcAddr;
		srcAddr = fb + (LCD_HEIGHT - row - 1) * LCD_WIDTH * PIXEL_BYTESIZE;
		HAL_DMA2D_Start(&hdma2d, srcAddr, destAddr, LCD_WIDTH, 1);
		HAL_DMA2D_PollForTransfer(&hdma2d, 1);

		// copy buffer to bottom
		destAddr = srcAddr;
		srcAddr = (uint32_t) dma2dBuffer;
		HAL_DMA2D_Start(&hdma2d, srcAddr, destAddr, LCD_WIDTH, 1);
		HAL_DMA2D_PollForTransfer(&hdma2d, 1);
	}

}

void DMA2D_HFlipRect(uint32_t fb, ScreenRegion region) {
	uint32_t srcAddr = 0;
	uint32_t destAddr = 0;
	uint32_t srcOffset = 0;
	uint32_t destOffset = 0;

	if (!checkRegion(&region)) {
		return;
	}

	// This nlr setup is constant for all operations on columns.
	// Treat each pixel as a line with width=1, so we can apply "offset" values for
	// source and destination
	MODIFY_REG(hdma2d.Instance->NLR, (DMA2D_NLR_NL | DMA2D_NLR_PL),
			(region.height| (1 << DMA2D_NLR_PL_Pos)));

	for (uint16_t column = 0; column < region.width / 2; column++) {
		// addresses
		uint32_t left_column_addr = fb + region.y * LCD_WIDTH * PIXEL_BYTESIZE
				+ (column + region.x) * PIXEL_BYTESIZE;
		uint32_t right_column_addr = fb + region.y * LCD_WIDTH * PIXEL_BYTESIZE
				+ (region.width - 1 - column + region.x) * PIXEL_BYTESIZE;

		// copy left column to buffer
		srcOffset = LCD_WIDTH - 1;
		destOffset = 0;
		srcAddr = left_column_addr;
		destAddr = (uint32_t) dma2dBuffer;

		DMA2D_Start(srcAddr, destAddr, srcOffset, destOffset);
		HAL_DMA2D_PollForTransfer(&hdma2d, 1);

		// copy right column to left column
		// srcOffset as above
		srcOffset = LCD_WIDTH - 1;
		destOffset = LCD_WIDTH - 1;
		srcAddr = right_column_addr;
		destAddr = left_column_addr;

		DMA2D_Start(srcAddr, destAddr, srcOffset, destOffset);
		HAL_DMA2D_PollForTransfer(&hdma2d, 1);

		// copy buffered left column to the right
		destOffset = LCD_WIDTH - 1;
		srcAddr = (uint32_t) dma2dBuffer;
		destAddr = right_column_addr;
		srcOffset = 0;
		// destOffset as above

		DMA2D_Start(srcAddr, destAddr, srcOffset, destOffset);
		HAL_DMA2D_PollForTransfer(&hdma2d, 1);
	}
}

void DMA2D_VFlipRect(uint32_t fb, ScreenRegion region) {
	uint32_t srcAddr = 0;
	uint32_t destAddr = 0;

	if (!checkRegion(&region)) {
		return;
	}

	// operate on 1 line with width region.width
	MODIFY_REG(hdma2d.Instance->NLR, (DMA2D_NLR_NL | DMA2D_NLR_PL),
			(1 | (region.width << DMA2D_NLR_PL_Pos)));

	for (uint16_t row = 0; row < region.height / 2; row++) {
		// copy top row to buffer
		srcAddr = fb + (row + region.y) * LCD_WIDTH * PIXEL_BYTESIZE
				+ region.x * PIXEL_BYTESIZE;
		destAddr = (uint32_t) dma2dBuffer;
		DMA2D_Start(srcAddr, destAddr, 0, 0);
		HAL_DMA2D_PollForTransfer(&hdma2d, 1);

		// bottom row to top
		destAddr = srcAddr;
		srcAddr = fb
				+ (region.height + region.y - row - 1) * LCD_WIDTH
						* PIXEL_BYTESIZE + region.x * PIXEL_BYTESIZE;
		DMA2D_Start(srcAddr, destAddr, 0, 0);
		HAL_DMA2D_PollForTransfer(&hdma2d, 1);

		// copy buffer to bottom
		destAddr = srcAddr;
		srcAddr = (uint32_t) dma2dBuffer;
		DMA2D_Start(srcAddr, destAddr, 0, 0);
		HAL_DMA2D_PollForTransfer(&hdma2d, 1);
	}
}
