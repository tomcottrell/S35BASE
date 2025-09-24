/*!*************************************************************************************************
 * @file mtest.h
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

#ifndef M_TEST_H
#define M_TEST_H

/* Includes
   -----------------------------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

/* Defines
   -----------------------------------------------------------------------------------------------*/

#define MT_VERSION         1.0.0
//#define MT_EXAMPLE // uncomment for self test
#define MAX_TESTS_IN_GROUP 64

/* Typedefs
   -----------------------------------------------------------------------------------------------*/
typedef bool (* testFuncPtr)(void);

/* Public interface
   -----------------------------------------------------------------------------------------------*/

#ifdef __cplusplus
extern "C" {
#endif

// setup and teardown are called before/after every test
// void setup(void);    		// define this function in your tests file,
// void teardown(void); 		// define this function in your tests file,
// void groupTeardown(void); 	// define this function in your tests file

void MT_printSummary(const char* groupName, uint8_t testCount, uint8_t failCount);
bool MT_compareArrays(const uint8_t* expected, const uint8_t* test, uint16_t sizeEx, uint16_t sizeT);
bool MT_compareStr(const char* expected, const char* test);
bool MT_selfTest(void);

/* Macros
   -----------------------------------------------------------------------------------------------*/

#define XSTR(x) STR(x)
#define STR(x)  #x

#define ASSERT(ptr) if ( !ptr ) { failCount++; }
#define IS_TRUE(cond) \
				if(! (cond) ) { failCount++; }
#define IS_EQUAL(a, b) if ( a != b ) { failCount++; }

#define IS_NEQUAL(a, b) if ( a == b ) { failCount++; }

#define ARR_EQ(a, b) if (! MT_compareArrays(a, b, sizeof(a), sizeof(b)) ) { failCount++; }

#define STR_EQ(a, b) if (! MT_compareStr(a,b) ) { failCount++; }

#define RUN_TESTS(name) TestGroup_##name();

/* To make the test group fully defined, use this macro where you want to run the test group. */
#define USE_TESTS(name) bool TestGroup_##name(void);

/* Define a test group after all test functions were defined. Include tests by the
 * INCLUDE_TEST macro.
 */
#define TEST_GROUP(name)   \
		    bool TestGroup_##name(void){                               					\
		    testFuncPtr tests[MAX_TESTS_IN_GROUP];                             			\
		    const char* groupName = XSTR(name); 										\
            uint8_t  index = 0;                                                       	\
            uint8_t  testCount = 0;                                                   	\
            uint8_t  failCount = 0;                                                   	\
            printf("------------------------------------------------------\n");        	\
            printf("Minimal Test %s\n",       XSTR(MT_VERSION));                      	\
            printf("Starting Test Group: %s \n", groupName);                          	\
            printf("------------------------------------------------------\n\n");       \
            if ( true )                                                               	\

#define END_GROUP                                                             	        \
        tests[index] = NULL;  															\
		index = 0;                                               						\
        while ( tests[index] != NULL ) {                                              	\
            bool  result = tests[index]();                                            	\
            testCount++;                                                              	\
            index++;                                                                  	\
            if ( !result ){                                                            	\
                failCount++;                                                            \
            }                                                                           \
        }                                                                               \
        MT_printSummary(groupName, testCount, failCount);   				           	\
        groupTeardown(); 																\
        if ( failCount ){                                                               \
            return false;                                                               \
        }                                                                               \
        return true;                                                                    \
        }                                                                               \

/* Use this within the test group definition, to add test functions to the group */
#define INCLUDE_TEST(name) tests[index] = Test_##name; 									\
						   index++; 													\
						   if(index == MAX_TESTS_IN_GROUP){                             \
							   printf("Maximum number of tests reached. Aborting!");	\
							   return false;											\
						   } 															\

/* Define all your test cases, before setting up the test group! */
#define TEST(name) bool Test_##name(void) {                                       		\
            const char* testname = XSTR(name);                                          \
            uint8_t     failCount = 0;                                                  \
            setup();                                                                    \
            if ( true )                                                               	\

#define END_TEST                                                                     	\
        teardown();                                                                     \
        if ( failCount ){                                                               \
        	printf("- Test: %s - %s\n", "FAILED", testname);                            \
            return false;                                                               \
        }                                                                               \
    	printf("- Test: %s - %s\n", "OK    ", testname);                                \
        return true;                                                                    \
        }                                                                               \

#ifdef __cplusplus
}
#endif

#endif
