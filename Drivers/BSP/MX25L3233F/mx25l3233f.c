/*!*************************************************************************************************
 * @file mx25l3233f.c
 *
 * @brief Implementation of a driver for the Macronix mx25l3233f QSPI NOR flash
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
#include "mx25l3233f.h"

/* Defines
   -----------------------------------------------------------------------------------------------*/
#define _DEFAULT_TIMEOUT_IN_MS     100
#define _MAX_DEFAULT_TIMEOUT_IN_MS 200

/* Typedefs
   -----------------------------------------------------------------------------------------------*/

typedef struct
{
    FlashDrv_ConfigDef config;
    uint8_t            dummyCycles;
    uint8_t            flashState;
    uint8_t            statusRegister;
    bool               isPoweredDown;
    bool               isInitialized;
}_Control;

/* Private variables
   -----------------------------------------------------------------------------------------------*/
static _Control  control;

/* Private interface
   -----------------------------------------------------------------------------------------------*/
static FlashDrv_Status _AutoPollMemReady(void);
static void _WriteEnable(void);
static void _WriteDisable(void);
static uint8_t _GetnrOfDummyCycles(uint8_t);
static bool _isValidAddress(uint32_t address);

/*!*************************************************************************************************
 * @brief _WriteEnable
 *
 **************************************************************************************************/
void _WriteEnable(void)
{
    FlashDrv_CommandDef  command = {
        .address = 0x0,
        .nrOfDataBytes = 0,
        .instruction = MX25L32_WREN,
        .autopollMatch = 0x0,
        .autopollMask = 0x0,
        .autoPollIntervalCC = 0x0,
        .autoPollTimeoutCC = 0x0,
        .addressByteSize = 3,
        .nrOfInstructionLines = 1,
        .nrOfAddressLines = 0,
        .nrOfDataLines = 0,
        .nrOfDummyCycles = 0,
        .autoPollMatchMode = FLASHDRV_MATCH_MODE_AND,
        .memoryMapModeConfig = FLASHDRV_CMD_MODE,
        .hasAddress = false,
        .useAlternateBytes = false,
        .useAddressDTR = false,
        .useDataDTR = false,
        .useAutoPoll = false,
        .useAutoPollStop = false
    };

    FlashDrv_SendCommand(&command);
}

/*!*************************************************************************************************
 * @brief _WriteDisable
 *
 **************************************************************************************************/
void _WriteDisable(void)
{
    FlashDrv_CommandDef  command = {
        .address = 0x0,
        .nrOfDataBytes = 0,
        .instruction = MX25L32_WRDI,
        .autopollMatch = 0x0,
        .autopollMask = 0x0,
        .autoPollIntervalCC = 0x0,
        .autoPollTimeoutCC = 0x0,
        .addressByteSize = 3,
        .nrOfInstructionLines = 1,
        .nrOfAddressLines = 0,
        .nrOfDataLines = 0,
        .nrOfDummyCycles = 0,
        .autoPollMatchMode = FLASHDRV_MATCH_MODE_AND,
        .memoryMapModeConfig = FLASHDRV_CMD_MODE,
        .hasAddress = false,
        .useAlternateBytes = false,
        .useAddressDTR = false,
        .useDataDTR = false,
        .useAutoPoll = false,
        .useAutoPollStop = false
    };

    FlashDrv_SendCommand(&command);
}

/*!*************************************************************************************************
 * @brief _AutoPollMemReady
 *
 **************************************************************************************************/
FlashDrv_Status _AutoPollMemReady()
{
    // Polling requires reading the status register
    FlashDrv_CommandDef  command = {
        .address = 0x0,
        .nrOfDataBytes = 1,
        .instruction = MX25L32_RDSR,
        .autopollMatch = 0x0,
        .autopollMask = MX25L32_SR_WIP,
        .autoPollIntervalCC = 500,
        .autoPollTimeoutCC = 30000,
        .addressByteSize = 3,
        .nrOfInstructionLines = 1,
        .nrOfAddressLines = 0,
        .nrOfDataLines = 1,
        .nrOfDummyCycles = 0,
        .autoPollMatchMode = FLASHDRV_MATCH_MODE_OR,
        .memoryMapModeConfig = FLASHDRV_CMD_MODE,
        .hasAddress = false,
        .useAlternateBytes = false,
        .useAddressDTR = false,
        .useDataDTR = false,
        .useAutoPoll = true,
        .useAutoPollStop = true
    };

    return FlashDrv_SendCommand(&command);
}

/*!*************************************************************************************************
 * @brief _GetnrOfDummyCycles
 * @return nr of dummy cycles
 **************************************************************************************************/
uint8_t _GetnrOfDummyCycles(uint8_t nrOfDatalines)
{
    uint8_t  nrDummyCycles = 0;
    if ( nrOfDatalines == 2 )
    {
        nrDummyCycles = ( control.config.maxSPIFrequencyInkHz >= 104000 ) ? 8 : 4;
    }
    else if ( nrOfDatalines == 4 )
    {
        nrDummyCycles = ( control.config.maxSPIFrequencyInkHz >= 104000 ) ? 10 : 6;
    }

    return nrDummyCycles;
}

/*!*************************************************************************************************
 * @brief _isValidAddress checks whether the address stays < the maximum possible address
 * @param address
 * @return
 **************************************************************************************************/
bool _isValidAddress(uint32_t address)
{
    return ( address > 0x3FFFFF ) ? false : true;
}

/* Public interface
   -----------------------------------------------------------------------------------------------*/


/*!*************************************************************************************************
 * @brief MX25L_Init
 **************************************************************************************************/
