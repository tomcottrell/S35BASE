/*!*******************************************************************************
 * @file button_handler.cpp
 *
 *
 * @brief This contains a handler for TouchGfx buttons, animating / triggering visuals
 * of SW buttons in TouchGfx, when hardware buttons are used.
 *
 * @author Enovation Controls
 *
 * @copyright Copyright 2020 by Enovation Controls.
 *            This file and/or parts of it are subject to Enovation Controls
 *            software license terms (SoLiT, Version 1.16.2).
 *            With the use of this software you accept the SoLiT.
 *            Without written approval of Enovation Controls this
 *            software may not be copied, redistributed or
 *            used in any other way than stated in the conditions of the SoLiT.
 *
 *******************************************************************************/
#include "button_handler.hpp"

void ButtonHandler::handleKey( uint8_t key ) {
  if (key > numberOfButtons) {
    return;
  }

  currentButton = buttons[key];
  ClickEvent ev = ClickEvent(ClickEvent::PRESSED, currentButton->getX(),
                             currentButton->getY());
  currentButton->handleClickEvent(ev);
  waitForDelay = true;
}

bool ButtonHandler::handleTickEvent() {
  static uint16_t counter = 0;

  if (waitForDelay) {
    counter++;

    if (counter >= delayInTicks) {
      counter = 0;
      waitForDelay = false;
      if (currentButton != nullptr) {
        ClickEvent ev = ClickEvent(ClickEvent::RELEASED, currentButton->getX(),
                                   currentButton->getY());
        currentButton->handleClickEvent(ev);
        currentButton = nullptr;
        return true;
      }
    }
  }
  return false;
}
