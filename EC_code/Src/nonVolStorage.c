////////////////////////////////////////////////////////////////////////////////////////////////////
// fileName: can.c
//
// Description: Low level CAN functions
//
// Author: Enovation controls
//
//         http://www.enovationcontrols.com
//
// copyright Copyright 2024 by Enovation Controls.
//            This file and/or parts of it are subject to
//            software license terms.
//
//            Without written approval of Enovation Controls this
//            software may not be copied, redistributed or used
//            in any other way.
//
////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "mtest.h"
#include "nonVolStorage.h"
#include "fs_layer.h"
#include "TickTimers.h"
#include "ConfigMemory.h"
#include "math.h"
#include "stm32l4xx_hal.h"


////////////////////////////////////////////////////////////////////////////////////////////////////
//Defines
////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////////////////
// Typedefs
////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////////////////
// Private variables
////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////////////////
// Private interface
////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////////////////
// Public interface
////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////
//
// loadConfigFile()
//
// Load config file if it exists and is valid.  Default values loaded if error detected
//
//    Parameters:
//       none
//
////////////////////////////////////////////////////////////////////////
void LoadConfigFile()
{
    char filename[] = CONFIG_FILENAME;
    char fileReadBuffer[((CONFIG_LENGTH + 1) * 6) + 4];

    uint16_t configPointer = 0;
    uint16_t i;
    uint16_t calculatedChecksum = 0;
    uint16_t configFileData[CONFIG_LENGTH];
    uint16_t configFileChecksum = 0;
    int result;

    //Check config file exists and is valid
    result = FS_read(filename, fileReadBuffer, ((CONFIG_LENGTH + 1) * 6) + 4);

    if(result == 0)
    {
        //File read ok: Extract config file and checksum
        char* token = strtok(fileReadBuffer, ",");

        while(token != NULL)
        {
            if(configPointer < CONFIG_LENGTH)  // Config data (0-32)
            {
                //Config data
                configFileData[configPointer] = atoi(token);
                calculatedChecksum += configFileData[configPointer];
            }
            else if(configPointer == CONFIG_LENGTH)  // Checksum (33)
            {
                //Checksum
                configFileChecksum = atoi(token);
            }
            ++configPointer;
            token = strtok(NULL, ",");
        }
    }

    //Check file length - should be CONFIG_LENGTH + 1 (33 config values + 1 checksum = 34 total)
    if(configPointer != CONFIG_LENGTH + 1)
    {
        //File size changed
        result = 2;
    }

    //Check checksum validity
    calculatedChecksum = ~calculatedChecksum;
    ++calculatedChecksum;

    if(calculatedChecksum != configFileChecksum)
    {
        //Checksum error
        result = 1;
    }

    FS_umount();

    //Check to copy file data to S35_config or restore default settings
    if(result == 0)
    {
        //File present and valid: copy to S35_config
        for(i=0; i<CONFIG_LENGTH; ++i)
            S35_config[i] = configFileData[i];

        printf("Config file valid\n\r");
    }
    else
    {
        //File read error or file checksum incorrect: use default values
        SetDefaultConfig();
        SaveConfigFile();
        printf("Config file load error, checksum error or length error: Default values loaded\n\r");
    }
}

////////////////////////////////////////////////////////////////////////
//
// saveConfigFile()
//
// Saves config (settings) file to littlefs file in CSV format
//
//    Parameters:
//       none
//
////////////////////////////////////////////////////////////////////////
void SaveConfigFile()
{
    uint16_t i;
    char ConfigDataString[(CONFIG_LENGTH * 6) + 4] = "";
    char filename[] = CONFIG_FILENAME;
    uint16_t checksum = 0;
    int result;
    char FileVerifyBuffer[(CONFIG_LENGTH * 6) + 4];

    FS_mount();

    //Create CSV string to write to file
    for(i=0; i<CONFIG_LENGTH; ++i)  // ONLY CHANGE: < instead of <=
    {
        sprintf(ConfigDataString + strlen(ConfigDataString), "%d,", S35_config[i]);
        checksum += S35_config[i];
    }

    //Append checksum
    checksum = ~checksum + 1;
    sprintf(ConfigDataString + strlen(ConfigDataString), "%d\n\r", checksum);

    //Write config data to file (overwriting existing file)
    result = FS_write(filename, ConfigDataString, sizeof(ConfigDataString), false);

    // check if file got created
    if(result == 0)
    {
        lfs_t* lfs = FS_getFS();
        lfs_file_t  lfsFile;
        result = lfs_file_open(lfs, &lfsFile, filename, LFS_O_RDONLY);
        if(result == 0)
        {
            //check file length and read file back in to RAM
            result = lfs_file_read(lfs, &lfsFile, &FileVerifyBuffer, (CONFIG_LENGTH * 6) + 4);
            if(result == 0)
            {
                //Display file contents (string)
                printf("Config file saved content: %s\n\r", FileVerifyBuffer);
            }
            // ADD ONLY THIS LINE:
            lfs_file_close(lfs, &lfsFile);
        }
    }

    //Unmount
    FS_umount();

    //update all features affected by changed config data
    UpdateAllConfigFeatures();
}