FlashDrv_Status MX25L32_Init(FlashDrv_ConfigDef config)
{
    control.config = config;
    control.dummyCycles = 0;
    control.flashState = FLASHDRV_STATE_ERROR;
    control.statusRegister = 0x0;
    control.isPoweredDown = false;
    control.isInitialized = false;

    FlashDrv_FlashState  state = 0x0;
    FlashDrv_Status      status = MX25L32_Reset();
    if ( status != FLASHDRV_OK )
    {
        return status;
    }

    MX25L32_GetState(&state);
    if ( state == FLASHDRV_STATE_BUSY )
    {
        // write-enable bit is alway on
        if ( _AutoPollMemReady() != FLASHDRV_OK )
        {
            return FLASHDRV_ERROR;
        }
    }

    // configure dummy cycles
    uint8_t              configRegister = 0x0;
    if ( control.config.maxSPIFrequencyInkHz >= 104000 )
    {
        configRegister |= MX25L32_CR_DC;
    }

    if ( ( control.config.nrIOLines == 2 ))
    {
        status = MX25L32_WriteStatusConfigRegister(control.statusRegister, configRegister);
    }
    else if ( control.config.nrIOLines == 4 )
    {
        // enable Quad
        uint8_t  statusReg = control.statusRegister | MX25L32_SR_QE;
        status = MX25L32_WriteStatusConfigRegister(statusReg, configRegister);
    }

    control.flashState = ( status ==
                           FLASHDRV_OK ) ? FLASHDRV_STATE_READY : FLASHDRV_STATE_ERROR;
    if ( control.flashState == FLASHDRV_STATE_READY )
    {
        control.isInitialized = true;
    }
    return status;
}



/*!*************************************************************************************************
 * @brief MX25L32_ReadStatusRegister
 * @param statusRegister address to write the received status byte to
 * @return
 **************************************************************************************************/
FlashDrv_Status MX25L32_ReadStatusRegister(uint8_t* statusRegister)
{
    FlashDrv_CommandDef  command = {
        .address = 0x0,
        .nrOfDataBytes = 1,
        .instruction = MX25L32_RDSR,
        .autopollMatch = 0x0,
        .autopollMask = 0x0,
        .autoPollIntervalCC = 0x0,
        .autoPollTimeoutCC = 0x0,
        .addressByteSize = 3,
        .nrOfInstructionLines = 1,
        .nrOfAddressLines = 0,
        .nrOfDataLines = 1,
        .nrOfDummyCycles = 0,
        .autoPollMatchMode = FLASHDRV_MATCH_MODE_AND,
        .memoryMapModeConfig = FLASHDRV_CMD_MODE,
        .hasAddress = false,
        .useAlternateBytes = false,
        .useAddressDTR = false,
        .useDataDTR = false,
        .useAutoPoll = false,
        .useAutoPollStop = false
    };

    FlashDrv_SendCommand(&command);
    return FlashDrv_ReceiveData(statusRegister,_DEFAULT_TIMEOUT_IN_MS);
}

/*!*************************************************************************************************
 * @brief MX25L32_ReadConfigRegister
 * @param statusRegister address to write the received status byte to
 * @return
 **************************************************************************************************/
FlashDrv_Status MX25L32_ReadConfigRegister(uint8_t* configRegister)
{
    FlashDrv_CommandDef  command = {
        .address = 0x0,
        .nrOfDataBytes = 1,
        .instruction = MX25L32_RDCR,
        .autopollMatch = 0x0,
        .autopollMask = 0x0,
        .autoPollIntervalCC = 0x0,
        .autoPollTimeoutCC = 0x0,
        .addressByteSize = 3,
        .nrOfInstructionLines = 1,
        .nrOfAddressLines = 0,
        .nrOfDataLines = 1,
        .nrOfDummyCycles = 0,
        .autoPollMatchMode = FLASHDRV_MATCH_MODE_AND,
        .memoryMapModeConfig = FLASHDRV_CMD_MODE,
        .hasAddress = false,
        .useAlternateBytes = false,
        .useAddressDTR = false,
        .useDataDTR = false,
        .useAutoPoll = false,
        .useAutoPollStop = false
    };

    FlashDrv_SendCommand(&command);
    return FlashDrv_ReceiveData(configRegister,_DEFAULT_TIMEOUT_IN_MS);
}

/*!*************************************************************************************************
 * @brief MX25L32_WriteStatusConfigRegister
 * @param statusRegister
 * @param configRegister
 * @return status of the operation
 **************************************************************************************************/
FlashDrv_Status MX25L32_WriteStatusConfigRegister(uint8_t statusRegister, uint8_t configRegister)
{

    FlashDrv_CommandDef  command = {
        .address = 0x0,
        .nrOfDataBytes = 2,
        .instruction = MX25L32_WRSR,
        .autopollMatch = 0x0,
        .autopollMask = 0x0,
        .autoPollIntervalCC = 0x0,
        .autoPollTimeoutCC = 0x0,
        .addressByteSize = 3,
        .nrOfInstructionLines = 1,
        .nrOfAddressLines = 0,
        .nrOfDataLines = 1,
        .nrOfDummyCycles = 0,
        .autoPollMatchMode = FLASHDRV_MATCH_MODE_AND,
        .memoryMapModeConfig = FLASHDRV_CMD_MODE,
        .hasAddress = false,
        .useAlternateBytes = false,
        .useAddressDTR = false,
        .useDataDTR = false,
        .useAutoPoll = false,
        .useAutoPollStop = false
    };

    uint8_t              dataBuffer[2] = { statusRegister, configRegister };

    _WriteEnable();
    FlashDrv_SendCommand(&command);
    FlashDrv_WriteData(dataBuffer, _DEFAULT_TIMEOUT_IN_MS);
    _WriteDisable();

    // check correct transmission
    uint8_t              statusRegSet = 0x0;
    uint8_t              configRegSet = 0x01;
    command.nrOfDataBytes = 1;
    _AutoPollMemReady();
    MX25L32_ReadConfigRegister(&configRegSet);
    MX25L32_ReadStatusRegister(&statusRegSet);

    if ( ( statusRegSet == statusRegister ) && ( configRegSet == configRegister ))
    {
        return FLASHDRV_OK;
    }
    return FLASHDRV_ERROR;
}

