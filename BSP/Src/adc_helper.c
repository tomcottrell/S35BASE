/*!*************************************************************************************************
 * @file adc_helper.c
 *
 * @brief Implements helper functions for controlling and retrieving ADC sampled values
 *
 * @author Enovation Controls
 *
 * @copyright Copyright 2024 by Enovation Controls.
 *            This file and/or parts of it are subject to Enovation Controls
 *            software license terms (SoLiT, Version 1.16.2).
 *            With the use of this software you accept the SoLiT.
 *            Without written approval of Enovation Controls this
 *            software may not be copied, redistributed or used
 *            in any other way than stated in the conditions of the SoLiT.
 *
 **************************************************************************************************/

/* Includes
   -----------------------------------------------------------------------------------------------*/
#include <stm32l4xx_hal.h>
#include <stdbool.h>
#include <stdio.h>
#include <cmsis_os2.h>
#include "adc_helper.h"
#include "main.h"
#include "frequency.h"
#include "analogs.h"

/* Defines
   -----------------------------------------------------------------------------------------------*/

#define TS_CAL1_ADDR          0x1FFF75A8 // calibration value (30°) written by ST in read only mem
// section
#define TS_CAL2_ADDR          0x1FFF75CA // calibration value (130°) written by ST in read only mem
// section
#define CAL1_TEMP             30
#define CAL2_TEMP             130
#define TEMP_STEP_WIDTH 	  16
#define NR_CHANNELS           8

// derived from the schematic of the adc inputs
#define VIN_OFFSET            0.73		//0.63
#define VIN_CORRECTION_FACTOR 12.85
#define AIN_CORRECTION_FACTOR 1.6723

/* Typedefs
   -----------------------------------------------------------------------------------------------*/

/* Private variables
   -----------------------------------------------------------------------------------------------*/

uint16_t                  adcSensorValues[NR_CHANNELS];
extern TIM_HandleTypeDef  htim7;
extern ADC_HandleTypeDef  hadc1;
extern DMA_HandleTypeDef  hdma_adc1;
static volatile bool      conversionCplt = false;

extern float SupplyVoltage;
extern float LCD_temperature;

int                       tempTable[] = {
		// vref 3.0v
		12500, 12500, 12500, 12500, 12400, 11500,10900,10300,
		10000, 9600, 9260, 8950, 8667, 8417, 8186, 7963, 7763, 7578,
		    7400, 7230, 7073, 6927, 6783, 6650, 6523, 6400, 6279, 6167,
		    6060, 5953, 5850, 5753, 5659, 5567, 5478, 5389, 5305, 5225,
		    5148, 5071, 4995, 4919, 4848, 4778, 4709, 4642, 4576, 4512,
		    4450, 4388, 4327, 4267, 4207, 4152, 4097, 4041, 3987, 3933,
		    3880, 3827, 3775, 3725, 3676, 3627, 3579, 3530, 3483, 3437,
		    3391, 3346, 3300, 3257, 3214, 3170, 3127, 3085, 3044, 3003,
		    2962, 2921, 2880, 2841, 2802, 2763, 2724, 2686, 2648, 2610,
		    2573, 2536, 2500, 2464, 2427, 2391, 2356, 2320, 2285, 2250,
		    2215, 2181, 2147, 2113, 2079, 2046, 2013, 1980, 1947, 1914,
		    1882, 1849, 1816, 1784, 1752, 1720, 1688, 1657, 1625, 1594,
		    1563, 1533, 1502, 1471, 1440, 1410, 1379, 1349, 1319, 1289,
		    1259, 1230, 1200, 1170, 1141, 1111, 1081, 1052, 1022, 993,
		    964, 935, 905, 876, 847, 818, 789, 761, 732, 704,
		    675, 645, 616, 588, 559, 530, 502, 473, 445, 416,
		    388, 359, 330, 302, 273, 244, 215, 186, 157, 129,
		    100, 71, 42, 13, -16, -45, -75, -104, -133, -162,
		    -191, -220, -250, -280, -309, -339, -369, -398, -428, -458,
		    -489, -519, -550, -581, -612, -642, -673, -704, -735, -767,
		    -798, -830, -862, -894, -926, -958, -990, -1023, -1056, -1090,
		    -1123, -1156, -1190, -1224, -1259, -1293, -1329, -1364, -1400, -1436,
		    -1471, -1507, -1544, -1581, -1619, -1657, -1695, -1734, -1773, -1813,
		    -1853, -1893, -1934, -1976, -2018, -2061, -2103, -2147, -2192, -2237,
		    -2283, -2329, -2376, -2424, -2473, -2523, -2574, -2627, -2680, -2734,
		    -2790, -2846, -2904, -2963, -3023, -3085, -3148, -3213, -3283
	};

