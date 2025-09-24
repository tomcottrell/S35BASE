/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * File Name          : TouchGFXHAL.cpp
 ******************************************************************************
 * This file was created by TouchGFX Generator 4.22.1. This file is only
 * generated once! Delete this file from your project and re-generate code
 * using STM32CubeMX or change this file manually to update it.
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2024 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */

#include <TouchGFXHAL.hpp>

/* USER CODE BEGIN TouchGFXHAL.cpp */
#include "touchgfx_input_controller.hpp"

extern "C" {
#include "dma2d_operations.h"
}

using namespace touchgfx;

/* needed for screen rotation */
#define LCD_WIDTH 320
#define LCD_HEIGHT 240
#define PIXEL_BYTESIZE 3 // size in bytes per pixel RGB888
static uint32_t dma2dBuffer[LCD_WIDTH * PIXEL_BYTESIZE / sizeof(uint32_t)];
TouchGfxInputController inputController;
static volatile bool hasNewFrame = false;

void TouchGFXHAL::initialize() {
	// Calling parent implementation of initialize().
	//
	// To overwrite the generated implementation, omit call to parent function
	// and implemented needed functionality here.
	// Please note, HAL::initialize() must be called to initialize the framework.

	TouchGFXGeneratedHAL::initialize();
	setFrameRefreshStrategy(HAL::REFRESH_STRATEGY_DEFAULT);

	inputController.init();
	setButtonController(&inputController);
}

/**
 * Gets the frame buffer address used by the TFT controller.
 *
 * @return The address of the frame buffer currently being displayed on the TFT.
 */
uint16_t* TouchGFXHAL::getTFTFrameBuffer() const {
	// Calling parent implementation of getTFTFrameBuffer().
	//
	// To overwrite the generated implementation, omit call to parent function
	// and implemented needed functionality here.

	return TouchGFXGeneratedHAL::getTFTFrameBuffer();
}

/**
 * Sets the frame buffer address used by the TFT controller.
 *
 * @param [in] address New frame buffer address.
 */
void TouchGFXHAL::setTFTFrameBuffer(uint16_t *address) {
	// Calling parent implementation of setTFTFrameBuffer(uint16_t* address).
	//
	// To overwrite the generated implementation, omit call to parent function
	// and implemented needed functionality here.

	TouchGFXGeneratedHAL::setTFTFrameBuffer(address);
}

/**
 * This function is called whenever the framework has performed a partial draw.
 *
 * @param rect The area of the screen that has been drawn, expressed in absolute coordinates.
 *
 * @see flushFrameBuffer().
 */
void TouchGFXHAL::flushFrameBuffer(const touchgfx::Rect &rect) {
	// Calling parent implementation of flushFrameBuffer(const touchgfx::Rect& rect).
	//
	// To overwrite the generated implementation, omit call to parent function
	// and implemented needed functionality here.
	// Please note, HAL::flushFrameBuffer(const touchgfx::Rect& rect) must
	// be called to notify the touchgfx framework that flush has been performed.
	// To calculate he start adress of rect,
	// use advanceFrameBufferToRect(uint8_t* fbPtr, const touchgfx::Rect& rect)
	// defined in TouchGFXGeneratedHAL.cpp

	hasNewFrame = true;

	TouchGFXGeneratedHAL::flushFrameBuffer(rect);
}

bool TouchGFXHAL::blockCopy(void *RESTRICT dest, const void *RESTRICT src,
		uint32_t numBytes) {
	return TouchGFXGeneratedHAL::blockCopy(dest, src, numBytes);
}

/**
 * Configures the interrupts relevant for TouchGFX. This primarily entails setting
 * the interrupt priorities for the DMA and LCD interrupts.
 */
void TouchGFXHAL::configureInterrupts() {
	// Calling parent implementation of configureInterrupts().
	//
	// To overwrite the generated implementation, omit call to parent function
	// and implemented needed functionality here.

	TouchGFXGeneratedHAL::configureInterrupts();
}

/**
 * Used for enabling interrupts set in configureInterrupts()
 */
void TouchGFXHAL::enableInterrupts() {
	// Calling parent implementation of enableInterrupts().
	//
	// To overwrite the generated implementation, omit call to parent function
	// and implemented needed functionality here.

	TouchGFXGeneratedHAL::enableInterrupts();
}

/**
 * Used for disabling interrupts set in configureInterrupts()
 */
void TouchGFXHAL::disableInterrupts() {
	// Calling parent implementation of disableInterrupts().
	//
	// To overwrite the generated implementation, omit call to parent function
	// and implemented needed functionality here.

	TouchGFXGeneratedHAL::disableInterrupts();
}

/**
 * Configure the LCD controller to fire interrupts at VSYNC. Called automatically
 * once TouchGFX initialization has completed.
 */
void TouchGFXHAL::enableLCDControllerInterrupt() {
	// Calling parent implementation of enableLCDControllerInterrupt().
	//
	// To overwrite the generated implementation, omit call to parent function
	// and implemented needed functionality here.

	TouchGFXGeneratedHAL::enableLCDControllerInterrupt();
}

bool TouchGFXHAL::beginFrame() {
	bool result = TouchGFXGeneratedHAL::beginFrame();
	return result;
}

