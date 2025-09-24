/*!************************************************************************************************
 * @file STM32L4R7_mx25l3233f.c
 *
 * @brief Contains the interface implementation for the mx25l3233f
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

/* Includes
   ----------------------------------------------------------------------------------------------*/
#include "flashdriver_interface.h"
#include <stm32l4xx_hal.h>

/* Defines
   ----------------------------------------------------------------------------------------------*/
extern OSPI_HandleTypeDef  hospi1;

/* Typedefs
   ----------------------------------------------------------------------------------------------*/

/* Private variables
   ----------------------------------------------------------------------------------------------*/

/* Private interface
   ----------------------------------------------------------------------------------------------*/

/* Public interface
   ----------------------------------------------------------------------------------------------*/

/*!*************************************************************************************************
 * @brief FlashDrv_SendCommand maps a generic command to the spi commands of the platform
 * @param command definition
 * @return status of the operation
 **************************************************************************************************/
FlashDrv_Status FlashDrv_SendCommand(FlashDrv_CommandDef* command)
{
    OSPI_RegularCmdTypeDef   cmd;
    OSPI_AutoPollingTypeDef  autopoll_config;

    assert_param(command);
    cmd.FlashId = HAL_OSPI_FLASH_ID_1;
    cmd.Address = command->address;
    cmd.AddressSize = HAL_OSPI_ADDRESS_24_BITS;
    cmd.AddressDtrMode =
        ( command->useAddressDTR ) ? HAL_OSPI_ADDRESS_DTR_ENABLE : HAL_OSPI_ADDRESS_DTR_DISABLE;

    switch ( command->nrOfAddressLines )
    {
        case 1:
            cmd.AddressMode = HAL_OSPI_ADDRESS_1_LINE;
            break;
        case 2:
            cmd.AddressMode = HAL_OSPI_ADDRESS_2_LINES;
            break;
        case 4:
            cmd.AddressMode = HAL_OSPI_ADDRESS_4_LINES;
            break;
        case 8:
            cmd.AddressMode = HAL_OSPI_ADDRESS_8_LINES;
            break;
        default:
            cmd.AddressMode = HAL_OSPI_ADDRESS_NONE;
            break;
    }

    if ( command->useAlternateBytes )
    {
        cmd.AlternateBytes = command->alternateBytes;
        cmd.AlternateBytesDtrMode = HAL_OSPI_ALTERNATE_BYTES_DTR_ENABLE;
        cmd.AlternateBytesMode = HAL_OSPI_ALTERNATE_BYTES_4_LINES;
        cmd.AlternateBytesSize = 1;
    }
    else
    {
        cmd.AlternateBytes = 0x0;
        cmd.AlternateBytesDtrMode = HAL_OSPI_ALTERNATE_BYTES_DTR_DISABLE;
        cmd.AlternateBytesMode = HAL_OSPI_ALTERNATE_BYTES_NONE;
        cmd.AlternateBytesSize = 0;
    }

    cmd.DataDtrMode =
        ( command->useDataDTR ) ? HAL_OSPI_DATA_DTR_ENABLE : HAL_OSPI_DATA_DTR_DISABLE;
    cmd.DummyCycles = command->nrOfDummyCycles;
    cmd.NbData = command->nrOfDataBytes;


    if ( cmd.NbData > 8 )
    {
        HAL_OSPI_SetFifoThreshold(&hospi1, 8);
    }
    else
    {
        HAL_OSPI_SetFifoThreshold(&hospi1, cmd.NbData);
    }

    switch ( command->nrOfDataLines )
    {
        case 1:
            cmd.DataMode = HAL_OSPI_DATA_1_LINE;
            break;
        case 2:
            cmd.DataMode = HAL_OSPI_DATA_2_LINES;
            break;
        case 4:
            cmd.DataMode = HAL_OSPI_DATA_4_LINES;
            break;
        case 8:
            cmd.DataMode = HAL_OSPI_DATA_8_LINES;
            break;
        default:
            cmd.DataMode = HAL_OSPI_DATA_NONE;
            break;
    }

    cmd.InstructionSize = HAL_OSPI_INSTRUCTION_8_BITS;
    cmd.InstructionDtrMode = HAL_OSPI_INSTRUCTION_DTR_DISABLE;
    cmd.Instruction = command->instruction;
    cmd.InstructionMode = HAL_OSPI_INSTRUCTION_1_LINE;
    cmd.DQSMode = HAL_OSPI_DQS_DISABLE;
    cmd.SIOOMode = HAL_OSPI_SIOO_INST_EVERY_CMD;

    // config for memory mapped read/write
    switch ( command->memoryMapModeConfig )
    {
        case FLASHDRV_MM_READ:
            cmd.OperationType = HAL_OSPI_OPTYPE_READ_CFG;
            break;
        case FLASHDRV_MM_WRITE:
            cmd.OperationType = HAL_OSPI_OPTYPE_WRITE_CFG;
            break;
        case FLASHDRV_CMD_MODE:
        default:
            cmd.OperationType = HAL_OSPI_OPTYPE_COMMON_CFG;
            break;
    }

    if ( HAL_OSPI_Command(&hospi1, &cmd, 500 /*HAL_OSPI_TIMEOUT_DEFAULT_VALUE*/) != HAL_OK )
    {
        return FLASHDRV_ERROR;
    }

    // configure auto polling
    if ( command->useAutoPoll )
    {

        autopoll_config.Match = command->autopollMatch;
        autopoll_config.Mask = command->autopollMask;
        autopoll_config.Interval = command->autoPollIntervalCC;
        autopoll_config.AutomaticStop = ( command->useAutoPollStop ) ?
                                        HAL_OSPI_AUTOMATIC_STOP_ENABLE:
                                        HAL_OSPI_AUTOMATIC_STOP_DISABLE;
        autopoll_config.MatchMode = ( command->autoPollMatchMode == FLASHDRV_MATCH_MODE_OR )?
                                    HAL_OSPI_MATCH_MODE_OR:
                                    HAL_OSPI_MATCH_MODE_AND;

        if ( HAL_OSPI_AutoPolling(&hospi1, &autopoll_config, command->autoPollTimeoutCC) != HAL_OK )
        {
            return FLASHDRV_ERROR;
        }
    }

    return FLASHDRV_OK;
}

/*!*************************************************************************************************
 * @brief FlashDrv_ReceiveData receives in a blocking call data from OSPI, until timeout is reached.
 * @param buffer
 * @param timeout
 * @return status of the operation
 **************************************************************************************************/
FlashDrv_Status FlashDrv_ReceiveData(uint8_t* buffer, uint32_t timeout)
{
    if ( HAL_OSPI_Receive(&hospi1, buffer, timeout) != HAL_OK )
    {
        return FLASHDRV_ERROR;
    }
    return FLASHDRV_OK;
}

/*!*************************************************************************************************
 * @brief FlashDrv_WriteData writes a chunk of data in a blocking call, until timeout is reached.
 * @param buffer
 * @param timeout
 * @return status of the operation
 **************************************************************************************************/
FlashDrv_Status FlashDrv_WriteData(uint8_t* buffer, uint32_t timeout)
{


    if ( HAL_OSPI_Transmit(&hospi1, buffer, timeout) != HAL_OK )
    {
        return FLASHDRV_ERROR;
    }

    return FLASHDRV_OK;
}