/*!*************************************************************************************************
 * @brief MX25L32_GetState
 * @param state, variable the state is written to
 * @return status of the operation
 **************************************************************************************************/
FlashDrv_Status MX25L32_GetState(uint8_t* state)
{
    if ( MX25L32_ReadStatusRegister(&control.statusRegister) != FLASHDRV_OK )
    {
        return FLASHDRV_ERROR;
    }

    if ( control.statusRegister & MX25L32_SR_WEL )
    {
        *state = FLASHDRV_STATE_WRITE_READY;
    }
    else if ( control.statusRegister & MX25L32_SR_WIP )
    {
        *state = FLASHDRV_STATE_BUSY;
    }
    else if ( control.statusRegister & MX25L32_SR_BP_MASK )
    {
        *state = FLASHDRV_STATE_LOCKED;
    }
    else if ( control.isPoweredDown )
    {
        *state = FLASHDRV_STATE_SLEEP;
    }
    else if ( !control.isInitialized )
    {
        *state = FLASHDRV_STATE_NOINIT;
    }
    else
    {
        *state = FLASHDRV_STATE_READY;
    }

    control.flashState = *state;

    return FLASHDRV_OK;
}



/*!*************************************************************************************************
 * @brief MX25L32_Read
 * @param address
 * @param buffer
 * @param dataSize
 * @return status of the operation
 **************************************************************************************************/
FlashDrv_Status MX25L32_Read(uint32_t address, uint8_t* buffer, uint32_t dataSize)
{
    FlashDrv_CommandDef  command = {
        .address = address,
        .nrOfDataBytes = dataSize,
        .instruction = MX25L32_READ,
        .autopollMatch = 0x0,
        .autopollMask = 0x0,
        .autoPollIntervalCC = 0x0,
        .autoPollTimeoutCC = 0x0,
        .addressByteSize = 3,
        .nrOfInstructionLines = 1,
        .nrOfAddressLines = 1,
        .nrOfDataLines = 1,
        .nrOfDummyCycles = 0,
        .autoPollMatchMode = FLASHDRV_MATCH_MODE_AND,
        .memoryMapModeConfig = FLASHDRV_CMD_MODE,
        .hasAddress = true,
        .useAlternateBytes = false,
        .useAddressDTR = false,
        .useDataDTR = false,
        .useAutoPoll = false,
        .useAutoPollStop = false
    };

    if ( !_isValidAddress(address))
    {
        return FLASHDRV_ERROR;
    }

    // wait for write operations to finish
    if ( _AutoPollMemReady() != FLASHDRV_OK )
    {
        return FLASHDRV_ERROR;
    }

    // send read command
    FlashDrv_SendCommand(&command);

    // call spi read
    return FlashDrv_ReceiveData(buffer, _MAX_DEFAULT_TIMEOUT_IN_MS);
}

/*!*************************************************************************************************
 * @brief MX25L32_ReadFast
 * @param address
 * @param buffer
 * @param dataSize
 * @return status of the operation
 **************************************************************************************************/
FlashDrv_Status MX25L32_ReadFast(uint32_t address, uint8_t* buffer,
                                 uint32_t dataSize)
{
    FlashDrv_CommandDef  command = {
        .address = address,
        .nrOfDataBytes = dataSize,
        .instruction = MX25L32_FAST_READ,
        .autopollMatch = 0x0,
        .autopollMask = 0x0,
        .autoPollIntervalCC = 0x0,
        .autoPollTimeoutCC = 0x0,
        .addressByteSize = 3,
        .nrOfInstructionLines = 1,
        .nrOfAddressLines = 1,
        .nrOfDataLines = 1,
        .nrOfDummyCycles = 8,
        .autoPollMatchMode = FLASHDRV_MATCH_MODE_AND,
        .memoryMapModeConfig = FLASHDRV_CMD_MODE,
        .hasAddress = true,
        .useAlternateBytes = false,
        .useAddressDTR = false,
        .useDataDTR = false,
        .useAutoPoll = false,
        .useAutoPollStop = false
    };

    if ( !_isValidAddress(address))
    {
        return FLASHDRV_ERROR;
    }

    // wait for write operations to finish
    if ( _AutoPollMemReady() != FLASHDRV_OK )
    {
        return FLASHDRV_ERROR;
    }

    // send read command
    FlashDrv_SendCommand(&command);

    // call spi read
    return FlashDrv_ReceiveData(buffer, _MAX_DEFAULT_TIMEOUT_IN_MS);
}

/*!*************************************************************************************************
 * @brief MX25L32_ReadDual
 * @param address
 * @param buffer
 * @param dataSize
 * @return status of the operation
 **************************************************************************************************/
