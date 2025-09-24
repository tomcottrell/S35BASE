/*!*************************************************************************************************
 * @file adc_helper.h
 *
 * @brief Implements helper functions for controlling and retrieving ADC sampled values
 *
 * @author Enovation Controls
 *
 * @copyright Copyright 2024 by Enovation Controls.
 *            This file and/or parts of it are subject to Enovation Controls
 *            software license terms.
 *            With the use of this software you accept the SoLiT.
 *            Without written approval of Enovation Controls this
 *            software may not be copied, redistributed or used
 *            in any other way than stated in the conditions of the SoLiT.
 *
 **************************************************************************************************/

#ifndef ADC_HELPER_H
#define ADC_HELPER_H

/* Includes
   -----------------------------------------------------------------------------------------------*/
#include <stdint.h>

/* Defines
   -----------------------------------------------------------------------------------------------*/

/* Typedefs
   -----------------------------------------------------------------------------------------------*/

typedef enum {
	HW_REF_ID0,
	HW_REF_ID1,
	VIN_MEAS,
	DISP_TEMP,
	AIN_1,
	AIN_2,
	INTERNAL_TEMP,
	VREF_INT,
	AIN_1_R,
	AIN_1_I,
	AIN_2_R,
	AIN_2_I
} ADC_Channels;

/* Private variables
   -----------------------------------------------------------------------------------------------*/

/* Private interface
   -----------------------------------------------------------------------------------------------*/

/* Public interface
   -----------------------------------------------------------------------------------------------*/
//
void ADC_InitTask(void);

//
void ADC_startTask(void *argument);

//
void ADC_startSampling(void);

//
void ADC_stopSampling(void);

//
void ADC_triggerConversion(void);

//
uint16_t ADC_getRawValue(ADC_Channels channel);

//
float ADC_getAbsValue(ADC_Channels channel);

//
float ADC_getTempValue(ADC_Channels channel);

//
float ADC_getVRef(void);

//
float ADC_getAINResistance(ADC_Channels channel, uint16_t holdTimeInConvCycles);

//
float ADC_getAINCurrent(ADC_Channels channel, uint16_t holdTimeInConvCycles);

//
float ADC_getAINVoltage(ADC_Channels channel);

#endif
