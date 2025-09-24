/*!*************************************************************************************************
 * @file fs_layer_test.c
 *
 * @brief Contains tests for the file system layer
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
#include "fs_layer.h"
#include "ff.h"
#include "MX25L3233F/mx25l3233f.h"
#include "fatfs.h"

/* Defines
   -----------------------------------------------------------------------------------------------*/

/* Typedefs
   -----------------------------------------------------------------------------------------------*/

/* Private variables
   -----------------------------------------------------------------------------------------------*/

/* Private interface
   -----------------------------------------------------------------------------------------------*/

static void setup(){
	FS_mount();
}

static void teardown(){
	FS_umount();
	FS_format();
}

static void groupTeardown(){
}

/* Public interface
   -----------------------------------------------------------------------------------------------*/

TEST(MakeDir_Ok){
	const char* dirpath = "TestDirectory";

	int result = FS_mkdir(dirpath);
	IS_TRUE(result == LFS_ERR_OK);
	lfs_t* lfs = FS_getFS();
    lfs_dir_t  lfsDir;

    result = lfs_dir_open(lfs, &lfsDir, "/TestDirectory");
    lfs_dir_close(lfs, &lfsDir);
    IS_TRUE(result == LFS_ERR_OK);

	// cleanup
    result = FS_rm("/TestDirectory", true);
    IS_TRUE(result == LFS_ERR_OK);
}END_TEST

TEST(GetFS_Size_Ok){
	// free formatted fs should have no used space but 2 dir blocks
	int32_t size = FS_size();
	IS_EQUAL(2, size);
}END_TEST

TEST(GetFS_Free_Ok){
	// free formatted fs should have maximal block count
	int32_t size = FS_free();
	IS_EQUAL(FS_BLOCK_COUNT - 2, size);
}END_TEST

TEST(WriteFile_Ok){
	const char* filepath = "testfile.txt";
	const char* text = "This is a test file.";
	int result = FS_write(filepath, text, 21, false);
	IS_TRUE(result == LFS_ERR_OK);

	// check if file got created
	lfs_t* lfs = FS_getFS();
    lfs_file_t  lfsFile;

    result = lfs_file_open(lfs, &lfsFile, filepath, LFS_O_RDONLY);
	IS_TRUE(result == LFS_ERR_OK);

	// check file content
	char contentBuffer[21];
	result = lfs_file_read(lfs, &lfsFile, &contentBuffer, 21);
	IS_TRUE(result > 0);

	STR_EQ(text,(const char*) &contentBuffer);

	// cleanup
	lfs_file_close(lfs, &lfsFile);
	result = FS_rm("/testfile.txt", true);
	IS_TRUE(result == LFS_ERR_OK);

}END_TEST

TEST(WriteString_File_Ok){
	const char* filepath = "testfile.txt";
	const char* text = "This is a test file.";
	int result = FS_writeStr(filepath, text, false);
	IS_TRUE(result == LFS_ERR_OK);

	// check if file got created
	lfs_t* lfs = FS_getFS();
    lfs_file_t  lfsFile;

    result = lfs_file_open(lfs, &lfsFile, filepath, LFS_O_RDONLY);
	IS_TRUE(result == LFS_ERR_OK);

	// check file content
	char contentBuffer[21];
	result = lfs_file_read(lfs, &lfsFile, &contentBuffer, 21);
	IS_TRUE(result > 0 );

	STR_EQ(text,(const char*) &contentBuffer);

	// cleanup
	lfs_file_close(lfs, &lfsFile);
	result = FS_rm("/testfile.txt", true);
	IS_TRUE(result == LFS_ERR_OK);

}END_TEST

TEST(WriteFile_OverExistingFile_Ok){
	const char* filepath = "testfile.txt";
	const char* text = "This is a test file.";
	const char* alternativeText = "This is other text!";
	int result = FS_writeStr(filepath, text, false);
	IS_TRUE(result == LFS_ERR_OK);

	result = FS_writeStr(filepath, alternativeText, false);
	IS_TRUE(result == LFS_ERR_OK);

	// check if file got created
	lfs_t* lfs = FS_getFS();
    lfs_file_t  lfsFile;

    result = lfs_file_open(lfs, &lfsFile, filepath, LFS_O_RDONLY);
	IS_TRUE(result == LFS_ERR_OK);

	// check file content
	char contentBuffer[20];
	result = lfs_file_read(lfs, &lfsFile, &contentBuffer, 20);
	IS_TRUE(result > 0 );

	STR_EQ(alternativeText,(const char*) &contentBuffer);

	// cleanup
	lfs_file_close(lfs, &lfsFile);
	result = FS_rm("/testfile.txt", true);
	IS_TRUE(result == LFS_ERR_OK);

}END_TEST