FlashDrv_Status MX25L32_ReadDual(uint32_t address, uint8_t* buffer, uint32_t dataSize)
{
    FlashDrv_CommandDef  command = {
        .address = address,
        .nrOfDataBytes = dataSize,
        .instruction = MX25L32_DREAD,
        .autopollMatch = 0x0,
        .autopollMask = 0x0,
        .autoPollIntervalCC = 0x0,
        .autoPollTimeoutCC = 0x0,
        .addressByteSize = 3,
        .nrOfInstructionLines = 1,
        .nrOfAddressLines = 1,
        .nrOfDataLines = 2,
        .nrOfDummyCycles = 8,
        .autoPollMatchMode = FLASHDRV_MATCH_MODE_AND,
        .memoryMapModeConfig = FLASHDRV_CMD_MODE,
        .hasAddress = true,
        .useAlternateBytes = false,
        .useAddressDTR = false,
        .useDataDTR = false,
        .useAutoPoll = false,
        .useAutoPollStop = false
    };

    if ( !_isValidAddress(address))
    {
        return FLASHDRV_ERROR;
    }

    // wait for write operations to finish
    if ( _AutoPollMemReady() != FLASHDRV_OK )
    {
        return FLASHDRV_ERROR;
    }

    // send read command
    FlashDrv_SendCommand(&command);

    // call spi read
    return FlashDrv_ReceiveData(buffer, _MAX_DEFAULT_TIMEOUT_IN_MS);
}
/*!*************************************************************************************************
 * @brief MX25L32_ReadQuad
 * @param address
 * @param buffer
 * @param dataSize
 * @return status of the operation
 **************************************************************************************************/
FlashDrv_Status MX25L32_ReadQuad(uint32_t address, uint8_t* buffer, uint32_t dataSize)
{
    FlashDrv_CommandDef  command = {
        .address = address,
        .nrOfDataBytes = dataSize,
        .instruction = MX25L32_QREAD,
        .autopollMatch = 0x0,
        .autopollMask = 0x0,
        .autoPollIntervalCC = 0x0,
        .autoPollTimeoutCC = 0x0,
        .addressByteSize = 3,
        .nrOfInstructionLines = 1,
        .nrOfAddressLines = 1,
        .nrOfDataLines = 4,
        .nrOfDummyCycles = 8,
        .autoPollMatchMode = FLASHDRV_MATCH_MODE_AND,
        .memoryMapModeConfig = FLASHDRV_CMD_MODE,
        .hasAddress = true,
        .useAlternateBytes = false,
        .useAddressDTR = false,
        .useDataDTR = false,
        .useAutoPoll = false,
        .useAutoPollStop = false
    };

    if ( !_isValidAddress(address))
    {
        return FLASHDRV_ERROR;
    }

    // wait for write operations to finish
    if ( _AutoPollMemReady() != FLASHDRV_OK )
    {
        return FLASHDRV_ERROR;
    }

    // send read command
    FlashDrv_SendCommand(&command);

    // call spi read
    return FlashDrv_ReceiveData(buffer, _MAX_DEFAULT_TIMEOUT_IN_MS);
}

/*!*************************************************************************************************
 * @brief MX25L32_ReadX implements the 2xIO or 4xIO enhanced mode depending on the SPI interface
 * @param address
 * @param buffer
 * @param dataSize
 * @return status of the operation
 **************************************************************************************************/
FlashDrv_Status MX25L32_ReadX(uint32_t address, uint8_t* buffer,
                              uint32_t dataSize)
{
    // if dual-spi => read2
    FlashDrv_CommandDef  command = {
        .address = address,
        .nrOfDataBytes = dataSize,
        .instruction = MX25L32_2READ,
        .autopollMatch = 0x0,
        .autopollMask = 0x0,
        .autoPollIntervalCC = 0x0,
        .autoPollTimeoutCC = 0x0,
        .addressByteSize = 3,
        .nrOfInstructionLines = 1,
        .nrOfAddressLines = 2,
        .nrOfDataLines = 2,
        .nrOfDummyCycles = _GetnrOfDummyCycles(2),
        .autoPollMatchMode = FLASHDRV_MATCH_MODE_AND,
        .memoryMapModeConfig = FLASHDRV_CMD_MODE,
        .hasAddress = true,
        .useAlternateBytes = false,
        .useAddressDTR = false,
        .useDataDTR = false,
        .useAutoPoll = false,
        .useAutoPollStop = false
    };

    if ( !_isValidAddress(address))
    {
        return FLASHDRV_ERROR;
    }

    // if quad spi => read4, uses alternate bytes mode (bytes after address byte)
    if ( control.config.nrIOLines == 4 )
    {
        command.instruction = MX25L32_4READ;
        command.nrOfAddressLines = 4;
        command.nrOfDataLines = 4;
        command.nrOfDummyCycles = _GetnrOfDummyCycles(4);
    }
    else if ( control.config.nrIOLines != 2 )
    {
        return FLASHDRV_NOT_SUPPORTED;
    }

    // wait for write operations to finish
    if ( _AutoPollMemReady() != FLASHDRV_OK )
    {
        return FLASHDRV_ERROR;
    }

    // send read command
    FlashDrv_SendCommand(&command);

    // call spi read
    return FlashDrv_ReceiveData(buffer, _MAX_DEFAULT_TIMEOUT_IN_MS);
}

/*!*************************************************************************************************
 * @brief MX25L32_PageProgram
 * @param address
 * @param buffer
 * @param dataSize
 * @return
 **************************************************************************************************/
