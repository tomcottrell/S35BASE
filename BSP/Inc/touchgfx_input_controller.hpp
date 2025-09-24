/*!*******************************************************************************
 * @file touchgfx_input_controller.hpp
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

#ifndef TOUCHGFX_INPUT_CONTROLLER_HPP
#define TOUCHGFX_INPUT_CONTROLLER_HPP

#include <main.h>
#include <touchgfx/hal/HAL.hpp>


class TouchGfxInputController: public touchgfx::ButtonController {
public:
	void init();
	bool sample(uint8_t &key);

	enum Buttons {
	  Button_0,
	  Button_1,
	  Button_2,
	  Button_3
	};

	static constexpr uint8_t buttonCount = 4;

	bool isButtonHeld(uint8_t button);

private:
	bool keyStates[buttonCount] = {0, 0, 0, 0};
	bool holdStates[buttonCount] = {false, false, false, false};
	unsigned int holdCounters[buttonCount] = {0, 0, 0, 0};
};

#endif
