/*!*************************************************************************************************
 * @file fs_layer.c
 *
 * @brief Implements the interface between littleFS and external flash IC
 *
 * @author Enovation Controls.
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

#include "fs_layer.h"
#include "MX25L3233F/mx25l3233f.h"
#include "string.h"
#include <ff.h>

/* Defines
   -----------------------------------------------------------------------------------------------*/
#define LITTLEFS_START_ADDRESS 0x24E000 // 417 blocks @4kB offset for FATFS partition

/* Typedefs
   -----------------------------------------------------------------------------------------------*/

/* Private variables
   -----------------------------------------------------------------------------------------------*/
static volatile bool  usbMounted = false;
static lfs_t          lfs;
static lfs_file_t     file;
static lfs_config     cfg;
static uint8_t        lfs_readBuf[1024];
static uint8_t        lfs_progBuf[1024];
static uint8_t        lfs_lookaheadBuf[128];

static bool           fsMounted = false;

/* Private interface
   -----------------------------------------------------------------------------------------------*/

static int _readBlock(const struct lfs_config *c, lfs_block_t block,
                      lfs_off_t off, void *buffer, lfs_size_t size)
{
    uint32_t         address = block * c->block_size + LITTLEFS_START_ADDRESS + off;
    FlashDrv_Status  status = MX25L32_ReadX(address, (uint8_t*) buffer, size);
    if ( status == FLASHDRV_OK )
    {
        return LFS_ERR_OK;
    }
    return LFS_ERR_IO;
}

/*!*************************************************************************************************
 * @brief programBlock
 * @param c config
 * @param block block number
 * @param off offset
 * @param buffer data
 * @param size
 * @return lfs error code
 **************************************************************************************************/
static int _programBlock(const struct lfs_config *c, lfs_block_t block,
                         lfs_off_t off, const void *buffer, lfs_size_t size)
{
    uint32_t  address = block * c->block_size + LITTLEFS_START_ADDRESS + off;

    if ( MX25L32_PageProgramX(address, (uint8_t*) buffer, size) == FLASHDRV_OK )
    {
        return LFS_ERR_OK;
    }
    return LFS_ERR_IO;
}

/*!*************************************************************************************************
 * @brief eraseBlock
 * @param c config
 * @param block number of block
 * @return lfs error code
 **************************************************************************************************/
static int _eraseBlock(const struct lfs_config *c, lfs_block_t block)
{
    uint32_t  address = block * c->block_size + LITTLEFS_START_ADDRESS;

    if ( MX25L32_EraseSector(address) == FLASHDRV_OK )
    {
        return LFS_ERR_OK;
    }
    return LFS_ERR_IO;
}

/*!*************************************************************************************************
 * @brief _eraseFS
 * @param c cfg
 * @return
 **************************************************************************************************/
static int _eraseFS(const struct lfs_config *c)
{
    int  result = LFS_ERR_IO;
    for ( uint16_t b = 0; b < FS_BLOCK_COUNT; b++ )
    {
        result = _eraseBlock(c, b);

        if ( result != LFS_ERR_OK )
        {
            return result;
        }
    }
    return result;
}

/*!*************************************************************************************************
 * @brief syncFS
 * @param c config
 * @return lfs error code
 **************************************************************************************************/
static int _syncFS(const struct lfs_config *c)
{
    return LFS_ERR_OK;
}

/*!*************************************************************************************************
 * @brief _removeDirFiles removes all files in directory
 * @param path
 * @return lfs error code
 **************************************************************************************************/
static int _removeDirFiles(const char *path)
{
    int        result = LFS_ERR_INVAL;
    lfs_info   fileInfo;

    // open directory
    lfs_dir_t  dir;
    if (( result = lfs_dir_open(&lfs, &dir, path)) != LFS_ERR_OK )
    {

        return result;
    }

    char       filepathBuffer[512] = { 0 }; //max file and directory name buffer length is 256

    // while files : delete files
    while (( result = lfs_dir_read(&lfs, &dir, &fileInfo)) > 0 )
    {
        if ( fileInfo.type == LFS_TYPE_REG )
        {
            strcat(filepathBuffer, path);
            strcat(filepathBuffer, "/");
            strcat(filepathBuffer, fileInfo.name);

            if (( result = lfs_remove(&lfs, filepathBuffer)) != LFS_ERR_OK )
            {
                break;
            }
        }
    }

    result = lfs_dir_close(&lfs, &dir);
    return result;
}