FlashDrv_Status MX25L32_PageProgram(uint32_t address, uint8_t* buffer, uint32_t dataSize)
{

    FlashDrv_CommandDef  command = {
        .address = address,
        .nrOfDataBytes = 0,
        .instruction = MX25L32_PP,
        .autopollMatch = 0x0,
        .autopollMask = 0x0,
        .autoPollIntervalCC = 0x0,
        .autoPollTimeoutCC = 0x0,
        .addressByteSize = 3,
        .nrOfInstructionLines = 1,
        .nrOfAddressLines = 1,
        .nrOfDataLines = 1,
        .nrOfDummyCycles = 0,
        .autoPollMatchMode = FLASHDRV_MATCH_MODE_AND,
        .memoryMapModeConfig = FLASHDRV_CMD_MODE,
        .hasAddress = true,
        .useAlternateBytes = false,
        .useAddressDTR = false,
        .useDataDTR = false,
        .useAutoPoll = false,
        .useAutoPollStop = false
    };

    if ( !_isValidAddress(address))
    {
        return FLASHDRV_ERROR;
    }

    if ( dataSize == 0 )
    {
        return FLASHDRV_ERROR;
    }

    // wait for write operations to finish
    _AutoPollMemReady();

    // get how many bytes until page end
    uint32_t             currentAddress = address;
    uint32_t             nrBytesToPageEnd = ( 0xFF - ( address & 0xFF )) + 1;
    FlashDrv_Status      status = FLASHDRV_OK;

    do
    {
        if ( dataSize > nrBytesToPageEnd ) // we need to wrap around
        {
            command.address = currentAddress;
            command.nrOfDataBytes = nrBytesToPageEnd;

            // every next iteration will start from page start
            // and can write the maximal page size
            if ( nrBytesToPageEnd < MX25L32_PAGE_SIZE )
            {
                nrBytesToPageEnd = MX25L32_PAGE_SIZE;
            }

        }
        else
        {
            command.nrOfDataBytes = dataSize; // last write operation
            command.address = currentAddress;
        }

        // send page program command
        _AutoPollMemReady();
        _WriteEnable();

        if ( FlashDrv_SendCommand(&command) != FLASHDRV_OK )
        {
            status = FLASHDRV_ERROR;
            break;
        }
        // increment buffer pointer, transmit data
        if ( FlashDrv_WriteData(buffer, _DEFAULT_TIMEOUT_IN_MS) != FLASHDRV_OK )
        {
            status = FLASHDRV_ERROR;
            break;
        }

        dataSize -= command.nrOfDataBytes;
        buffer += command.nrOfDataBytes;
        currentAddress += command.nrOfDataBytes;

    }
    while ( dataSize != 0 );

    _AutoPollMemReady();
    return status;
}

/*!*************************************************************************************************
 * @brief MX25L32_PageProgramX
 * @param address
 * @param buffer
 * @param dataSize
 * @return
 **************************************************************************************************/
FlashDrv_Status MX25L32_PageProgramX(uint32_t address, uint8_t* buffer, uint32_t dataSize)
{
    FlashDrv_CommandDef  command = {
        .address = address,
        .nrOfDataBytes = 0,
        .instruction = MX25L32_4PP,
        .autopollMatch = 0x0,
        .autopollMask = 0x0,
        .autoPollIntervalCC = 0x0,
        .autoPollTimeoutCC = 0x0,
        .addressByteSize = 3,
        .nrOfInstructionLines = 1,
        .nrOfAddressLines = 4,
        .nrOfDataLines = 4,
        .nrOfDummyCycles = 0,
        .autoPollMatchMode = FLASHDRV_MATCH_MODE_AND,
        .memoryMapModeConfig = FLASHDRV_CMD_MODE,
        .hasAddress = true,
        .useAlternateBytes = false,
        .useAddressDTR = false,
        .useDataDTR = false,
        .useAutoPoll = false,
        .useAutoPollStop = false
    };

    if ( !_isValidAddress(address))
    {
        return FLASHDRV_ERROR;
    }

    if ( dataSize == 0 )
    {
        return FLASHDRV_ERROR;
    }

    if ( control.config.nrIOLines != 4 )
    {
        return FLASHDRV_NOT_SUPPORTED;
    }

    // get how many bytes until page end
    uint32_t             currentAddress = address;
    uint32_t             nrBytesToPageEnd = ( 0xFF - ( address & 0xFF )) + 1;
    FlashDrv_Status      status = FLASHDRV_OK;

    do
    {
        if ( dataSize > nrBytesToPageEnd ) // we need to wrap around
        {
            command.address = currentAddress;
            command.nrOfDataBytes = nrBytesToPageEnd;

            // every next iteration will start from page start
            // and can write the maximal page size
            if ( nrBytesToPageEnd < MX25L32_PAGE_SIZE )
            {
                nrBytesToPageEnd = MX25L32_PAGE_SIZE;
            }

        }
        else
        {
            command.nrOfDataBytes = dataSize; // last write operation
            command.address = currentAddress;
        }

        // wait for write operations to finish
        if ( _AutoPollMemReady() != FLASHDRV_OK )
        {
            return FLASHDRV_ERROR;
        }

        _WriteEnable();

        if ( FlashDrv_SendCommand(&command) != FLASHDRV_OK )
        {
            status = FLASHDRV_ERROR;
            break;
        }
        // increment buffer pointer, transmit data
        if ( FlashDrv_WriteData(buffer, _DEFAULT_TIMEOUT_IN_MS) != FLASHDRV_OK )
        {
            status = FLASHDRV_ERROR;
            break;
        }

        dataSize -= command.nrOfDataBytes;
        buffer += command.nrOfDataBytes;
        currentAddress += command.nrOfDataBytes;

    }
    while ( dataSize != 0 );

    // wait for write operations to finish
    if ( _AutoPollMemReady() != FLASHDRV_OK )
    {
        return FLASHDRV_ERROR;
    }
    _WriteDisable();
    return status;
}

/*!*************************************************************************************************
 * @brief MX25L32_EraseChip
 * @return
 **************************************************************************************************/
