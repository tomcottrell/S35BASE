/*!************************************************************************************************
 * @file mx25l3233f.h
 *
 * @brief Implementation of a driver for the Macronix mx25l3233f QSPI NOR
 * flash
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
 *************************************************************************************************/


#ifndef MX25L3233F_H
#define MX25L3233F_H

/* Includes
   -----------------------------------------------------------------------------------------------*/
#include "flashdriver_interface.h"

/* Defines
   -----------------------------------------------------------------------------------------------*/

/* MX25L3233F capacity is 32Mbit =   4Mb (512kb x 8)                          */
#define MX25L32_PAGE_SIZE          256        /* Page size 1 << 8 = 256 */
#define MX25L32_SECTOR_SHIFT       12         /* Sector size 1 << 12 = 4Kb */
#define MX25L32_NSECTORS           1024
#define MX25L32_ERASED_VALUE       0xff

/* MX25L3233F Instructions
   -----------------------------------------------------------------------------------------------*/
/*      Command              Value  Description             Addr Data Dummy   */
#define MX25L32_READ               0x03       // Read data bytes          3   0   >=1
#define MX25L32_FAST_READ          0x0b       // Higher speed read        3   1   >=1
#define MX25L32_2READ              0xbb       // 2 x I/O read command
#define MX25L32_DREAD              0x3b       // 1I / 2O read command     3   1   >=1
#define MX25L32_4READ              0xeb       // 4 x I/O read command
#define MX25L32_QREAD              0x6b       // 1I / 4O read command     3   1   >=1
#define MX25L32_WREN               0x06       // Write Enable             0   0   0
#define MX25L32_WRDI               0x04       // Write Disable            0   0   0
#define MX25L32_RDSR               0x05       // Read status register     0   0   >=1
#define MX25L32_RDCR               0x15       // Read config register     0   0   >=1
#define MX25L32_WRSR               0x01       // Write stat/conf register 0   0   2
#define MX25L32_SE                 0x20       // 4Kb Sector erase         3   0   0
#define MX25L32_BE32               0x52       // 32Kbit block Erase       3   0   0
#define MX25L32_BE64               0xd8       // 64Kbit block Erase       3   0   0
#define MX25L32_CE                 0xc7       // Chip erase               0   0   0
#define MX25L32_CE_ALT             0x60       // Chip erase (alternate)   0   0   0
#define MX25L32_PP                 0x02       // Page program             3 1-256 0
#define MX25L32_4PP                0x38       // Quad page program        3   0
                                              //   1-256
#define MX25L32_DP                 0xb9       // Deep power down          0   0   0
#define MX25L32_RDP                0xab       // Release deep power down  0   0   0
#define MX25L32_PGM_SUSPEND        0x75       // Suspends program         0   0   0
#define MX25L32_ERS_SUSPEND        0xb0       // Suspends erase           0   0   0
#define MX25L32_PGM_RESUME         0x7A       // Resume program           0   0   0
#define MX25L32_ERS_RESUME         0x30       // Resume erase             0   0   0
#define MX25L32_RDID               0x9f       // Read identification      0   0   3
#define MX25L32_RES                0xab       // Read electronic ID       0   3   1
#define MX25L32_REMS               0x90       // Read manufacture and ID  1   2   >=2
#define MX25L32_ENSO               0xb1       // Enter secured OTP        0   0   0
#define MX25L32_EXSO               0xc1       // Exit secured OTP         0   0   0
#define MX25L32_RDSCUR             0x2b       // Read security register   0   0   0
#define MX25L32_WRSCUR             0x2f       // Write security register  0   0   0
#define MX25L32_RSTEN              0x66       // Reset Enable             0   0   0
#define MX25L32_RST                0x99       // Reset Memory             0   0   0
#define MX25L32_RDSFDP             0x5a       // read out until CS# high
#define MX25L32_SBL                0xc0       // Set Burst Length
#define MX25L32_SBL_ALT            0x77       // Set Burst Length
#define MX25L32_NOP                0x00       // No Operation             0   0   0

/* MX25L Registers
   -----------------------------------------------------------------------------------------------*/
/* Read ID (RDID) register values */

#define MX25L32_MANUFACTURER       0xc2       // Macronix manufacturer ID
#define MX25L32_DEVID              0x15       // MX25L3233F device ID

/* JEDEC Read ID register values */

#define MX25L32_JEDEC_MANUFACTURER 0xc2       // Macronix manufacturer ID
#define MX25L32_JEDEC_MEMORY_TYPE  0x20       // MX25Lx  memory type
#define MX25L32_JEDEC__CAPACITY    0x16       // MX25L3233F memory capacity

