/*!*************************************************************************************************
 * @file RX8900SA.c
 *
 * @brief Implements RTC driver
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
#include <RX8900SA/rx8900sa.h>

/* Defines
   -----------------------------------------------------------------------------------------------*/

/* Typedefs
   -----------------------------------------------------------------------------------------------*/

/* Private variables
   -----------------------------------------------------------------------------------------------*/
static I2C_HandleTypeDef* i2cHandle;

/* Private interface
   -----------------------------------------------------------------------------------------------*/

/*!*************************************************************************************************
 * @brief bcd2Bin converts rtc data from "binary coded decimal" to binary
 * @param bcd bcd nimble
 * @return binary value
 **************************************************************************************************/
uint8_t _bcd2Bin(uint8_t bcd)
{
    return (( bcd >> 4 ) * 10 ) + ( bcd & 0x0F );
}

/*!*************************************************************************************************
 * @brief bin2Bcd converts binary to bcd format
 * @param bin binary input
 * @return bcd value
 **************************************************************************************************/
uint8_t _bin2Bcd(uint8_t bin)
{
    return (( bin / 10 ) << 4 ) | ( bin % 10 );
}

/* Public interface
   -----------------------------------------------------------------------------------------------*/

/*!*************************************************************************************************
 * @brief RX8900SA_Init
 * @param hi2c HAL handle to stm32 i2c interface
 **************************************************************************************************/
void RX8900SA_Init(I2C_HandleTypeDef* hi2c)
{
    i2cHandle = hi2c;
}

/*!*************************************************************************************************
 * @brief RX8900SA_readTimeDate
 * @param dateTime
 **************************************************************************************************/
void RX8900SA_ReadTimeDate(RX8900SA_DateTime *dateTime)
{
    uint8_t  rx_data[7];

    if ( dateTime == NULL )
    {
        return;
    }

    HAL_I2C_Mem_Read(i2cHandle, RX_I2C_ADDRESS, 0, 1, rx_data, 7, 1000);

    dateTime->second = _bcd2Bin(rx_data[0]);
    dateTime->minute = _bcd2Bin(rx_data[1]);
    dateTime->hour = _bcd2Bin(rx_data[2]);

    for ( int i = 0; i < 7; i++ )
    {
        if ( rx_data[3] & ( 0x01 << i ))
        {
            dateTime->weekday = i;
            break;
        }
    }

    dateTime->day = _bcd2Bin(rx_data[4]);
    dateTime->month = _bcd2Bin(rx_data[5]);
    dateTime->year = _bcd2Bin(rx_data[6]);
}


/*!*************************************************************************************************
 * @brief RX8900SA_writeTimeDate
 * @param dateTime
 **************************************************************************************************/
void RX8900SA_WriteTimeDate(RX8900SA_DateTime dateTime)
{
    uint8_t  tx_data[7];

    tx_data[0] = _bin2Bcd(dateTime.second);
    tx_data[1] = _bin2Bcd(dateTime.minute);
    tx_data[2] = _bin2Bcd(dateTime.hour);

    tx_data[3] = ( 0x01 << dateTime.weekday );

    tx_data[4] = _bin2Bcd(dateTime.day);
    tx_data[5] = _bin2Bcd(dateTime.month);
    tx_data[6] = _bin2Bcd(dateTime.year);

    HAL_I2C_Mem_Write(i2cHandle, RX_I2C_ADDRESS, 0, 1, tx_data, 7, 1000);
}
