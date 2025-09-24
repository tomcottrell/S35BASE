/*!*************************************************************************************************
 * @file usb_helper.h
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

#ifndef USB_HELPER_H
#define USB_HELPER_H

/* Includes
   -----------------------------------------------------------------------------------------------*/
#include <cmsis_os2.h>

/* Defines
   -----------------------------------------------------------------------------------------------*/

/* Typedefs
   -----------------------------------------------------------------------------------------------*/

/* Private variables
   -----------------------------------------------------------------------------------------------*/

/* Private interface
   -----------------------------------------------------------------------------------------------*/

/* Public interface
   -----------------------------------------------------------------------------------------------*/

// callback, called when usb stick is plugged in
void OnUSBMount(void);

// callback, called when usb stick is removed
void OnUSBUmount(void);

void USB_trackResumeEvent(void);

#endif
