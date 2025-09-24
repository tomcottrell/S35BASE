/*!*************************************************************************************************
 * @file mx25l3233f_test.c
 *
 * @brief Contains test implementation for the flash IC
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
#include "MX25L3233F/mx25l3233f.h"
#include "mtest.h"
#include <stdbool.h>

/* Defines
   -----------------------------------------------------------------------------------------------*/
#define MAX_SPI_CLK 120000 // configure here the SPI base clock the OSPI is running on

/* Typedefs
   -----------------------------------------------------------------------------------------------*/

/* Private variables
   -----------------------------------------------------------------------------------------------*/

/* Private interface
   -----------------------------------------------------------------------------------------------*/


static bool _compareBuffer(uint8_t* buf1, uint8_t* buf2, uint32_t size){

	if ((buf1 == NULL) || (buf2 == NULL)){
		return false;
	}

	for(uint32_t i = 0; i < size; i++){
		if ( buf1[i] != buf2[i]){
			return false;
		}
	}
	return true;
}

static void setup(void)
{
    /* executed before *every* non-skipped test */
    FlashDrv_ConfigDef  config = {.maxSPIFrequencyInkHz = MAX_SPI_CLK, .nrIOLines = 4};
    MX25L32_Init(config);
}

/*!*************************************************************************************************
 * @brief teardown
 *
 **************************************************************************************************/
static void teardown(void)
{
    /* executed after *every* non-skipped and non-failing test */
    MX25L32_Reset();
}

/*!*************************************************************************************************
 * @brief groupTeardown
 *
 **************************************************************************************************/
static void groupTeardown()
{
    FlashDrv_ConfigDef  config = {.maxSPIFrequencyInkHz = MAX_SPI_CLK, .nrIOLines = 4};
    MX25L32_Init(config);
}

/* Public interface
   -----------------------------------------------------------------------------------------------*/

TEST(FlashInit_StatusConfigRegister_QE_DC_bits){
    uint8_t  statusRegister = 0x0;
    MX25L32_ReadStatusRegister(&statusRegister);

    uint8_t  configRegister = 0xFF;
    MX25L32_ReadConfigRegister(&configRegister);

    IS_TRUE(( statusRegister & MX25L32_SR_QE ));
    if ( MAX_SPI_CLK > 104000 )
    {
        IS_TRUE(( configRegister & MX25L32_CR_DC ));
    }
    else
    {
        IS_TRUE(!( configRegister & MX25L32_CR_DC ));
    }
}

END_TEST

TEST(FlashInit_WriteStatusConfigRegister_NDC_NQE)
{
    MX25L32_Reset();
    FlashDrv_Status  status = MX25L32_WriteStatusConfigRegister(0x0, 0x0);

    IS_TRUE( status == FLASHDRV_OK);

}
END_TEST

TEST(PageProgram_RW_AddressWithinPage)
{
    char             str[161] =
        "Test PageProgram_RW_AddressWithinPage: This string should be written within a page "
        "without wrap and if all goes well this string should be returned by read cmd.";
    char             readBuffer[161];

    uint32_t         address = 0x3E8F0F;
    FlashDrv_Status  status = MX25L32_PageProgram(address,(uint8_t*) str, 161);
    IS_TRUE(status == FLASHDRV_OK);

    status = MX25L32_ReadFast(address, (uint8_t*) readBuffer, 161);
    IS_TRUE(status == FLASHDRV_OK);

    STR_EQ(str, readBuffer);
}
END_TEST

TEST(PageProgram_RW_AddressWrapAroundPage)
{
    char             str[162] =
        "Test PageProgram_RW_AddressWrapAroundPage: This string should be written within a page "
        "with wrap and if all goes well this string should be returned by read cmd.";
    char             readBuffer[162];

    uint32_t         address = 0x008F8F;
    FlashDrv_Status  status = MX25L32_PageProgramX(address,(uint8_t*) str, 162);
    IS_TRUE(status == FLASHDRV_OK);

    status = MX25L32_ReadFast(address, (uint8_t*) readBuffer, 162);
    IS_TRUE(status == FLASHDRV_OK);

    STR_EQ(str, readBuffer);
}
END_TEST

