/*!*************************************************************************************************
 * @file usb_helper.c
 *
 * @brief Contains some example implementation for USB handling of mass storage devices
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
#include "usb_helper.h"
#include "fs_layer.h"

/* Defines
   -----------------------------------------------------------------------------------------------*/

/* Typedefs
   -----------------------------------------------------------------------------------------------*/

/* Private variables
   -----------------------------------------------------------------------------------------------*/

static bool           usbConnected = false;

/* Private interface
   -----------------------------------------------------------------------------------------------*/

/* Public interface
   -----------------------------------------------------------------------------------------------*/

/* Public interface
   -----------------------------------------------------------------------------------------------*/

/*!*************************************************************************************************
 * @brief OnUSBMount callback, is called when usb stick is plugged in and ready
 *
 **************************************************************************************************/
void OnUSBMount(void)
{
    // wake up usb device task
    FS_setUSBMountStatus(true);
}

/*!*************************************************************************************************
 * @brief OnUSBUnmount callback, is called when usb stick is disconnected
 *
 **************************************************************************************************/
void OnUSBUmount(void)
{
    // suspend usb device task
    FS_setUSBMountStatus(false);
}

/*!*************************************************************************************************
 * @brief trackUSBResumeEvent is just a temporary implementation, to track usb-connection.
 * This has to be done later by VBus sensing & interrupts. On PTB rev100 and rev200, the pin for vbus sensing has not
 * the correct logical states and voltage levels. This might also explain, why callbacks from the usb
 * stack are not generated correctly.
 *
 **************************************************************************************************/
void USB_trackResumeEvent(void)
{
    usbConnected = !usbConnected;

    if(usbConnected){
      OnUSBMount();
    }else {
      OnUSBUmount();
    }
}
