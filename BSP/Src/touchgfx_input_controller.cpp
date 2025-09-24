/*!*******************************************************************************
 * @file touchgfx_input_controller.cpp
 *
 * @brief This file contains the implementation of a custom input controller for
 * 		  TouchGfx.
 *
 * @author Enovation Controls
 *
 * @copyright Copyright 2023 by Enovation Controls.
 *            This file and/or parts of it are subject to Enovation Controls
 *            software license terms (SoLiT, Version 1.16.2).
 *            With the use of this software you accept the SoLiT.
 *            Without written approval of Enovation Controls this
 *            software may not be copied, redistributed or
 *            used in any other way than stated in the conditions of the SoLiT.
 *
 *******************************************************************************/

#include "touchgfx_input_controller.hpp"

void TouchGfxInputController::init() {
}

bool TouchGfxInputController::sample( uint8_t& key ) {
  bool currentKeyStates[buttonCount] =
    { 0, 0, 0, 0 };

  currentKeyStates[Button_0] = !HAL_GPIO_ReadPin(nBUTTON0_GPIO_Port,
                                                 nBUTTON0_Pin);
  currentKeyStates[Button_1] = !HAL_GPIO_ReadPin(nBUTTON1_GPIO_Port,
                                                 nBUTTON1_Pin);
  currentKeyStates[Button_2] = !HAL_GPIO_ReadPin(nBUTTON2_GPIO_Port,
                                                 nBUTTON2_Pin);
  currentKeyStates[Button_3] = !HAL_GPIO_ReadPin(nBUTTON3_GPIO_Port,
                                                 nBUTTON3_Pin);

  // ToDo: only for debug, can be useful as a heartbeat on the touchgfx task
//  HAL_GPIO_TogglePin(DOUT_nRUN_LED_GPIO_Port, DOUT_nRUN_LED_Pin);

  // only allow one button press
  for (uint8_t i = 0; i < buttonCount; i++) {
    if (keyStates[i] != currentKeyStates[i]) {
      if (currentKeyStates[i]) {
        keyStates[i] = true;
        return false;
      }
      else { // trigger button press only on release
        key = i;
        keyStates[i] = false;
        return true;
      }
    }
  }

  return false;
}

//called from handleTickEvent (60hz)
bool TouchGfxInputController::isButtonHeld(uint8_t button)
{
    if (keyStates[button])
    {
        if (holdStates[button] && holdCounters[button] >= 20)
        {
            return true;
        }

        holdStates[button] = true;
        holdCounters[button]++;
    }
    else
    {
        holdStates[button] = false;
        holdCounters[button] = 0;
    }
    return false;
}