const int TEMPERATURE_TABLE_SIZE = sizeof(tempTable) / sizeof(tempTable[0]);

/* Definitions for ADC_Task */
osThreadId_t              ADC_TaskHandle;
const osThreadAttr_t      ADC_Task_attributes = { .name = "ADC_Task", .stack_size =
                                                      256 * 4,
                                                  .priority = (osPriority_t) osPriorityBelowNormal,
};


/* Private interface
   -----------------------------------------------------------------------------------------------*/

void _waitUntilConvCplt(void)
{
    while ( !conversionCplt )
    {

    }
}

/*!*************************************************************************************************
 * @brief _getVref
 * @return
 **************************************************************************************************/
float _getVref(void)
{
    const uint16_t  vcal = 3.0;      // 3.0V at calibration
    const uint16_t  cal = *(uint16_t*) ( VREFINT_CAL_ADDR );
    const float     vrefint = adcSensorValues[VREF_INT];
    float           vref = 3.0;

    if ( vrefint != 0 )
    {
        vref = (float) ( vcal * cal ) / vrefint;
    }

    return vref;
}

/*!*************************************************************************************************
 * @brief _getVRes
 * @return
 **************************************************************************************************/
float _getVRes(void)
{
    const float  vref = _getVref();
    float        res = vref / 4095;                       // 12 bit res [V]
    return res;
}

/*!*************************************************************************************************
 * @brief _calculateDisplayTemperature
 * @return
 **************************************************************************************************/
float _calculateDisplayTemperature(void)
{
    uint16_t  raw = ( adcSensorValues[DISP_TEMP] >> 4 ); // 4096, 12bit resolution down to 8bit
    raw = (raw > TEMPERATURE_TABLE_SIZE - 1) ? TEMPERATURE_TABLE_SIZE - 1 : raw;
    float t0 = (float) tempTable[( raw )]; // °Deg
    float t1 = (float) tempTable[( raw ) + 1]; // °Deg
    int x0 =  raw * TEMP_STEP_WIDTH;
    int x1 =  x0 + TEMP_STEP_WIDTH;
    int dx = TEMP_STEP_WIDTH;
    int x = adcSensorValues[DISP_TEMP];

    float     temp = ((t0 * (x1-x) + t1 * (x-x0)) / dx ) / 100;
    return temp;
}

/*!*************************************************************************************************
 * @brief _calculateInternalTemperature
 * @return
 **************************************************************************************************/
float _calculateInternalTemperature(void)
{
    uint16_t     cal1 = *(uint16_t*) ( TS_CAL1_ADDR );
    uint16_t     cal2 = *(uint16_t*) ( TS_CAL2_ADDR );

    uint16_t     raw = adcSensorValues[INTERNAL_TEMP];

    // equation as given in the manual
    // T = [ (CAL2_TEMP - CAL1_TEMP) / CAL2-CAL1 ] * (DATA-CAL1) + 30°C
    const float  vrefFactor = _getVref() / 3.0;
    const float  calFactor = (((float) ( CAL2_TEMP - CAL1_TEMP )) / ( cal2 - cal1 ));
    float        value = (uint16_t) ( calFactor * (( raw * vrefFactor ) - cal1 ) + 30 );
    return value;
}

/*!*************************************************************************************************
 * @brief _getVinMEAS
 * @return Voltage in [V]
 **************************************************************************************************/
float _getVinMEAS(void)
{
    float  value = adcSensorValues[VIN_MEAS] * _getVRes();
    value *= VIN_CORRECTION_FACTOR;
    value += VIN_OFFSET;
    return value;
}

/*!*************************************************************************************************
 * @brief Start_ADC_Task
 * @param argument
 **************************************************************************************************/
