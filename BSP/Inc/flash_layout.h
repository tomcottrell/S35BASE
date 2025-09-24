/*!*************************************************************************************************
 * @file flash_layout.h
 *
 * @brief Contains flash addresses of data on on-chip flash
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

#ifndef FLASH_LAYOUT_H
#define FLASH_LAYOUT_H

/* Includes
   -----------------------------------------------------------------------------------------------*/

/* Defines
   -----------------------------------------------------------------------------------------------*/

#define DDATA_SECTION_SIZE 		0x1000 // 4kb Page
#define DDATA_SECTION_ADDRESS 	0x8014000
#define DDATA_CRC_ADR			0x8014000
#define DDATA_FINGERPRINT_ADR   0x8014004
#define DDATA_PARTNUMBER_ADR    0x8014024
#define DDATA_SERIALNUMBER_ADR  0x801402C
#define DDATA_PN_STRING			0x8014034
#define DDATA_SN_STRING			0x8014056

#define APP_HEADER_SECTION_ADR  0x8015000

/* Typedefs
   -----------------------------------------------------------------------------------------------*/

/* Private variables
   -----------------------------------------------------------------------------------------------*/

/* Private interface
   -----------------------------------------------------------------------------------------------*/

/* Public interface
   -----------------------------------------------------------------------------------------------*/

typedef union
{
    uint8_t data[APP_HEADER_SIZE];
    struct
    {
        uint32_t crc;         // crc of the app-header + app [size -> | APP ]
        uint32_t size;        // size of [size -> | APP ], size of crc input data
        uint8_t  version_major;
        uint8_t  version_minor;
        uint8_t  version_patch;
        uint8_t  name[APP_NAME_LENGTH];
        uint8_t  tbd[APP_HEADER_RESERVED];
    }__attribute__(( packed ));
} AppHeaderSection;

typedef union
{
    uint8_t data[DEVICE_DATA_SIZE];
    struct
    {
        uint32_t crc;
        uint8_t  fingerprint[32];
        uint64_t partNumber;
        uint64_t serialNumber;
        char     partNumberString[22];
        char     serialNumberString[22];
    } __attribute__(( packed ));
} DeviceDataSection;

#endif