FlashDrv_Status MX25L32_EraseChip(void)
{
    FlashDrv_CommandDef  command = {
        .address = 0x0,
        .nrOfDataBytes = 0,
        .instruction = MX25L32_CE,
        .autopollMatch = 0x0,
        .autopollMask = 0x0,
        .autoPollIntervalCC = 0x0,
        .autoPollTimeoutCC = 0x0,
        .addressByteSize = 3,
        .nrOfInstructionLines = 1,
        .nrOfAddressLines = 0,
        .nrOfDataLines = 0,
        .nrOfDummyCycles = 0,
        .autoPollMatchMode = FLASHDRV_MATCH_MODE_AND,
        .memoryMapModeConfig = FLASHDRV_CMD_MODE,
        .hasAddress = false,
        .useAlternateBytes = false,
        .useAddressDTR = false,
        .useDataDTR = false,
        .useAutoPoll = false,
        .useAutoPollStop = false
    };

    // wait for write operations to finish
    if ( _AutoPollMemReady() != FLASHDRV_OK )
    {
        return FLASHDRV_ERROR;
    }
    _WriteEnable();
    FlashDrv_Status      status = FlashDrv_SendCommand(&command);

    if ( _AutoPollMemReady() != FLASHDRV_OK )
    {
        return FLASHDRV_ERROR;
    }

    return status;
}

/*!*************************************************************************************************
 * @brief MX25L32_EraseSector
 * @param address
 * @return
 **************************************************************************************************/
FlashDrv_Status MX25L32_EraseSector(uint32_t address)
{
    FlashDrv_CommandDef  command = {
        .address = address, // set it to sector start address
        .nrOfDataBytes = 0,
        .instruction = MX25L32_SE,
        .autopollMatch = 0x0,
        .autopollMask = 0x0,
        .autoPollIntervalCC = 0x0,
        .autoPollTimeoutCC = 0x0,
        .addressByteSize = 3,
        .nrOfInstructionLines = 1,
        .nrOfAddressLines = 1,
        .nrOfDataLines = 0,
        .nrOfDummyCycles = 0,
        .autoPollMatchMode = FLASHDRV_MATCH_MODE_AND,
        .memoryMapModeConfig = FLASHDRV_CMD_MODE,
        .hasAddress = false,
        .useAlternateBytes = false,
        .useAddressDTR = false,
        .useDataDTR = false,
        .useAutoPoll = false,
        .useAutoPollStop = false
    };

    if ( !_isValidAddress(address))
    {
        return FLASHDRV_ERROR;
    }

    // wait for write operations to finish
    if ( _AutoPollMemReady() != FLASHDRV_OK )
    {
        return FLASHDRV_ERROR;
    }

    _WriteEnable();
    FlashDrv_Status      status = FlashDrv_SendCommand(&command);

    if ( _AutoPollMemReady() != FLASHDRV_OK )
    {
        return FLASHDRV_ERROR;
    }

    return status;
}

/*!*************************************************************************************************
 * @brief MX25L32_EraseBlock32kb
 * @param address
 * @return
 **************************************************************************************************/
FlashDrv_Status MX25L32_EraseBlock32kb(uint32_t address)
{
    FlashDrv_CommandDef  command = {
        .address = address, // set it to sector start address, valid for block
        .nrOfDataBytes = 0,
        .instruction = MX25L32_BE32,
        .autopollMatch = 0x0,
        .autopollMask = 0x0,
        .autoPollIntervalCC = 0x0,
        .autoPollTimeoutCC = 0x0,
        .addressByteSize = 3,
        .nrOfInstructionLines = 1,
        .nrOfAddressLines = 1,
        .nrOfDataLines = 0,
        .nrOfDummyCycles = 0,
        .autoPollMatchMode = FLASHDRV_MATCH_MODE_AND,
        .memoryMapModeConfig = FLASHDRV_CMD_MODE,
        .hasAddress = false,
        .useAlternateBytes = false,
        .useAddressDTR = false,
        .useDataDTR = false,
        .useAutoPoll = false,
        .useAutoPollStop = false
    };

    if ( !_isValidAddress(address))
    {
        return FLASHDRV_ERROR;
    }

    // wait for write operations to finish
    if ( _AutoPollMemReady() != FLASHDRV_OK )
    {
        return FLASHDRV_ERROR;
    }

    _WriteEnable();
    FlashDrv_Status      status = FlashDrv_SendCommand(&command);

    if ( _AutoPollMemReady() != FLASHDRV_OK )
    {
        return FLASHDRV_ERROR;
    }

    return status;
}

/*!*************************************************************************************************
 * @brief MX25L32_EraseBlock64kb
 * @param address
 * @return
 **************************************************************************************************/
FlashDrv_Status MX25L32_EraseBlock64kb(uint32_t address)
{
    FlashDrv_CommandDef  command = {
        .address = address, // set it to sector start address, valid for block
        .nrOfDataBytes = 0,
        .instruction = MX25L32_BE64,
        .autopollMatch = 0x0,
        .autopollMask = 0x0,
        .autoPollIntervalCC = 0x0,
        .autoPollTimeoutCC = 0x0,
        .addressByteSize = 3,
        .nrOfInstructionLines = 1,
        .nrOfAddressLines = 1,
        .nrOfDataLines = 0,
        .nrOfDummyCycles = 0,
        .autoPollMatchMode = FLASHDRV_MATCH_MODE_AND,
        .memoryMapModeConfig = FLASHDRV_CMD_MODE,
        .hasAddress = false,
        .useAlternateBytes = false,
        .useAddressDTR = false,
        .useDataDTR = false,
        .useAutoPoll = false,
        .useAutoPollStop = false
    };

    if ( !_isValidAddress(address))
    {
        return FLASHDRV_ERROR;
    }

    // wait for write operations to finish
    if ( _AutoPollMemReady() != FLASHDRV_OK )
    {
        return FLASHDRV_ERROR;
    }

    _WriteEnable();
    FlashDrv_Status      status = FlashDrv_SendCommand(&command);

    if ( _AutoPollMemReady() != FLASHDRV_OK )
    {
        return FLASHDRV_ERROR;
    }

    return status;
}

