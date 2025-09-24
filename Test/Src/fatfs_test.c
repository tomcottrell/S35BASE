/*!*************************************************************************************************
 * @file fatfs_test.c
 *
 * @brief Contains for fatfs on external SPI flash
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
#include <ff.h>
#include <string.h>
#include <fatfs.h>

/* Defines
   -----------------------------------------------------------------------------------------------*/

/* Typedefs
   -----------------------------------------------------------------------------------------------*/

/* Private variables
   -----------------------------------------------------------------------------------------------*/

/* Private interface
   -----------------------------------------------------------------------------------------------*/

static void setup(){

}

static void teardown(){

}

static void groupTeardown(){

}

/* Public interface
   -----------------------------------------------------------------------------------------------*/

TEST(CreateFile_Readback_Ok){
	TCHAR* filepath = "testfile.txt";
	const char* data = "This is a test line of bytes, for testfile.";
	char readbackBuffer[44];

	FATFS fs;
    FATFS* fsPtr = &fs;
	f_mount(fsPtr, "", 1); // always mount fs

  FIL       fp;
  int       openFlags = FA_WRITE | FA_CREATE_ALWAYS;
  FRESULT ffResult = f_open(&fp, filepath, openFlags);

  IS_EQUAL(ffResult, FR_OK);

  uint32_t bytesWritten = 0;
  ffResult = f_write(&fp, (const void*) data, strlen(data), (UINT*) &bytesWritten);
  ffResult = f_close(&fp);

  IS_EQUAL(ffResult, FR_OK);

  // get file status
  FILINFO finfo;
  ffResult = f_stat(filepath, &finfo);

  // read back
  uint32_t bytesRead = 0;
  openFlags = FA_READ;
  ffResult = f_open(&fp, filepath, openFlags);
  IS_EQUAL(ffResult, FR_OK);

  ffResult = f_read(&fp, (void*) readbackBuffer, bytesWritten, (UINT*) &bytesRead);
  f_close(&fp);
  f_mount(NULL, "", 1); // unmount when not needed

  IS_EQUAL(ffResult, FR_OK);
  IS_EQUAL(bytesWritten, bytesRead);
  STR_EQ(data,readbackBuffer);

}END_TEST

TEST(CreateBigFile_FileSize_Correct){
  char data[256]="...............\n"
      "I will not waste chalk.\n"
      "I will not skateboard in the halls.\n"
      "I will not instigate a revolution.\n"
      "I did not see elvis.\n"
      "Garlic gum is not funny.\n"
      "They are laughing at me, not with me.\n"
      "I will not yell FIRE in a crowded classroom.\n"
      "...............\n";
  char readBuffer[256];
  TCHAR* filepath = "bart_chalk.txt";
  uint16_t iterations = 8192;
  uint16_t counts = 0;

  FATFS fs;
  FATFS* fsPtr = &fs;
  FRESULT ffResult = f_mount(fsPtr, "", 1); // always mount fs

  FIL       fp;
  int       openFlags = FA_WRITE | FA_CREATE_ALWAYS;

  ffResult = FR_DISK_ERR;

  ffResult = f_open(&fp, filepath, openFlags);

  for(uint16_t i = 0; i < iterations; i++){
      uint16_t  bytesWritten = 0;

      ffResult = f_write(&fp, (const void*) data, 256, (UINT*) &bytesWritten);
      if (ffResult != FR_OK){
        break;
      }
      counts++;
  }

  f_close(&fp);

  IS_EQUAL(iterations, counts);
  IS_EQUAL(ffResult, FR_OK);

  // read back
  uint32_t bytesRead = 0;
  openFlags = FA_READ;
  ffResult = f_open(&fp, filepath, openFlags);
  IS_EQUAL(ffResult, FR_OK);

  ffResult = f_read(&fp, (void*) readBuffer, 256, (UINT*) &bytesRead);
  f_close(&fp);

  IS_EQUAL(ffResult, FR_OK);
  IS_EQUAL(fp.obj.objsize, iterations * 256);
  ARR_EQ((const uint8_t*)data, (const uint8_t*)readBuffer);

  f_mount(NULL, "", 1); // unmount when not needed
}END_TEST

TEST(FatFs_writeFile_ReadbackOk){
  // create dummy file on fatfs
  const char* filepath = "logfile.log";
  const char* text = "This is a log.";
  char readBuffer[14];

  FRESULT ffResult = FATFS_writeFile(filepath, (char*) text, 14, true);
  IS_EQUAL(ffResult, FR_OK);

  ffResult = FATFS_readFile(filepath, readBuffer, 14);
  IS_EQUAL(ffResult, FR_OK);

  STR_EQ(text, (const char*) readBuffer);
}END_TEST

TEST_GROUP(FatFs){
	INCLUDE_TEST(CreateFile_Readback_Ok);
	INCLUDE_TEST(CreateBigFile_FileSize_Correct)
  INCLUDE_TEST(FatFs_writeFile_ReadbackOk);
}END_GROUP