TEST(WriteString_FileAppend_Ok){
	const char* filepath = "testfile.txt";
	const char* text = "This is a test file.";
	int result = FS_writeStr(filepath, text, false);
	IS_TRUE(result == LFS_ERR_OK);

	result = FS_writeStr(filepath, "Append this.", true);

	// check if file got created
	lfs_t* lfs = FS_getFS();
    lfs_file_t  lfsFile;

    result = lfs_file_open(lfs, &lfsFile, filepath, LFS_O_RDONLY);
	IS_TRUE(result == LFS_ERR_OK);

	// check file content
	char contentBuffer[32];
	result = lfs_file_read(lfs, &lfsFile, &contentBuffer, 32);
	IS_EQUAL(result, 32);

	char* ptr = strstr((const char*) &contentBuffer, "Append this.");
	IS_NEQUAL(ptr, NULL);

	lfs_file_close(lfs, &lfsFile);
	result = FS_rm("/testfile.txt", true);
	IS_TRUE(result == LFS_ERR_OK);

}END_TEST

TEST(ReadFile_Ok){
	const char* filepath = "testfile.txt";
	const char* text = "This is a test file.";
	int result = FS_write(filepath, text, 21, false);
	IS_TRUE(result == LFS_ERR_OK);

	// check file content
	char contentBuffer[21];
	result = FS_read(filepath, contentBuffer, 21);
	IS_TRUE(result == LFS_ERR_OK);
	STR_EQ(text,(const char*) &contentBuffer);

	result = FS_rm("/testfile.txt", true);
	IS_TRUE(result == LFS_ERR_OK);
}END_TEST

TEST(RemoveFile_Ok){
	const char* filepath = "testfile.txt";
	const char* text = "This is a test file.";
	int result = FS_write(filepath, text, 21, false);
	IS_TRUE(result == LFS_ERR_OK);

	result = FS_rm("/testfile.txt", false);
	IS_TRUE(result == LFS_ERR_OK);

	lfs_t* lfs = FS_getFS();
    lfs_file_t  lfsFile;
    result = lfs_file_open(lfs, &lfsFile, filepath, LFS_O_RDONLY);
	IS_TRUE(result != LFS_ERR_OK);

}END_TEST

TEST(WriteLargeContinuousFile){
	char buffer[4096];
	int32_t result = LFS_ERR_OK;
	uint32_t fileSize = 0;
	const char* filename = "longFile";
	uint16_t countOfBlocks = FS_BLOCK_COUNT -4;

    for ( int i = 0; i < 4096; i++ )
    {
        buffer[i] = 1 << ( i % 8 );
    }

    result = FS_write(filename, buffer, 4096, true);

    while(result == LFS_ERR_OK){
    	result = FS_write(filename, buffer, 4096, true);
    }

    // get file size
    fileSize = FS_fileSize(filename);

    result = FS_rm(filename, false);

    IS_TRUE( fileSize >= (4096 * countOfBlocks) );

    // cleanup
    FS_umount();
    FS_format();
}END_TEST

TEST(WriteMaxNumberOfFiles){
	char pageBuffer[4096];
	int result = LFS_ERR_OK;
	uint16_t numberOfFiles = 0;
	char filenameBuffer[32];
	const uint32_t fileLimit = FS_BLOCK_COUNT - 22;

    for ( int i = 0; i < 4096; i++ )
    {
        pageBuffer[i] = 1 << ( i % 8 );
    }

	// write files until no longer possible and print out number
	for (uint32_t i = 0; i<fileLimit; i++){
		// create new filename
		snprintf(filenameBuffer, 32, "writeTestFile_%d", numberOfFiles);
		result = FS_write(filenameBuffer, pageBuffer, 4096, false);
		if( result == LFS_ERR_OK ){
			numberOfFiles++;
		}else {
			break;
		}
	}

    IS_EQUAL(numberOfFiles,fileLimit);

    // cleanup
    FS_umount();
    FS_format();
}END_TEST

TEST(RemoveDir_NoForce_Ok){
	const char* dirpath = "TestDirectory";
	int result = FS_mkdir(dirpath);
	IS_TRUE(result == LFS_ERR_OK);

	result = FS_rm("/TestDirectory", false);
	IS_TRUE(result == LFS_ERR_OK);
}END_TEST

