/*!*************************************************************************************************
 * @file RX8900SA.h
 *
 * @brief Implements driver for the RX8900SA rtc
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

#ifndef RX8900SA_H
#define RX8900SA_H

/* Includes
   -----------------------------------------------------------------------------------------------*/
#include "stm32l4xx.h"

/* Defines
   -----------------------------------------------------------------------------------------------*/
#define RX_I2C_ADDRESS 0x64

/* Typedefs
   -----------------------------------------------------------------------------------------------*/
typedef enum
{
    SUNDAY = 0,
    MONDAY,
    TUESDAY,
    WEDNESDAY,
    THURSDAY,
    FRIDAY,
    SATURDAY
} RX8900SA_Weekday;

typedef struct
{
    uint8_t          second;
    uint8_t          minute;
    uint8_t          hour;
    RX8900SA_Weekday weekday;
    uint8_t          day;
    uint8_t          month;
    uint8_t          year;

} RX8900SA_DateTime;

/* Private variables
   -----------------------------------------------------------------------------------------------*/

/* Private interface
   -----------------------------------------------------------------------------------------------*/

/* Public interface
   -----------------------------------------------------------------------------------------------*/
void RX8900SA_Init(I2C_HandleTypeDef* hi2c);
void RX8900SA_ReadTimeDate(RX8900SA_DateTime *dateTime);
void RX8900SA_WriteTimeDate(RX8900SA_DateTime dateTime);

#endif