/*!*************************************************************************************************
 * @brief MX25L32_DeepPowerDown
 * @return
 **************************************************************************************************/
FlashDrv_Status MX25L32_DeepPowerDown(void)
{
    FlashDrv_CommandDef  command = {
        .address = 0x0,
        .nrOfDataBytes = 1,
        .instruction = MX25L32_DP,
        .autopollMatch = 0x0,
        .autopollMask = 0x0,
        .autoPollIntervalCC = 0x0,
        .autoPollTimeoutCC = 0x0,
        .addressByteSize = 3,
        .nrOfInstructionLines = 1,
        .nrOfAddressLines = 0,
        .nrOfDataLines = 0,
        .nrOfDummyCycles = 0,
        .autoPollMatchMode = FLASHDRV_MATCH_MODE_AND,
        .memoryMapModeConfig = FLASHDRV_CMD_MODE,
        .hasAddress = false,
        .useAlternateBytes = false,
        .useAddressDTR = false,
        .useDataDTR = false,
        .useAutoPoll = false,
        .useAutoPollStop = false
    };

    FlashDrv_Status      status = FlashDrv_SendCommand(&command);
    if ( status == FLASHDRV_OK )
    {
        control.isPoweredDown = true;
    }
    return status;
}

/*!*************************************************************************************************
 * @brief MX25L32_ReleaseDeepPowerDown
 * @return
 **************************************************************************************************/
FlashDrv_Status MX25L32_ReleaseDeepPowerDown(void)
{
    FlashDrv_CommandDef  command = {
        .address = 0x0,
        .nrOfDataBytes = 1,
        .instruction = MX25L32_RDP,
        .autopollMatch = 0x0,
        .autopollMask = 0x0,
        .autoPollIntervalCC = 0x0,
        .autoPollTimeoutCC = 0x0,
        .addressByteSize = 3,
        .nrOfInstructionLines = 1,
        .nrOfAddressLines = 0,
        .nrOfDataLines = 1,
        .nrOfDummyCycles = 8 * 3, //3 bytes
        .autoPollMatchMode = FLASHDRV_MATCH_MODE_AND,
        .memoryMapModeConfig = FLASHDRV_CMD_MODE,
        .hasAddress = false,
        .useAlternateBytes = false,
        .useAddressDTR = false,
        .useDataDTR = false,
        .useAutoPoll = false,
        .useAutoPollStop = false
    };

    uint8_t              res = 0x0; // we dont care about the RES-command => digital id, RES is
                                    // deprecated
    FlashDrv_SendCommand(&command);
    FlashDrv_Status      status = FlashDrv_ReceiveData(&res,_DEFAULT_TIMEOUT_IN_MS);

    if ( status == FLASHDRV_OK )
    {
        control.isPoweredDown = false;
    }
    return status;
}
/*!*************************************************************************************************
 * @brief MX25L32_Reset
 * @return
 **************************************************************************************************/
FlashDrv_Status MX25L32_Reset(void)
{
    FlashDrv_CommandDef  command = {
        .address = 0x0,
        .nrOfDataBytes = 0,
        .instruction = MX25L32_RSTEN,
        .autopollMatch = 0x0,
        .autopollMask = 0x0,
        .autoPollIntervalCC = 0x0,
        .autoPollTimeoutCC = 0x0,
        .addressByteSize = 3,
        .nrOfInstructionLines = 1,
        .nrOfAddressLines = 0,
        .nrOfDataLines = 0,
        .nrOfDummyCycles = 0,
        .autoPollMatchMode = FLASHDRV_MATCH_MODE_AND,
        .memoryMapModeConfig = FLASHDRV_CMD_MODE,
        .hasAddress = false,
        .useAlternateBytes = false,
        .useAddressDTR = false,
        .useDataDTR = false,
        .useAutoPoll = false,
        .useAutoPollStop = false
    };

    if ( control.isInitialized )
    {
        // wait for write operations to finish
        if ( _AutoPollMemReady() != FLASHDRV_OK )
        {
            return FLASHDRV_ERROR;
        }
    }

    FlashDrv_SendCommand(&command);
    command.instruction = MX25L32_RST;
    // device config will now be blank, back to power-up state -> run init again
    return FlashDrv_SendCommand(&command);
}

/*!*************************************************************************************************
 * @brief MX25L32_SetNOP
 * @return
 **************************************************************************************************/
FlashDrv_Status MX25L32_SetNOP(void)
{
    FlashDrv_CommandDef  command = {
        .address = 0x0,
        .nrOfDataBytes = 0,
        .instruction = MX25L32_NOP,
        .autopollMatch = 0x0,
        .autopollMask = 0x0,
        .autoPollIntervalCC = 0x0,
        .autoPollTimeoutCC = 0x0,
        .addressByteSize = 3,
        .nrOfInstructionLines = 1,
        .nrOfAddressLines = 0,
        .nrOfDataLines = 0,
        .nrOfDummyCycles = 0,
        .autoPollMatchMode = FLASHDRV_MATCH_MODE_AND,
        .memoryMapModeConfig = FLASHDRV_CMD_MODE,
        .hasAddress = false,
        .useAlternateBytes = false,
        .useAddressDTR = false,
        .useDataDTR = false,
        .useAutoPoll = false,
        .useAutoPollStop = false
    };

    return FlashDrv_SendCommand(&command);
}