void ADC_startTask(void *argument)
{
    /* Infinite loop */
    for (;;)
    {
        osDelay(1000);

        //Store Analogue values
        SupplyVoltage = ADC_getAbsValue(VIN_MEAS);
        LCD_temperature = ADC_getTempValue(DISP_TEMP);

        //Store analogue channel 1 value
        switch(analog_config[0])
		{
			case 0:
				//Not used
				break;
			case 1:
				//Resistive
				analogInput1value = ADC_getAbsValue(AIN_1_R);
				break;
			case 2:
				//Voltage
				analogInput1value = ADC_getAbsValue(AIN_1);
				break;
			case 3:
				//4-20mA current
				analogInput1value = ADC_getAbsValue(AIN_1_I);
				break;
		}

        //Store analogue channel 2 value
		switch(analog_config[1])
		{
			case 0:
				//Not used
				break;
			case 1:
				//Resistive
				analogInput2value = ADC_getAbsValue(AIN_2_R);
				break;
			case 2:
				//Voltage
				analogInput2value = ADC_getAbsValue(AIN_2);
				break;
			case 3:
				//4-20mA current
				analogInput2value = ADC_getAbsValue(AIN_2_I);
				break;
		}

		printf("MCU temp: %2.0f  Display temp: %2.0f  Vin: %2.1f\n\r",
		        	               ADC_getTempValue(INTERNAL_TEMP),
								   ADC_getTempValue(DISP_TEMP),
		        				   ADC_getAbsValue(VIN_MEAS));

       // ToDo: you can add here additional print-outs of adc values

    }
}

/* Public interface
   -----------------------------------------------------------------------------------------------*/

void ADC_InitTask(void)
{
    // Init Task
    ADC_TaskHandle = osThreadNew(ADC_startTask, NULL, &ADC_Task_attributes);
}

/*!*************************************************************************************************
 * @brief HAL_ADC_ConvCpltCallback
 * @param hadc
 **************************************************************************************************/
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
    conversionCplt = true;
}

/*!*************************************************************************************************
 * @brief ADC_triggerConversion
 *
 **************************************************************************************************/
void ADC_triggerConversion(void)
{
    conversionCplt = false;
    // start ADC sampling
    HAL_ADC_Start_DMA(&hadc1, (uint32_t*) &adcSensorValues, NR_CHANNELS);
}

/*!*************************************************************************************************
 * @brief ADC_startSampling
 *
 **************************************************************************************************/
void ADC_startSampling(void)
{
    // start trigger source
    HAL_TIM_Base_Start_IT(&htim7);
}

/*!*************************************************************************************************
 * @brief ADC_stopSampling
 *
 **************************************************************************************************/
void ADC_stopSampling(void)
{
    HAL_ADC_Stop_DMA(&hadc1);
    HAL_TIM_Base_Stop_IT(&htim7);
}

/*!*************************************************************************************************
 * @brief ADC_getRawValue
 * @param channel
 * @return
 **************************************************************************************************/
uint16_t ADC_getRawValue(ADC_Channels channel)
{
    _waitUntilConvCplt();
    return adcSensorValues[channel];
}

/*!*************************************************************************************************
 * @brief ADC_getAbsValue
 * @param channel
 * @return
 **************************************************************************************************/
float ADC_getAbsValue(ADC_Channels channel)
{
    float  value = 0;

    switch ( channel )
    {
        case VIN_MEAS:
            value = _getVinMEAS();
            break;
        case AIN_1:
            value = ADC_getAINVoltage(AIN_1);
            break;
        case AIN_1_R:
            value = ADC_getAINResistance(AIN_1_R, 1);
            break;
        case AIN_1_I:
            value = ADC_getAINCurrent(AIN_1_I, 1);
            break;
        case AIN_2:
            value = ADC_getAINVoltage(AIN_2);
            break;
        case AIN_2_R:
            value = ADC_getAINResistance(AIN_2_R, 1);
            break;
        case AIN_2_I:
            value = ADC_getAINCurrent(AIN_2_I, 1);
            break;
        default:
            value = adcSensorValues[channel] * _getVRes();
            break;
    }
    return value;
}

/*!*************************************************************************************************
 * @brief ADC_getTempValue
 * @param channel
 * @return
 **************************************************************************************************/