/*!*************************************************************************************************
 * @brief _checkForCorruptFS
 * @param errorCode
 * @return
 **************************************************************************************************/
static int _checkForCorruptFS(int errorCode)
{
    // a corrupted filesystem is dangerous, as further littlefs operations on one
    // can result in crashes, undefined behavior or a hung system
    int  result = errorCode;

    if ( errorCode == LFS_ERR_CORRUPT )
    {
        FS_umount();
        lfs_format(&lfs, &cfg);
        result = FS_mount();
    }
    return result;
}

/* Public interface
   -----------------------------------------------------------------------------------------------*/

/*!*************************************************************************************************
 * @brief FS_Init initializes the littlefs filesystem and mounts it
 * @return lfs error code
 **************************************************************************************************/
int FS_Init(void)
{
    cfg.context = NULL;

    // block device operations
    cfg.read = _readBlock;
    cfg.prog = _programBlock;
    cfg.erase = _eraseBlock;
    cfg.sync = _syncFS;

    // block device configuration
    cfg.read_size = 1;         // no limit actually 1 - max-capacity
    cfg.prog_size = 1;         // no limit actually 1-256 per command
    cfg.block_size = 4096;
    cfg.block_count = FS_BLOCK_COUNT;
    cfg.block_cycles = 500;
    cfg.lookahead_size = 128;
    cfg.cache_size = 1024;     //ToDo BSH 1024;
    cfg.compact_thresh = 0;

    cfg.read_buffer = lfs_readBuf;
    cfg.prog_buffer = lfs_progBuf;
    cfg.lookahead_buffer = lfs_lookaheadBuf;

    cfg.name_max = 0;
    cfg.file_max = 0;
    cfg.attr_max = 0;
    cfg.metadata_max = 0;
    cfg.inline_max = 0;     // required

    int  result = FS_mount();

    return result;
}

/*!*************************************************************************************************
 * @brief FS_getFS
 * @return filesystem object
 **************************************************************************************************/
lfs_t* FS_getFS(void)
{
    return &lfs;
}
/*!*************************************************************************************************
 * @brief FS_getFSConfig
 * @return
 **************************************************************************************************/
lfs_config* FS_getFSConfig(void)
{
    return &cfg;
}

/*!*************************************************************************************************
 * @brief FS_copyFileToFatfs
 * @param filepath
 * @return lfs error code if < 0, fatfs error code if > 0, 0 if successful
 **************************************************************************************************/
int FS_copyFileToFatfs(const char *filepath)
{
    // if usb is mounted, potential conflict between file access from USB host and from device
    if ( usbMounted )
    {
        return FR_NOT_READY;
    }

    // check if file exists on littlefs
    lfs_info  info;
    int       lfsOpenFlags = LFS_O_RDONLY;
    int       result = lfs_stat(&lfs, filepath, &info);
    char      readBuffer[128];

    if ( info.type == LFS_TYPE_DIR )
    {
        return LFS_ERR_ISDIR;
    }

    if ( result != LFS_ERR_OK )
    {
        // file already exists
        return result;
    }

    result = _checkForCorruptFS(result);

    // open file
    result = lfs_file_open(&lfs, &file, filepath, lfsOpenFlags);
    if ( result != LFS_ERR_OK )
    {
        result = _checkForCorruptFS(result);
        return result;
    }

    uint32_t  fileSize = lfs_file_size(&lfs, &file);
    if ( fileSize < 0 )
    {
        return fileSize;                          // error code
    }
    //-----

    //-----
    // create file on fatfs
    FATFS     fs;
    FATFS *   fsPtr = &fs;
    FRESULT   ffResult = f_mount(fsPtr, "", 1);   // always mount fs

    FIL       fp;
    int       openFlags = FA_WRITE | FA_CREATE_ALWAYS;
    ffResult = f_open(&fp, filepath, openFlags);

    if ( ffResult != FR_OK )
    {
        f_mount(NULL, "", 1);                     // unmount when not needed
        return ffResult;
    }

    // transfer file data
    while ( fileSize != 0 )
    {
        uint32_t  bytesRead = 0;
        uint32_t  bytesWritten = 0;
        if ( fileSize < 128 )
        {
            bytesRead = lfs_file_read(&lfs, &file, readBuffer, fileSize);
        }
        else
        {
            bytesRead = lfs_file_read(&lfs, &file, readBuffer, 128);
        }

        ffResult = f_write(&fp, (const void*) &( readBuffer ), bytesRead,
                           (UINT*) &bytesWritten);
        if ( ffResult != FR_OK )
        {
            // close fatfs file
            f_close(&fp);
            f_mount(NULL, "", 1);             // unmount when not needed

            // close lfs file
            result = lfs_file_close(&lfs, &file);

            return ffResult;
        }

        fileSize -= bytesRead;
        f_sync(&fp);
    }

    // close fatfs file
    ffResult = f_close(&fp);
    f_mount(NULL, "", 1);     // unmount when not needed

    // close lfs file
    result = lfs_file_close(&lfs, &file);

    return ( result != LFS_ERR_OK ) ? result : ffResult;
}