/*!*************************************************************************************************
 * @brief MX25L32_ReadSecurityRegister
 * @param secRegister
 * @return
 **************************************************************************************************/
FlashDrv_Status MX25L32_ReadSecurityRegister(uint8_t* secRegister)
{
    FlashDrv_CommandDef  command = {
        .address = 0x0,
        .nrOfDataBytes = 1,
        .instruction = MX25L32_RDSCUR,
        .autopollMatch = 0x0,
        .autopollMask = 0x0,
        .autoPollIntervalCC = 0x0,
        .autoPollTimeoutCC = 0x0,
        .addressByteSize = 3,
        .nrOfInstructionLines = 1,
        .nrOfAddressLines = 0,
        .nrOfDataLines = 1,
        .nrOfDummyCycles = 0,
        .autoPollMatchMode = FLASHDRV_MATCH_MODE_AND,
        .memoryMapModeConfig = FLASHDRV_CMD_MODE,
        .hasAddress = false,
        .useAlternateBytes = false,
        .useAddressDTR = false,
        .useDataDTR = false,
        .useAutoPoll = false,
        .useAutoPollStop = false
    };

    FlashDrv_SendCommand(&command);
    return FlashDrv_ReceiveData(secRegister,_DEFAULT_TIMEOUT_IN_MS);
}
/*!*************************************************************************************************
 * @brief MX25L32_WriteSecurityRegister
 * @param secRegister
 * @return
 **************************************************************************************************/
FlashDrv_Status MX25L32_WriteSecurityRegister(uint8_t secRegister)
{
    FlashDrv_CommandDef  command = {
        .address = 0x0,
        .nrOfDataBytes = 0,
        .instruction = MX25L32_WRSCUR,
        .autopollMatch = 0x0,
        .autopollMask = 0x0,
        .autoPollIntervalCC = 0x0,
        .autoPollTimeoutCC = 0x0,
        .addressByteSize = 3,
        .nrOfInstructionLines = 1,
        .nrOfAddressLines = 0,
        .nrOfDataLines = 0,
        .nrOfDummyCycles = 0,
        .autoPollMatchMode = FLASHDRV_MATCH_MODE_AND,
        .memoryMapModeConfig = FLASHDRV_CMD_MODE,
        .hasAddress = false,
        .useAlternateBytes = false,
        .useAddressDTR = false,
        .useDataDTR = false,
        .useAutoPoll = false,
        .useAutoPollStop = false
    };

    // wait for write operations to finish
    if ( _AutoPollMemReady() != FLASHDRV_OK )
    {
        return FLASHDRV_ERROR;
    }

    _WriteEnable();
    FlashDrv_Status      status = FlashDrv_SendCommand(&command);
    return status;
}

/*!*************************************************************************************************
 * @brief MX25L32_ReadREMS
 * @param manufacturer
 * @param deviceId
 * @return
 **************************************************************************************************/
FlashDrv_Status MX25L32_ReadREMS(uint8_t* manufacturer, uint8_t* deviceId)
{
    uint8_t              idBuffer[2] = {0x0,0x0};

    FlashDrv_CommandDef  command = {
        .address = 0x0,
        .nrOfDataBytes = 2,
        .instruction = MX25L32_REMS,
        .autopollMatch = 0x0,
        .autopollMask = 0x0,
        .autoPollIntervalCC = 0x0,
        .autoPollTimeoutCC = 0x0,
        .addressByteSize = 3,
        .nrOfInstructionLines = 1,
        .nrOfAddressLines = 1,
        .nrOfDataLines = 1,
        .nrOfDummyCycles = 0,
        .autoPollMatchMode = FLASHDRV_MATCH_MODE_AND,
        .memoryMapModeConfig = FLASHDRV_CMD_MODE,
        .hasAddress = true,
        .useAlternateBytes = false,
        .useAddressDTR = false,
        .useDataDTR = false,
        .useAutoPoll = false,
        .useAutoPollStop = false
    };

    FlashDrv_SendCommand(&command);
    FlashDrv_Status      status = FlashDrv_ReceiveData(idBuffer,_DEFAULT_TIMEOUT_IN_MS);

    *manufacturer = idBuffer[0];
    *deviceId = idBuffer[1];
    return status;
}


/*!*************************************************************************************************
 * @brief MX25L32_ReadSFDPValue
 * @param address
 * @param dataSize table is read out from 1 byte to datasize
 * @return
 **************************************************************************************************/
FlashDrv_Status MX25L32_ReadSFDPValue(uint32_t address, uint8_t* buffer, uint8_t dataSize)
{
    FlashDrv_CommandDef  command = {
        .address = address,
        .nrOfDataBytes = dataSize,
        .instruction = MX25L32_RDSFDP,
        .autopollMatch = 0x0,
        .autopollMask = 0x0,
        .autoPollIntervalCC = 0x0,
        .autoPollTimeoutCC = 0x0,
        .addressByteSize = 3,
        .nrOfInstructionLines = 1,
        .nrOfAddressLines = 1,
        .nrOfDataLines = 1,
        .nrOfDummyCycles = 8,
        .autoPollMatchMode = FLASHDRV_MATCH_MODE_AND,
        .memoryMapModeConfig = FLASHDRV_CMD_MODE,
        .hasAddress = true,
        .useAlternateBytes = false,
        .useAddressDTR = false,
        .useDataDTR = false,
        .useAutoPoll = false,
        .useAutoPollStop = false
    };

    FlashDrv_SendCommand(&command);
    return FlashDrv_ReceiveData(buffer,_DEFAULT_TIMEOUT_IN_MS);
}