TEST(PageProgram_RW_AddressWithinPage_4IO)
{
    char             str[216] =
        "Test PageProgram_RW_AddressWithinPage_4IO: This test string should be written within a page "
        "and wrap around the next page section. If all is working well, a following read command "
        "should return this exact string. :)";
    char             readBuffer[216];

    uint32_t         address = 0x001F00;
    FlashDrv_Status  status = MX25L32_PageProgramX(address,(uint8_t*) str, 216);
    IS_TRUE(status == FLASHDRV_OK);

    status = MX25L32_ReadFast(address, (uint8_t*) readBuffer, 216);
    IS_TRUE(status == FLASHDRV_OK);

    ARR_EQ((uint8_t*) str, (uint8_t*)readBuffer);
}
END_TEST

TEST(PageProgram_RW_SlowRead_WithinPage)
{
    char             str[167] =
        "Test PageProgram_SlowRead_AddressWithinPage: This string should be written within a page "
        "without wrap and if all goes well this string should be returned by read cmd.";
    char             readBuffer[167];
    uint32_t         address = 0x3FA000;
    FlashDrv_Status  status = MX25L32_PageProgramX(address,(uint8_t*) str, 167);
    IS_TRUE(status == FLASHDRV_OK);

    status = MX25L32_Read(address, (uint8_t*) readBuffer, 167);
    IS_TRUE(status == FLASHDRV_OK);

    ARR_EQ((uint8_t*) str, (uint8_t*)readBuffer);
}
END_TEST

TEST(EraseSector_AfterWrite_Empty)
{
    char             str[73] =
        "This test string should be written and when be deleted after chip erase!";

    uint32_t         address = 0x001F0F;
    FlashDrv_Status  status = MX25L32_PageProgramX(address,(uint8_t*) str, 173);
    IS_TRUE(status == FLASHDRV_OK);

    status = MX25L32_EraseSector(address);         // should erase the sector: 0x001000h
    IS_TRUE(status == FLASHDRV_OK);

    char             firstLetter = 0;
    status = MX25L32_ReadFast(address, (uint8_t*) &firstLetter, 1);
    IS_TRUE(firstLetter == MX25L32_ERASED_VALUE);
}
END_TEST

TEST(EraseAllSectors_WriteAfter_Ok){
	const uint32_t nrSectors = 1024;
	uint32_t address = 0x0;
	uint32_t count = 0;

	for ( uint16_t i = 0; i < nrSectors; i++){
		address = i* 0x1000;
		MX25L32_EraseSector(address);
		MX25L32_PageProgramX(address, (uint8_t*) &address, sizeof(address));
	}

	// read and check for each sector if address value is written properly
	for ( uint16_t i = 0; i < nrSectors; i++){

		address = i* 0x1000;
		MX25L32_ReadX(address, (uint8_t*) &address, sizeof(address));

		if(address == (i* 0x1000)){
			count++;
		}else {
			break;
		}
	}

	IS_EQUAL(count,nrSectors);

}END_TEST

TEST(EraseBlock32_AfterWrite_Empty)
{
    char             str[73] =
        "This test string should be written and when be deleted after chip erase!";

    uint32_t         address = 0x001F0F;
    FlashDrv_Status  status = MX25L32_PageProgramX(address,(uint8_t*) str, 173);
    IS_TRUE(status == FLASHDRV_OK);

    // write to next 32kB block '1'
    uint32_t         next32kbBlockAddress = 0x008000;
    status = MX25L32_PageProgramX(next32kbBlockAddress,(uint8_t*) str, 173);
    IS_TRUE(status == FLASHDRV_OK);

    status = MX25L32_EraseBlock32kb(next32kbBlockAddress);         // should erase the 32kB block '1'
    IS_TRUE(status == FLASHDRV_OK);

    char             firstLetter = 0;
    status = MX25L32_ReadFast(address,(uint8_t*)  &firstLetter, 1);
    IS_TRUE(status == FLASHDRV_OK);
    IS_TRUE(firstLetter == str[0]);

    // check block '1'
    status = MX25L32_ReadFast(next32kbBlockAddress, (uint8_t*) &firstLetter, 1);
    IS_TRUE(status == FLASHDRV_OK);
    IS_TRUE(firstLetter == MX25L32_ERASED_VALUE);

    // cleanup
    MX25L32_EraseBlock32kb(address);
}
END_TEST