/*!*************************************************************************************************
 * @brief FS_copyFileToFlash copies a file from fatfs to littlefs flash
 * @param filepath only supports files in root directory
 * @return lfs error code if < 0, fatfs error code if > 0, 0 if successful
 **************************************************************************************************/
int FS_copyFileToFlash(const char *filepath)
{
    // if usb is mounted, potential conflict between file access from USB host and from device
    if ( usbMounted )
    {
        return FR_NOT_READY;
    }

    // check if file exists on littlefs
    lfs_info  info;
    int       lfsOpenFlags = LFS_O_TRUNC | LFS_O_WRONLY | LFS_O_CREAT;
    int       result = lfs_stat(&lfs, filepath, &info);
    char      readBuffer[128];

    if ( info.type == LFS_TYPE_DIR )
    {
        return LFS_ERR_ISDIR;
    }

    result = _checkForCorruptFS(result);

    // create file
    result = lfs_file_open(&lfs, &file, filepath, lfsOpenFlags);
    if ( result != LFS_ERR_OK )
    {
        result = _checkForCorruptFS(result);
        return result;
    }
    //-----

    //-----
    // open file on fatfs
    FATFS     fs;
    FATFS *   fsPtr = &fs;
    FRESULT   ffResult = f_mount(fsPtr, "", 1);   // always mount fs

    FIL       fp;
    ffResult = f_open(&fp, filepath, FA_OPEN_ALWAYS | FA_READ);
    uint32_t  fileSize = f_size(&fp);

    if ( ffResult != FR_OK )
    {
        lfs_file_close(&lfs, &file);
        f_mount(NULL, "", 1);                     // unmount when not needed
        return ffResult;
    }

    // transfer file data
    while ( fileSize != 0 )
    {
        uint32_t  bytesRead = 0;
        uint32_t  bytesWritten = 0;
        if ( fileSize < 128 )
        {
            ffResult = f_read(&fp, (void*) readBuffer, fileSize,
                              (UINT*) &bytesRead);
        }
        else
        {
            ffResult = f_read(&fp, (void*) readBuffer, 128, (UINT*) &bytesRead);
        }

        bytesWritten = lfs_file_write(&lfs, &file, readBuffer, bytesRead);
        if ( bytesWritten < LFS_ERR_OK )
        {
            result = bytesWritten;

            // close fatfs file
            f_close(&fp);
            f_mount(NULL, "", 1);     // unmount when not needed

            // close lfs file
            lfs_file_close(&lfs, &file);
            result = _checkForCorruptFS(result);

            return result;            // contains error code
        }

        fileSize -= bytesRead;
    }

    // close fatfs file
    ffResult = f_close(&fp);
    f_mount(NULL, "", 1);             // unmount when not needed

    // close lfs file
    result = lfs_file_close(&lfs, &file);
    result = _checkForCorruptFS(result);

    return ( result != LFS_ERR_OK ) ? result : ffResult;
}

/*!*************************************************************************************************
 * @brief FS_setUSBMountStatus
 * @param mounted
 **************************************************************************************************/
void FS_setUSBMountStatus(bool mounted)
{
    usbMounted = mounted;
}

/*!*************************************************************************************************
 * @brief FS_getUSBMountStatus
 * @return
 **************************************************************************************************/
bool FS_getUSBMountStatus(void)
{
    return usbMounted;
}

/*!*************************************************************************************************
 * @brief FS_mkdir
 * @param dirPath
 * @return lfs error code
 **************************************************************************************************/
int FS_mkdir(const char *dirPath)
{
    return lfs_mkdir(&lfs, dirPath);
}

/*!*************************************************************************************************
 * @brief FS_mv
 * @param oldPath
 * @param newPath
 * @return lfs error code
 **************************************************************************************************/
int FS_mv(const char *oldPath, const char *newPath)
{
    return lfs_rename(&lfs, oldPath, newPath);
}

