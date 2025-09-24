/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file   fatfs.c
  * @brief  Code for fatfs applications
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

#include <stdbool.h>
#include "RX8900SA/rx8900sa.h"
#include <stdio.h>
#include "MX25L3233F/mx25l3233f.h"
/* USER CODE END Header */
#include "fatfs.h"

uint8_t retUSER;    /* Return value for USER */
char USERPath[4];   /* USER logical drive path */
FATFS USERFatFS;    /* File system object for USER logical drive */
FIL USERFile;       /* File object for USER */

/* USER CODE BEGIN Variables */
#define DEVICE_ID 0
#define MOUNT_IMMEDIATELY 1

static bool isInitialized = false;

// function prototypes
static FRESULT _createFS(void);
/* USER CODE END Variables */

void MX_FATFS_Init(void)
{
  /*## FatFS: Link the USER driver ###########################*/
  retUSER = FATFS_LinkDriver(&USER_Driver, USERPath);

  /* USER CODE BEGIN Init */
  /* additional user code for init */
  /* USER CODE END Init */
}

/**
  * @brief  Gets Time from RTC
  * @param  None
  * @retval Time in DWORD
  */
DWORD get_fattime(void)
{
  /* USER CODE BEGIN get_fattime */

    RX8900SA_DateTime dateTime;
    RX8900SA_ReadTimeDate(&dateTime);

    return (DWORD)(dateTime.year - 80) << 25 |
           (DWORD)(dateTime.month + 1) << 21 |
           (DWORD) dateTime.day << 16 |
           (DWORD) dateTime.hour << 11 |
           (DWORD) dateTime.minute << 5 |
           (DWORD) dateTime.second >> 1;

  /* USER CODE END get_fattime */
}

/* USER CODE BEGIN Application */

static FRESULT _createFS(void) {

	uint8_t workBuffer[_MAX_SS];

	// try mount, if fails, create filesystem
	FRESULT result = f_mount(&USERFatFS, USERPath, MOUNT_IMMEDIATELY);

	if ( result != FR_OK ){
	  result = f_mkfs(USERPath, FM_FAT, _MAX_SS, &workBuffer, _MAX_SS);
	  if( result == FR_OK ){
		  result = f_mount(&USERFatFS, USERPath, MOUNT_IMMEDIATELY);
	  } else {
		  // problem with NOR flash?
		  // delete sectors belonging to the partition
		  FATFS_eraseFS();
		  result = f_mkfs(USERPath, FM_FAT, _MAX_SS, &workBuffer, _MAX_SS);
	  }
	}

	isInitialized = (result == FR_OK);

	result = f_mount(NULL, USERPath, MOUNT_IMMEDIATELY);

	return result;
}

FRESULT FATFS_Init(void){
	  return _createFS();
}

FRESULT FATFS_readFile(const char* filepath, char* readBuffer, uint32_t length){
  FATFS fs;
  FATFS* fsPtr = &fs;
  FRESULT ffResult = f_mount(fsPtr, "", 1); // always mount fs

  if ( ffResult != FR_OK )
  {
      return ffResult;
  }

  FIL       fp;
  ffResult = f_open(&fp, filepath, FA_OPEN_ALWAYS | FA_READ);
  uint32_t  fileSize = f_size(&fp);

  if ( ffResult != FR_OK )
  {
      f_mount(NULL, "", 1); // unmount
      return ffResult;
  }

  if ( length > fileSize ){
    f_close(&fp);
    f_mount(NULL, "", 1); // unmount
    return FR_INVALID_PARAMETER;
  }

  uint32_t  bytesRead = 0;
  ffResult = f_read(&fp, (void*) readBuffer, fileSize, (UINT*) &bytesRead);
  f_close(&fp);
  if ( ffResult != FR_OK )
  {
      return ffResult;
  }

  f_mount(NULL, "", 1); // unmount
  return ffResult;
}

FRESULT FATFS_writeFile(const char* filepath, char* buffer, uint32_t length, bool overwrite){
  FATFS fs;
  FATFS* fsPtr = &fs;
  FRESULT ffResult = f_mount(fsPtr, "", 1); // always mount fs

  if ( ffResult != FR_OK )
  {
      return ffResult;
  }

  FIL       fp;
  int       openFlags = FA_WRITE | (( overwrite )? FA_CREATE_ALWAYS: FA_OPEN_APPEND );
  ffResult = f_open(&fp, filepath, openFlags);

  if ( ffResult != FR_OK )
  {
      f_mount(NULL, "", 1); // unmount
      return ffResult;
  }

  uint32_t bytesWritten = 0;
  ffResult = f_write(&fp,(const void*) buffer, length, (UINT*) &bytesWritten);
  if ( ffResult != FR_OK )
  {
      // close fatfs file
      f_close(&fp);
      f_mount(NULL, "", 1); // unmount
      return ffResult;
  }
  ffResult = f_sync(&fp);

  // close fatfs file
  ffResult = f_close(&fp);
  f_mount(NULL, "", 1); // unmount

  return ffResult;
}

FRESULT FATFS_removeFile(const char* filepath){
  FATFS fs;
  FATFS* fsPtr = &fs;
  FRESULT ffResult = f_mount(fsPtr, "", 1); // always mount fs

  if ( ffResult != FR_OK )
  {
      return ffResult;
  }

  ffResult = f_unlink(filepath);
  f_mount(NULL, "", 1); // unmount

  return ffResult;
}

FRESULT FATFS_eraseFS(void){
	/* Erases sectors on NOR flash belonging to fatfs partition */
	// startAddress = 0x000000;
	// endAddress = 0x24E000; // littlefs start address

	// 24 x 64kB blocks
	for (uint8_t b=0; b<24;b++){
		if(MX25L32_EraseBlock64kb(b * 0x10000) != FLASHDRV_OK ){
			return FR_DISK_ERR;
		}
	}
	// E x 4kB sectors
	for (uint8_t s=0; s<14; s++){
		if (MX25L32_EraseSector(0x24000 + s*0x1000) != FLASHDRV_OK){
			return FR_DISK_ERR;
		}
	}
	return FR_OK;
}
/* USER CODE END Application */