TEST(EraseBlock64_AfterWrite_Empty)
{
    char             str[73] =
        "This test string should be written and when be deleted after chip erase!";

    uint32_t         address = 0x001F0F;
    FlashDrv_Status  status = MX25L32_PageProgramX(address,(uint8_t*) str, 173);
    IS_TRUE(status == FLASHDRV_OK);

    // write to next 64kB block '1'
    uint32_t         next64kbBlockAddress = 0x010000;
    status = MX25L32_PageProgramX(next64kbBlockAddress,(uint8_t*) str, 173);
    IS_TRUE(status == FLASHDRV_OK);

    status = MX25L32_EraseBlock64kb(next64kbBlockAddress);         // should erase the 64kB block '1'
    IS_TRUE(status == FLASHDRV_OK);

    char             firstLetter = 0;
    status = MX25L32_ReadFast(address, (uint8_t*)  &firstLetter, 1);
    IS_TRUE(status == FLASHDRV_OK);
    IS_TRUE(firstLetter == str[0]);

    // check 64kB block '1'
    status = MX25L32_ReadFast(next64kbBlockAddress, (uint8_t*)  &firstLetter, 1);
    IS_TRUE(status == FLASHDRV_OK);
    IS_TRUE(firstLetter == MX25L32_ERASED_VALUE);

    // cleanup
    MX25L32_EraseBlock64kb(address);
}
END_TEST

TEST(ReadWriteSecurityRegister)
{
    uint8_t  secRegister = MX25L32_SCUR_LDSO;
    MX25L32_WriteSecurityRegister(secRegister);

    uint8_t  readback;
    MX25L32_ReadSecurityRegister(&readback);

    IS_TRUE(readback == secRegister);
}
END_TEST

TEST(ReadFast)
{
    char             writeBuffer[16] = {"Test fast read."};
    char             readbackBuffer[16];
    uint32_t         address = 0x0;         //from start
    FlashDrv_Status  status = MX25L32_PageProgramX(address,(uint8_t*)  writeBuffer, 16);
    IS_TRUE(status == FLASHDRV_OK);

    status = MX25L32_ReadFast(address, (uint8_t*) readbackBuffer, 16);
    IS_TRUE(status == FLASHDRV_OK);

    ARR_EQ((uint8_t*) writeBuffer, (uint8_t*) readbackBuffer);
}
END_TEST

TEST(ReadDual)
{
    char             writeBuffer[16] = {"Test dual read."};
    char             readbackBuffer[16];
    uint32_t         address = 0x1000;         //from start
    FlashDrv_Status  status = MX25L32_PageProgram(address, (uint8_t*)  writeBuffer, 16);

    IS_TRUE(status == FLASHDRV_OK);
    status = MX25L32_ReadDual(address,(uint8_t*)  readbackBuffer, 16);
    IS_TRUE(status == FLASHDRV_OK);

    ARR_EQ((uint8_t*) writeBuffer, (uint8_t*) readbackBuffer);
}
END_TEST

TEST(ReadQuad)
{
    char             writeBuffer[16] = {"Test quad read."};
    char             readbackBuffer[16];
    uint32_t         address = 0x2000;         // from start
    FlashDrv_Status  status = MX25L32_PageProgram(address,(uint8_t*)  writeBuffer, 16);

    IS_TRUE(status == FLASHDRV_OK);
    status = MX25L32_ReadQuad(address, (uint8_t*) readbackBuffer, 16);
    IS_TRUE(status == FLASHDRV_OK);

    ARR_EQ((uint8_t*) writeBuffer, (uint8_t*) readbackBuffer);
}
END_TEST

TEST(ReadX_Dual)
{
    MX25L32_Reset();
    FlashDrv_ConfigDef  config = {.maxSPIFrequencyInkHz = 120000, .nrIOLines = 2};
    MX25L32_Init(config);

    char                writeBuffer[16] = {"Test 2xIO read."};
    char                readbackBuffer[16];
    uint32_t            address = 0x3000;         //from start
    FlashDrv_Status     status = MX25L32_PageProgram(address, (uint8_t*)  writeBuffer,
                                                     16);
    IS_TRUE(status == FLASHDRV_OK);

    status = MX25L32_ReadX(address, (uint8_t*) readbackBuffer, 16);
    IS_TRUE(status == FLASHDRV_OK);
    ARR_EQ((uint8_t*) writeBuffer, (uint8_t*) readbackBuffer);
}
END_TEST

TEST(ReadX_Quad)
{
    char             writeBuffer[16] = {"Test 4xIO read."};
    char             readbackBuffer[16];
    uint32_t         address = 0x4000;          //from start
    FlashDrv_Status  status = MX25L32_PageProgramX(address, (uint8_t*) writeBuffer, 16);
    IS_TRUE(status == FLASHDRV_OK);

    status = MX25L32_ReadX(address,(uint8_t*)  readbackBuffer, 16);
    IS_TRUE(status == FLASHDRV_OK);
    ARR_EQ((uint8_t*) writeBuffer, (uint8_t*) readbackBuffer);
}
END_TEST