/*!*************************************************************************************************
 * @brief FS_mount
 * @return lfs error code
 **************************************************************************************************/
int FS_mount(void)
{
    if ( fsMounted )
    {
        return LFS_ERR_OK;         // aldready mounted, prevent double-mount, it will corrupt the fs
    }

    // try to mount, format if it fails
    int  result = lfs_mount(&lfs, &cfg);
    if ( result != LFS_ERR_OK )
    {
        //        printf("Filesystem mount failed. Erasing chip and re-formatting to prevent
        // unstable fs.\n");
        _eraseFS(&cfg);
        if (( result = lfs_format(&lfs, &cfg)) == LFS_ERR_OK )
        {
            result = lfs_mount(&lfs, &cfg);
            if ( result == LFS_ERR_OK )
            {
                fsMounted = true;
            }
        }
    }

    return result;
}

/*!*************************************************************************************************
 * @brief FS_umount
 * @return lfs error code
 **************************************************************************************************/
int FS_umount(void)
{
    if ( !fsMounted )
    {
        return LFS_ERR_OK;
    }

    int  result = lfs_unmount(&lfs);

    if ( result == LFS_ERR_OK )
    {
        fsMounted = false;
    }
    return result;
}

/*!*************************************************************************************************
 * @brief FS_format
 * @return lfs error code
 **************************************************************************************************/
int FS_format(void)
{
    return lfs_format(&lfs, &cfg);
}

/*!*************************************************************************************************
 * @brief FS_rm
 * @param filepath must be absolute path
 * @param force if set to true, it will remove directories, even when it contains other files
 * @return lfs error code
 **************************************************************************************************/
int FS_rm(const char *filepath, bool force)
{
    // check if dir
    lfs_info  info;
    int       result = lfs_stat(&lfs, filepath, &info);

    if ( info.type == LFS_TYPE_DIR )
    {
        result = lfs_remove(&lfs, filepath);

        if ( result == LFS_ERR_NOTEMPTY && force )
        {
            _removeDirFiles(filepath);
            // now delete empty dir
            result = lfs_remove(&lfs, filepath);
        }
    }
    else
    {
        result = lfs_remove(&lfs, filepath);
    }

    return result;
}

/*!*************************************************************************************************
 * @brief FS_writeFile
 * @param filepath
 * @return lfs error code
 **************************************************************************************************/
int FS_write(const char *filepath, const char *buffer, uint32_t size,
             bool append)
{
    int          result = LFS_ERR_IO;
    int          lfsOpenFlags = LFS_O_RDWR | LFS_O_CREAT; // make sure to have
    // write permission

    // check if file
    lfs_info     info;
    result = lfs_stat(&lfs, filepath, &info);

    if ( info.type == LFS_TYPE_DIR )
    {
        return LFS_ERR_ISDIR;
    }

    // check size available and required.
    uint32_t     freeSpace = FS_free() * cfg.block_size;
    bool         hasSpace = false;

    if ( append )
    {
        // free space >= size
        hasSpace = ( freeSpace > size ) ? true : false;
        lfsOpenFlags |= LFS_O_APPEND;
    }
    else if ( size > info.size )
    {
        hasSpace = ( freeSpace >= ( size - info.size )) ? true : false;
    }
    else
    {
        // overwrites existing file, therefore has enough space
        hasSpace = true;
    }

    if ( !hasSpace )
    {
        return LFS_ERR_NOSPC;
    }

    // open file
    result = lfs_file_open(&lfs, &file, filepath, lfsOpenFlags);

    if ( result != LFS_ERR_OK )
    {
        result = _checkForCorruptFS(result);
        return result;
    }

    lfs_ssize_t  written = lfs_file_write(&lfs, &file, buffer, size);
    result = ( written < 0 ) ? written : LFS_ERR_OK;

    if ( result != LFS_ERR_OK )
    {
        result = _checkForCorruptFS(result);
    }

    lfs_file_close(&lfs, &file);

    return result;
}

/*!*************************************************************************************************
 * @brief FS_writeStr
 * @param filepath
 * @param buffer buffer with string
 * @param append when true, data is appended - otherwise written to start of file
 * @return lfs error code
 **************************************************************************************************/
int FS_writeStr(const char *filepath, const char *buffer, bool append)
{
    return FS_write(filepath, buffer, strlen(buffer), append);
}

/*!*************************************************************************************************
 * @brief FS_readFile
 * @param filepath
 * @param buffer
 * @param size
 * @return lfs error code
 **************************************************************************************************/
