/*!************************************************************************************************
 * @file flashdriver_interface.h
 *
 * @brief Defines a common interface and abstraction for flash IC drivers
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

#ifndef FLASHDRIVERINTERFACE_H
#define FLASHDRIVERINTERFACE_H

/* Includes
   -----------------------------------------------------------------------------------------------*/
#include <stdbool.h>
#include <stdint.h>
/* Defines
   -----------------------------------------------------------------------------------------------*/

/* Typedefs
   -----------------------------------------------------------------------------------------------*/

typedef enum
{
    FLASHDRV_STATE_ERROR,
    FLASHDRV_STATE_READY,
    FLASHDRV_STATE_WRITE_READY,
    FLASHDRV_STATE_LOCKED,
    FLASHDRV_STATE_BUSY,
    FLASHDRV_STATE_SUSPENDED,
    FLASHDRV_STATE_SLEEP,
	FLASHDRV_STATE_NOINIT
}FlashDrv_FlashState;

typedef enum
{
    FLASHDRV_OK,
    FLASHDRV_ERROR,
    FLASHDRV_NOT_SUPPORTED
}FlashDrv_Status;

typedef enum
{
    FLASHDRV_MATCH_MODE_AND,
    FLASHDRV_MATCH_MODE_OR
}FlashDrv_AutoPollMatchMode;

typedef enum
{
    FLASHDRV_CMD_MODE,
    FLASHDRV_MM_READ,
    FLASHDRV_MM_WRITE
}FlashDrv_MemoryMappedConfig;

typedef struct
{
    uint32_t address;
    uint32_t nrOfDataBytes;
    uint32_t instruction;
    uint32_t alternateBytes;
    uint16_t autopollMatch;
    uint16_t autopollMask;
    uint16_t autoPollIntervalCC;
    uint16_t autoPollTimeoutCC;
    uint8_t  addressByteSize;
    uint8_t  nrOfInstructionLines;
    uint8_t  nrOfAddressLines;
    uint8_t  nrOfDataLines;
    uint8_t  nrOfDummyCycles;
    uint8_t  autoPollMatchMode;    // FlashDrv_AutoPollMatchMode
    uint8_t  memoryMapModeConfig;  // FlashDrv_MemoryMappedConfig
    bool     hasAddress;
    bool     useAlternateBytes;
    bool     useAddressDTR;        // double transfer rate
    bool     useDataDTR;
    bool     useAlternateBytesDTR;
    bool     useAutoPoll;
    bool     useAutoPollStop;
}FlashDrv_CommandDef;

typedef struct
{
    uint32_t maxSPIFrequencyInkHz; // some IC have freq. dependent dummy cycles
    uint8_t  nrIOLines;
}FlashDrv_ConfigDef;

/* Private variables
   -----------------------------------------------------------------------------------------------*/

/* Private interface
   -----------------------------------------------------------------------------------------------*/

/* Public variables
   -----------------------------------------------------------------------------------------------*/

/* Public interface
   -----------------------------------------------------------------------------------------------*/

/*Function prototype for the driver interface. Define and configure your driver*/
FlashDrv_Status FlashDrv_SendCommand(FlashDrv_CommandDef* command);
FlashDrv_Status FlashDrv_ReceiveData(uint8_t* buffer, uint32_t timeout);
FlashDrv_Status FlashDrv_WriteData(uint8_t* buffer, uint32_t timeout);

#endif