TEST(DeepPowerDown)
{
    FlashDrv_Status  status = MX25L32_DeepPowerDown();
    IS_TRUE(status == FLASHDRV_OK);
}
END_TEST

TEST(ReleaseDeepPowerDown)
{
    FlashDrv_Status  status = MX25L32_ReleaseDeepPowerDown();
    IS_TRUE(status == FLASHDRV_OK);
}
END_TEST

TEST(WriteEraseChip_Readback_Correct)
{
    uint8_t         pageBuffer[256];
    uint8_t 		readBuffer[256];
    uint32_t        address = 0x0;
    const uint32_t  numberOfPages = 16384;
    uint32_t        correctPrograms = 0;

    MX25L32_EraseChip();

    //fill buffer
    for ( int i = 0; i < 256; i++ )
    {
        pageBuffer[i] = 1 << ( i % 8 );
    }

    for ( int i = 0; i < numberOfPages; i++ )
    {
        MX25L32_PageProgramX(address, pageBuffer, 256);
        MX25L32_ReadX(address, readBuffer, 256);

        if(_compareBuffer(pageBuffer, readBuffer, 256)){
        	correctPrograms++;
        }
    }

    FlashDrv_Status  status = MX25L32_EraseChip();
    MX25L32_ReadX(address, readBuffer, 16);


    IS_EQUAL(correctPrograms, numberOfPages);
    IS_TRUE(status == FLASHDRV_OK);
    IS_EQUAL(readBuffer[8], 0xFF);
}
END_TEST

TEST(ReadManufacturerDeviceId_Ids_Ok){
  uint8_t manufacturer = 0x0;
  uint8_t deviceId = 0x0;

  FlashDrv_Status  status = MX25L32_ReadREMS(&manufacturer, &deviceId);
  IS_TRUE(status == FLASHDRV_OK);
  IS_EQUAL(manufacturer, MX25L32_MANUFACTURER);
  IS_EQUAL(deviceId, MX25L32_DEVID);

}END_TEST

TEST(ReadJdecDeviceData_MemDensity_Ok){
  uint32_t memCapacity = 0x0;
  uint32_t cap32Mbit = 0x1FFFFFF;
  uint32_t address = 0x34; //mem capacity is byte 34h-37h

  FlashDrv_Status status = MX25L32_ReadSFDPValue(address, (uint8_t*) &memCapacity, sizeof(memCapacity));
  IS_TRUE(status == FLASHDRV_OK);

  IS_EQUAL(memCapacity, cap32Mbit);

}END_TEST

TEST_GROUP(MX25L3233F_Driver)
{
    printf("OSPI running on %d kHz\n\n", MAX_SPI_CLK);

    INCLUDE_TEST(FlashInit_StatusConfigRegister_QE_DC_bits);
    INCLUDE_TEST(FlashInit_WriteStatusConfigRegister_NDC_NQE);
    INCLUDE_TEST(ReadWriteSecurityRegister);
    INCLUDE_TEST(PageProgram_RW_AddressWithinPage);
    INCLUDE_TEST(PageProgram_RW_AddressWrapAroundPage);
    INCLUDE_TEST(PageProgram_RW_AddressWithinPage_4IO);
    if ( 60000 >= MAX_SPI_CLK )
    {
        INCLUDE_TEST(PageProgram_RW_SlowRead_WithinPage);
    }
    INCLUDE_TEST(EraseSector_AfterWrite_Empty);
//    INCLUDE_TEST(EraseAllSectors_WriteAfter_Ok);
//    INCLUDE_TEST(EraseBlock32_AfterWrite_Empty);
//    INCLUDE_TEST(EraseBlock64_AfterWrite_Empty);
//    INCLUDE_TEST(ReadFast);
//    INCLUDE_TEST(ReadDual);
//    INCLUDE_TEST(ReadQuad);
//    INCLUDE_TEST(ReadX_Dual);
    INCLUDE_TEST(ReadX_Quad);
//    INCLUDE_TEST(DeepPowerDown);
//    INCLUDE_TEST(ReleaseDeepPowerDown);
    INCLUDE_TEST(ReadManufacturerDeviceId_Ids_Ok);
    INCLUDE_TEST(ReadJdecDeviceData_MemDensity_Ok);
    INCLUDE_TEST(WriteEraseChip_Readback_Correct);
}
END_GROUP