void TouchGFXHAL::endFrame() {
	if (hasNewFrame) {
		uint16_t *framebuffer = touchgfx::HAL::getInstance()->lockFrameBuffer();
		if (usesDMA2D) {
			HFlip((uint32_t) framebuffer);
			VFlip((uint32_t) framebuffer);
		} else {
			DMA2D_HFlip((uint32_t) framebuffer);
			DMA2D_VFlip((uint32_t) framebuffer);
		}

		touchgfx::HAL::getInstance()->unlockFrameBuffer();
		hasNewFrame = false;
	}
	return TouchGFXGeneratedHAL::endFrame();
}

void TouchGFXHAL::HFlip(uint32_t fb) {
	uint32_t srcAddress = 0;
	uint32_t dstAddress = 0;
	uint32_t srcPxOffset = 0;
	uint32_t dstPxOffset = 0;
	colortype c = colortype();

	BlitOp op = { BLIT_OP_COPY,
	NULL,
	NULL, c,
	NULL, 1, // nSteps, number of pixels
	LCD_HEIGHT, // nLoops, number of lines
	LCD_WIDTH, // srcLoopStride src_offset = srcLoopStride - nSteps
	LCD_WIDTH, // dstLoopStride dest_offset = dstLoopStride - nSteps
	255, Bitmap::RGB888, Bitmap::RGB888, false, false };

	for (uint16_t column = 0; column < LCD_WIDTH / 2; column++) {
		// addresses
		uint32_t leftColumnAddress = fb + (column) * PIXEL_BYTESIZE;
		uint32_t rightColumnAddress = fb
				+ (LCD_WIDTH - 1 - column) * PIXEL_BYTESIZE;

		// copy left column to buffer
		srcPxOffset = LCD_WIDTH;
		dstPxOffset = 1;
		srcAddress = leftColumnAddress;
		dstAddress = (uint32_t) dma2dBuffer;

		op.pSrc = reinterpret_cast<const uint16_t*>(srcAddress);
		op.pDst = reinterpret_cast<uint16_t*>(dstAddress);
		op.srcLoopStride = srcPxOffset;
		op.dstLoopStride = dstPxOffset;
		this->dma.addToQueue(op);

		// copy right column to left column
		// srcPxOffset as above
		srcPxOffset = LCD_WIDTH;
		dstPxOffset = LCD_WIDTH;
		srcAddress = rightColumnAddress;
		dstAddress = leftColumnAddress;

		op.pSrc = reinterpret_cast<const uint16_t*>(srcAddress);
		op.pDst = reinterpret_cast<uint16_t*>(dstAddress);
		op.srcLoopStride = srcPxOffset;
		op.dstLoopStride = dstPxOffset;
		this->dma.addToQueue(op);

		// copy buffered left column to the right
		dstPxOffset = LCD_WIDTH;
		srcAddress = (uint32_t) dma2dBuffer;
		dstAddress = rightColumnAddress;
		srcPxOffset = 1;

		// dstPxOffset as above
		op.pSrc = reinterpret_cast<const uint16_t*>(srcAddress);
		op.pDst = reinterpret_cast<uint16_t*>(dstAddress);
		op.srcLoopStride = srcPxOffset;
		op.dstLoopStride = dstPxOffset;
		this->dma.addToQueue(op);
	}
	this->dma.flush();
}

void TouchGFXHAL::VFlip(uint32_t fb) {
	uint32_t srcAddress = 0;
	uint32_t dstAddress = 0;
	colortype c = colortype();
	BlitOp op = { BLIT_OP_COPY,
	NULL,
	NULL, c,
	NULL,
	LCD_WIDTH, 1,
	LCD_WIDTH,
	LCD_WIDTH, 255, Bitmap::RGB888, Bitmap::RGB888, false, false };

	// For simply copying rows, we can use the HAL API for DMA2D and don't
	// need to set any offset values.
	for (uint16_t row = 0; row < LCD_HEIGHT / 2; row++) {
		// copy top row to buffer
		srcAddress = fb + row * LCD_WIDTH * PIXEL_BYTESIZE;
		dstAddress = (uint32_t) dma2dBuffer;
		op.pSrc = reinterpret_cast<const uint16_t*>(srcAddress);
		op.pDst = reinterpret_cast<uint16_t*>(dstAddress);
		this->dma.addToQueue(op);

		// bottom row to top
		dstAddress = srcAddress;
		srcAddress = fb + (LCD_HEIGHT - row - 1) * LCD_WIDTH * PIXEL_BYTESIZE;
		op.pSrc = reinterpret_cast<const uint16_t*>(srcAddress);
		op.pDst = reinterpret_cast<uint16_t*>(dstAddress);
		this->dma.addToQueue(op);

		// copy buffer to bottom
		dstAddress = srcAddress;
		srcAddress = (uint32_t) dma2dBuffer;
		op.pSrc = reinterpret_cast<const uint16_t*>(srcAddress);
		op.pDst = reinterpret_cast<uint16_t*>(dstAddress);
		this->dma.addToQueue(op);
	}
	this->dma.flush();
}
/* USER CODE END TouchGFXHAL.cpp */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
