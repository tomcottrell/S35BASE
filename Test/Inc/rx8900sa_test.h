/*!*************************************************************************************************
 * @file rx8900sa_test.c
 *
 * @brief Contains test for the RTC IC
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
#include "RX8900SA/rx8900sa.h"
#include <stdbool.h>
#include <stdio.h>
#include "mtest.h"

/* Defines
   -----------------------------------------------------------------------------------------------*/

/* Typedefs
   -----------------------------------------------------------------------------------------------*/

/* Private variables
   -----------------------------------------------------------------------------------------------*/
extern I2C_HandleTypeDef  hi2c2;

/* Private interface
   -----------------------------------------------------------------------------------------------*/

bool DateIsEqual(RX8900SA_DateTime a, RX8900SA_DateTime b)
{
    if ( ( a.day == b.day ) && ( a.weekday == b.weekday ) && ( a.month == b.month )
         && ( a.year == b.year ) && ( a.minute == b.minute ) && ( a.second != b.second ) && ( a.year == b.year ))
    {
        return true;
    }
    else
    {
        return false;
    }
}
/* Public interface
   -----------------------------------------------------------------------------------------------*/

static void setup() {
    RX8900SA_Init(&hi2c2);
}

static void teardown(){

}


static void groupTeardown(){
}

TEST(RX8900SA_RW_Date)
{
    RX8900SA_DateTime  date = {9, 8, 7, SUNDAY, 31, 3, 24};
    RX8900SA_DateTime  readDate;

    RX8900SA_WriteTimeDate(date);
    HAL_Delay(1000);
    RX8900SA_ReadTimeDate(&readDate);

    IS_TRUE(DateIsEqual(date,readDate));
}END_TEST

TEST_GROUP(RX8900SA){
	INCLUDE_TEST(RX8900SA_RW_Date);
}END_GROUP
