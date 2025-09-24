/*!*******************************************************************************
 * @file button_handler.hpp
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
#ifndef BUTTON_HANDLER_HPP
#define BUTTON_HANDLER_HPP

#include <touchgfx/widgets/Button.hpp>

using namespace touchgfx;

class ButtonHandler {
public:
  ButtonHandler(uint16_t delay, Button* button0, Button* button1, Button* button2, Button* button3):
    delayInTicks(delay), buttons{button0, button1, button2, button3}{}

  void handleKey(uint8_t key);
  bool handleTickEvent();

protected:
  static constexpr uint8_t numberOfButtons = 4;
  uint16_t delayInTicks = 0;
  Button* buttons[numberOfButtons];
  Button* currentButton = nullptr;
  bool waitForDelay = false;
};

#endif