int FS_read(const char *filepath, char *buffer, uint32_t size)
{
    int          result = LFS_ERR_IO;
    int          lfsOpenFlags = LFS_O_RDONLY; // make sure to have read permissions

    // check if file exists
    lfs_info     info;
    result = lfs_stat(&lfs, filepath, &info);

    if ( info.type == LFS_TYPE_DIR )
    {
        return LFS_ERR_ISDIR;
    }

    if ( result != LFS_ERR_OK )
    {
        result = _checkForCorruptFS(result);
        return result;
    }

    // open file
    if (( result = lfs_file_open(&lfs, &file, filepath, lfsOpenFlags))
        != LFS_ERR_OK )
    {
        result = _checkForCorruptFS(result);
        return result;
    }

    // read
    lfs_ssize_t  read = lfs_file_read(&lfs, &file, buffer, size);
    result = ( read < 0 ) ? read : result;
    result = _checkForCorruptFS(result);

    // close
    result = lfs_file_close(&lfs, &file);
    result = _checkForCorruptFS(result);

    return result;
}

/*!*************************************************************************************************
 * @brief FS_ls list files and directories and prints them to output
 * @param dirpath
 * @return
 **************************************************************************************************/
int FS_ls(const char *dirpath)
{
    lfs_dir_t  lfsDir;
    lfs_info   lfsInfo;
    int        result = lfs_dir_open(&lfs, &lfsDir, dirpath);

    if ( result != LFS_ERR_OK )
    {
        result = _checkForCorruptFS(result);
        return result;
    }

    printf("\n%s\n", dirpath);

    while ( true )
    {
        result = lfs_dir_read(&lfs, &lfsDir, &lfsInfo);

        if (( result == LFS_ERR_OK ) || ( result < LFS_ERR_OK ))
        {
            break;
        }

        switch ( lfsInfo.type )
        {
            case LFS_TYPE_REG:
                printf("reg ");
                break;
            case LFS_TYPE_DIR:
                printf("dir ");
                break;
            default:
                printf("?   ");
                break;
        }

        printf("%s\n", lfsInfo.name);
    }
    result = lfs_dir_close(&lfs, &lfsDir);
    result = _checkForCorruptFS(result);

    return result;
}

/*!*************************************************************************************************
 * @brief FS_fileSize
 * @param filepath
 * @return lfs error code or file-size if > 0
 **************************************************************************************************/
int32_t FS_fileSize(const char *filepath)
{

    // check if file
    lfs_info  info;
    int       result = lfs_stat(&lfs, filepath, &info);

    if ( info.type == LFS_TYPE_DIR )
    {
        return LFS_ERR_ISDIR;
    }

    if ( result != LFS_ERR_OK )
    {
        result = _checkForCorruptFS(result);
        return result;
    }

    // get file structure
    int       lfsOpenFlags = LFS_O_RDONLY;

    // open file
    result = lfs_file_open(&lfs, &file, filepath, lfsOpenFlags);
    int32_t   size = 0;
    if ( result == LFS_ERR_OK )
    {
        size = lfs_file_size(&lfs, &file);
        result = lfs_file_close(&lfs, &file);
    }

    result = _checkForCorruptFS(result);

    return size;
}

/*!*************************************************************************************************
 * @brief FS_size
 * @return number of used blocks or negative lfs error code
 **************************************************************************************************/
int32_t FS_size(void)
{
    return lfs_fs_size(&lfs);
}

/*!*************************************************************************************************
 * @brief FS_free
 * @return number of free blocks or negative lfs error code
 **************************************************************************************************/
int32_t FS_free(void)
{
    uint32_t  freeBlocks = 0;
    int32_t   usedBlocks = lfs_fs_size(&lfs);

    if ( usedBlocks < 0 )
    {
        // error
        return usedBlocks;
    }

    freeBlocks = cfg.block_count - usedBlocks;
    return freeBlocks;
}

/*!*************************************************************************************************
 * @brief FS_isReady
 * @return
 **************************************************************************************************/
bool FS_isReady(void)
{
    FlashDrv_Status  status;
    status = MX25L32_GetState(&status);
    bool             isReady = false;

    if ( status == FLASHDRV_OK )
    {
        isReady = true;
    }
    return isReady;
}

/*!*************************************************************************************************
 * @brief FS_eraseFs erases sectors belonging to littlefs on the NOR flash
 * @return
 **************************************************************************************************/
int FS_eraseFs(void)
{
    return _eraseFS(&cfg);
}