/* Status register bit definitions */

#define MX25L32_SR_WIP             ( 1 << 0 ) // Bit 0: Write in progress
#define MX25L32_SR_WEL             ( 1 << 1 ) // Bit 1: Write enable latch
#define MX25L32_SR_BP_SHIFT        ( 2 )      // Bits 2-5: Block protect
                                              // bits
#define MX25L32_SR_BP_MASK         ( 15 << MX25L32_SR_BP_SHIFT )
#define MX25L32_SR_QE              ( 1 << 6 ) // Bit 6: Quad enable
#define MX25L32_SR_SRWD            ( 1 << 7 ) // Bit 7: Status register wp

/* Configuration register bit definitions */

#define MX25L32_CR_ODS             ( 1 << 0 ) // Bit 0: Output driver
                                              // strength
#define MX25L32_CR_TB              ( 1 << 3 ) // Bit 3: Top/bottom selected
#define MX25L32_CR_DC              ( 1 << 6 ) // Bit 6: Dummy cycle

/* Security register values        */

#define MX25L32_SCUR_OPT_IND       ( 1 << 0 ) // Bit 0: secured otp indicator, factory lock
#define MX25L32_SCUR_LDSO          ( 1 << 1 ) // Bit 1: lock-down, 4k bit secured
#define MX25L32_SCUR_PSB           ( 1 << 2 ) // Bit 2: programm suspend status
#define MX25L32_SCUR_ESB           ( 1 << 3 ) // Bit 3: erase suspend status
#define MX25L32_SCUR_PFL           ( 1 << 5 ) // Bit 5: program operation failed
#define MX25L32_SCUR_EFL           ( 1 << 6 ) // Bit 6: erase operation failed

/* Typedefs
   -----------------------------------------------------------------------------------------------*/

/* Private variables
   -----------------------------------------------------------------------------------------------*/

/* Private interface
   -----------------------------------------------------------------------------------------------*/

/* Public interface
   -----------------------------------------------------------------------------------------------*/

FlashDrv_Status MX25L32_Init(FlashDrv_ConfigDef config);

FlashDrv_Status MX25L32_GetState(uint8_t* state);
FlashDrv_Status MX25L32_ReadStatusRegister(uint8_t* statusRegister);
FlashDrv_Status MX25L32_ReadConfigRegister(uint8_t* configRegister);
FlashDrv_Status MX25L32_WriteStatusConfigRegister(uint8_t statusRegister, uint8_t configRegister);

FlashDrv_Status MX25L32_Read(uint32_t address, uint8_t* buffer, uint32_t dataSize);
FlashDrv_Status MX25L32_ReadFast(uint32_t address, uint8_t* buffer, uint32_t dataSize);
FlashDrv_Status MX25L32_ReadDual(uint32_t address, uint8_t* buffer, uint32_t dataSize);
FlashDrv_Status MX25L32_ReadQuad(uint32_t address, uint8_t* buffer, uint32_t dataSize);

// 2xIO, 4xIO read mode if supported
FlashDrv_Status MX25L32_ReadX(uint32_t address, uint8_t* buffer, uint32_t dataSize);                                         //

FlashDrv_Status MX25L32_PageProgram(uint32_t address, uint8_t* buffer, uint32_t dataSize);

// up to 4xIO parallel page program, if available
FlashDrv_Status MX25L32_PageProgramX(uint32_t address, uint8_t* buffer, uint32_t dataSize);

FlashDrv_Status MX25L32_EraseChip(void);
FlashDrv_Status MX25L32_EraseSector(uint32_t address);
FlashDrv_Status MX25L32_EraseBlock32kb(uint32_t address);
FlashDrv_Status MX25L32_EraseBlock64kb(uint32_t address);

FlashDrv_Status MX25L32_DeepPowerDown(void);
FlashDrv_Status MX25L32_ReleaseDeepPowerDown(void);
FlashDrv_Status MX25L32_Reset(void);
FlashDrv_Status MX25L32_SetNOP(void);

FlashDrv_Status MX25L32_ReadSecurityRegister(uint8_t* secRegister);
FlashDrv_Status MX25L32_WriteSecurityRegister(uint8_t secRegister);

FlashDrv_Status MX25L32_ReadREMS(uint8_t* manufacturer, uint8_t* deviceId);
FlashDrv_Status MX25L32_ReadSFDPValue(uint32_t address, uint8_t* buffer, uint8_t dataSize);
#endif
