/*!*************************************************************************************************
 * @file fs_layer.h
 *
 * @brief Implements the interface between littleFS and external flash IC. It needs at least 4kB
 * 		  of reserved RAM.
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

#ifndef FS_HELPER_H
#define FS_HELPER_H

/* Includes
   -----------------------------------------------------------------------------------------------*/
#include <stdbool.h>
#include <stdint.h>

#include "lfs.h"

/* Defines
   -----------------------------------------------------------------------------------------------*/
#define FS_BLOCK_COUNT 417

/* Typedefs
   -----------------------------------------------------------------------------------------------*/
typedef struct lfs_config lfs_config;
typedef struct lfs_info lfs_info;

/* Private variables
   -----------------------------------------------------------------------------------------------*/

/* Private interface
   -----------------------------------------------------------------------------------------------*/

/* Public interface
   -----------------------------------------------------------------------------------------------*/

// define this functions in user code, to get notified when a USB stick is mounted or disconnected
void OnUSBMount(void);

//
void OnUSBUnmount(void);

//
int FS_Init(void);

//
lfs_t* FS_getFS(void);

//
lfs_config* FS_getFSConfig(void);

//
int FS_copyFileToFatfs(const char* filepath);

//
int FS_copyFileToFlash(const char* filepath);

//
void FS_setUSBMountStatus(bool mounted);

//
bool FS_getUSBMountStatus(void);

int FS_mkdir(const char* dirPath);

//
int FS_mv(const char* oldPath, const char* newPath);

//
int FS_mount(void);

//
int FS_umount(void);

//
int FS_format(void);

//
int FS_rm(const char* filepath, bool force);

//
int FS_write(const char* filepath, const char* buffer, uint32_t size, bool append);

//
int FS_writeStr(const char* filepath, const char* buffer, bool append);

//
int FS_read(const char* filepath, char* buffer, uint32_t size);

//
int FS_ls(const char* dirpath);

//
int FS_eraseFs(void);

//
int32_t FS_fileSize(const char* filepath);

//
int32_t FS_size(void);

//
int32_t FS_free(void);

//
bool FS_isReady(void);

#endif