float ADC_getTempValue(ADC_Channels channel)
{

    if (( channel != DISP_TEMP ) && ( channel != INTERNAL_TEMP ))
    {
        return 0;
    }

    _waitUntilConvCplt();

    if ( channel == DISP_TEMP )
    {
        return _calculateDisplayTemperature();
    }
    else
    {
        return _calculateInternalTemperature();
    }
}

/*!*************************************************************************************************
 * @brief ADC_getVRef
 * @return in mV
 **************************************************************************************************/
float ADC_getVRef(void)
{
    float  vref = _getVref();
    return vref;
}


/*!*************************************************************************************************
 * @brief ADC_getResistance
 * @param channel AIN_1_R or AIN_2_R
 * @param holdTimeInConvCycles waits number of conversions for measurement before resetting
 * @return
 **************************************************************************************************/
float ADC_getAINResistance(ADC_Channels channel, uint16_t holdTimeInConvCycles)
{
    if ( (channel != AIN_1_R) && (channel != AIN_2_R) )
    {
        return -1;
    }

    GPIO_TypeDef*   port = ( channel ==
                             AIN_1_R )? ENABLE_RESISTIVE_AIN1_GPIO_Port :
                           ENABLE_RESISTIVE_AIN2_GPIO_Port;
    uint16_t        pin = ( channel ==
                            AIN_1_R ) ? ENABLE_RESISTIVE_AIN1_Pin : ENABLE_RESISTIVE_AIN2_Pin;
    HAL_GPIO_WritePin(port,
                      pin, GPIO_PIN_SET);

    for ( uint16_t c = 0; c < holdTimeInConvCycles; c++ )
    {
        _waitUntilConvCplt();
    }
    HAL_GPIO_WritePin(port,
                      pin, GPIO_PIN_RESET);

    ADC_Channels    ch = ( channel == AIN_1_R ) ? AIN_1 : AIN_2;
    const uint16_t  Vin = 5;                                // [V]
    const uint16_t  R1 = 212;                               // [Ohm]
    float           Vout = adcSensorValues[ch];
    Vout *= _getVRes();                                     // [V]
    Vout *= AIN_CORRECTION_FACTOR;

    float           R2 = R1 * ( 1 / (( Vin / Vout ) - 1 )); // [Ohm]
    return R2;
}


/*!*************************************************************************************************
 * @brief ADC_getAINCurrent
 * @param channel AIN_1_I or AIN_2_I
 * @param holdTimeInConvCycles waits number of conversions for measurement before resetting
 * @return
 **************************************************************************************************/
float ADC_getAINCurrent(ADC_Channels channel, uint16_t holdTimeInConvCycles)
{
    if ( (channel != AIN_1_I) && (channel != AIN_2_I) )
    {
        return -1;
    }

    GPIO_TypeDef*   port = ( channel ==
                             AIN_1_I )? ENABLE_CURRENT_AIN1_GPIO_Port :
                           ENABLE_CURRENT_AIN2_GPIO_Port;
    uint16_t        pin = ( channel ==
                            AIN_1_I ) ? ENABLE_CURRENT_AIN1_Pin : ENABLE_CURRENT_AIN2_Pin;

    HAL_GPIO_WritePin(port, pin,
                      GPIO_PIN_SET);

    for ( uint16_t c = 0; c < holdTimeInConvCycles; c++ )
    {
        _waitUntilConvCplt();
    }

    HAL_GPIO_WritePin(port, pin,
                      GPIO_PIN_RESET);

    ADC_Channels    ch = ( channel == AIN_1_I ) ? AIN_1 : AIN_2;
    float           value = adcSensorValues[ch];
    value *= _getVRes();     // [V]
    value *= AIN_CORRECTION_FACTOR;

    const uint16_t  R = 200; // [Ohm] resistor used for
                             // measuring
    // I = U/R
    value /= R;              // [A]
    return value;
}

/*!*************************************************************************************************
 * @brief ADC_getAINVoltage
 * @param channel
 * @param holdTimeInConvCycles waits number of conversions for measurement before resetting
 * @return
 **************************************************************************************************/
float ADC_getAINVoltage(ADC_Channels channel)
{
    if ( channel != AIN_1 && channel != AIN_2 )
    {
        return -1;
    }

    float  value = adcSensorValues[channel] * _getVRes();
    value *= AIN_CORRECTION_FACTOR;
    return value;
}