TEST(RemoveDir_NoForce_Fail){
	const char* dirpath = "TestDirectory";
	const char* filepath = "/TestDirectory/testfile.txt";
	const char* text = "This is a test file.";
	int result = FS_mkdir(dirpath);
	IS_TRUE(result == LFS_ERR_OK);

	lfs_dir_t dir;
	result = lfs_dir_open(FS_getFS(), &dir, "/TestDirectory");
	result = FS_write(filepath, text, 21, false);
	IS_TRUE(result == LFS_ERR_OK);
	lfs_dir_close(FS_getFS(), &dir);

	result = FS_rm(dirpath, false);
	IS_TRUE(result != LFS_ERR_OK);

	result = FS_rm(filepath, false);
	IS_TRUE(result == LFS_ERR_OK);

	result = FS_rm("/TestDirectory", false);
	IS_TRUE(result == LFS_ERR_OK);
}END_TEST

TEST(RemoveDir_Force_Ok){
	const char* dirpath = "TestDirectory";
	const char* filepath = "/TestDirectory/testfile.txt";
	const char* text = "This is a test file.";
	int result = FS_mkdir(dirpath);
	IS_TRUE(result == LFS_ERR_OK);

	result = FS_writeStr(filepath, text, false);
	IS_TRUE(result == LFS_ERR_OK);

	result = FS_rm(dirpath, true);
	IS_TRUE(result == LFS_ERR_OK);

	lfs_dir_t dir;
	result = lfs_dir_open(FS_getFS(), &dir, "/TestDirectory");
	IS_TRUE(result != LFS_ERR_OK);
}END_TEST

TEST(CopyFileToFatFs_FileOnLittleFs_NewFileOnFatFs){

  // create dummy file on littlefs
  const char* filepath = "logfile.log";
  const char* text = "This is a log.";
  char readBuffer[14];

  int result = FS_write(filepath, text, 14, true);
  IS_TRUE(result == LFS_ERR_OK);

  result = FS_copyFileToFatfs(filepath);
  IS_TRUE(result == LFS_ERR_OK);

  // read FatFS file
  FATFS_readFile(filepath, readBuffer, 14);

  STR_EQ(text,(const char*) readBuffer);

  FRESULT ffResult = FATFS_removeFile(filepath);
  IS_EQUAL(ffResult, FR_OK);


}END_TEST

TEST(CopyFileToFlash_FileOnFatFs_NewFileOnLittleFs){
  // create dummy file on fatfs
  const char* filepath = "logfile.log";
  const char* text = "This is a log.";
  char readBuffer[14];

  FRESULT ffResult = FATFS_writeFile(filepath, (char*) text, 14, true);
  IS_EQUAL(ffResult, FR_OK);

  uint8_t result = FS_copyFileToFlash(filepath);
  IS_EQUAL(result, LFS_ERR_OK);

  // readback file from littlefs
  result = FS_read(filepath, readBuffer, 14);
  IS_EQUAL(result, LFS_ERR_OK);
  STR_EQ(text, readBuffer);

  // cleanup
  ffResult = FATFS_removeFile(filepath);
  IS_EQUAL(ffResult, FR_OK);

}END_TEST


TEST_GROUP(FSLayer){
	INCLUDE_TEST(GetFS_Size_Ok);
	INCLUDE_TEST(GetFS_Free_Ok);
	INCLUDE_TEST(MakeDir_Ok);
	INCLUDE_TEST(WriteFile_Ok);
	INCLUDE_TEST(WriteString_File_Ok);
	INCLUDE_TEST(WriteString_FileAppend_Ok);
	INCLUDE_TEST(WriteFile_OverExistingFile_Ok);
	INCLUDE_TEST(ReadFile_Ok);
	INCLUDE_TEST(RemoveFile_Ok);
//	INCLUDE_TEST(WriteMaxNumberOfFiles);
//	INCLUDE_TEST(WriteLargeContinuousFile);
	INCLUDE_TEST(RemoveDir_NoForce_Ok);
	INCLUDE_TEST(RemoveDir_NoForce_Fail);
	INCLUDE_TEST(RemoveDir_Force_Ok);
//	INCLUDE_TEST(CopyFileToFatFs_FileOnLittleFs_NewFileOnFatFs);
//	INCLUDE_TEST(CopyFileToFlash_FileOnFatFs_NewFileOnLittleFs);
}END_GROUP

