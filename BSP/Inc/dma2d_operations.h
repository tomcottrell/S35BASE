/*!*******************************************************************************
 * @file dma2d_operations.h
 *
 * @brief This file contains implementations of screen operations using dma2d
 *
 * @author Enovation Controls
 *
 * @copyright Copyright 2024 by Enovation Controls.
 *            This file and/or parts of it are subject to Enovation Controls
 *            software license terms (SoLiT, Version 1.16.2).
 *            With the use of this software you accept the SoLiT.
 *            Without written approval of Enovation Controls this
 *            software may not be copied, redistributed or
 *            used in any other way than stated in the conditions of the SoLiT.
 *
 *******************************************************************************/

#ifndef DMA2D_OPERATIONS_H
#define DMA2D_OPERATIONS_H

/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_hal.h"
#include "stdbool.h"

/* Defines  ------------------------------------------------------------------*/
#define LCD_WIDTH 320
#define LCD_HEIGHT 240
#define PIXEL_BYTESIZE 3 // size in bytes per pixel RGB888

/* Typedefs ------------------------------------------------------------------*/
typedef struct
{
  int16_t x;
  int16_t y;
  int16_t width;
  int16_t height;
} ScreenRegion;

/* Private variables ---------------------------------------------------------*/

/* Public interface ----------------------------------------------------------*/

//
HAL_StatusTypeDef DMA2D_Start();

//
uint32_t DMA2D_GetBufferAddress();

//
void DMA2D_HFlip( uint32_t fb );

//
void DMA2D_VFlip( uint32_t fb );

//
void DMA2D_HFlipRect( uint32_t fb, ScreenRegion region );

//
void DMA2D_VFlipRect( uint32_t fb, ScreenRegion region );

#endif /* DMA2D_OPERATIONS_H */
