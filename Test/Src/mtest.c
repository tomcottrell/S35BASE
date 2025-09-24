/*!*************************************************************************************************
 * @file mtest.c
 *
 * @brief Implements minimal embedded software test
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

#include "mtest.h"

/* Defines
   -----------------------------------------------------------------------------------------------*/

/* Typedefs
   -----------------------------------------------------------------------------------------------*/

/* Private variables
   -----------------------------------------------------------------------------------------------*/

/* Private interface
   -----------------------------------------------------------------------------------------------*/

/* Public interface
   -----------------------------------------------------------------------------------------------*/

bool MT_compareArrays(const uint8_t* expected, const uint8_t* test, uint16_t sizeEx, uint16_t sizeT)
{
	if ( sizeEx != sizeT) {
		return false;
	}

    for ( uint8_t i = 0; i < sizeEx; i++ )
    {
        if ( *expected != *test )
        {
            return false;
        }
        expected++;
        test++;
    }
    return true;
}

bool MT_compareStr(const char* expected, const char* test){
	while( (*expected != '\0' ) && (*test != '\0') ){
        if ( *expected != *test )
        {
            return false;
        }
        expected++;
        test++;
	}
	return true;
}

void MT_printSummary(const char* groupName, uint8_t testCount, uint8_t failCount){
	printf("------------------------------------------------------\n");
	printf("%s - Tests: %d", groupName, testCount);
	printf(" Failed: %d \n", failCount);
	printf("------------------------------------------------------\n\n");
}

/* Example
   -----------------------------------------------------------------------------------------------*/
#ifdef MT_EXAMPLE
/* Define your start/end housekeeping */
static void setup(){
}

static void teardown(){
}

/* Define test functions first*/
TEST(Check_IS_TRUE_fail){
	IS_TRUE(false);
}END_TEST

TEST(Check_IS_TRUE_ok){
	IS_TRUE(true);
}END_TEST

TEST(Check_IS_EQUAL_ok){
	IS_EQUAL(1,1);
}END_TEST

TEST(Check_IS_EQUAL_fail){
	IS_EQUAL(1,-1);
}END_TEST

TEST(Check_STR_EQUAL_ok){
	char text[] = "This is a Test.";
	STR_EQ("This is a Test.",text);
}END_TEST

TEST(Check_STR_EQUAL_fail){
	char text[] = "This is not a Test.";
	STR_EQ("This is a Test.",text);
}END_TEST

TEST(Check_ARR_EQUAL_ok){
	uint8_t arr1[] = {0, 1, 2, 3};
	uint8_t arr2[] = {0, 1, 2, 3};
	ARR_EQ(arr1, arr2);
}END_TEST

TEST(Check_ARR_EQUAL_fail){
	uint8_t arr1[] = {0, 1, 2, 3};
	uint8_t arr2[] = {0, 2, 1, 3};
	ARR_EQ(arr1, arr2);
}END_TEST

TEST(Check_ARR_EQUAL_fail_differentBounds){
	uint8_t arr1[] = {0, 1, 2, 3};
	uint8_t arr2[] = {0, 2, 3};
	ARR_EQ(arr1, arr2);
}END_TEST

TEST(Check_ASSERT_fail){
	const char* ptr = "!";
	ASSERT(ptr);
	ASSERT(NULL);
}END_TEST

/* Define test group */
/* To be able to execute this Test group, you will have to use the macro USE_TESTS before RUN_TESTS */
TEST_GROUP(MinimalTest_SelfTest){
	INCLUDE_TEST(Check_IS_TRUE_fail);
	INCLUDE_TEST(Check_IS_TRUE_ok);
	INCLUDE_TEST(Check_IS_EQUAL_ok);
	INCLUDE_TEST(Check_IS_EQUAL_fail);
	INCLUDE_TEST(Check_STR_EQUAL_ok);
	INCLUDE_TEST(Check_STR_EQUAL_fail);
	INCLUDE_TEST(Check_ARR_EQUAL_ok);
	INCLUDE_TEST(Check_ARR_EQUAL_fail);
	INCLUDE_TEST(Check_ARR_EQUAL_fail_differentBounds);
	INCLUDE_TEST(Check_ASSERT_fail);
}END_GROUP
#endif
