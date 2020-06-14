#pragma once

/*
	Computer architectures semestral assignment - Zelinka Josef & Michal Vojtech

	This file contains interface to two connected RGB leds. Each of them is wrapped by a global
	object exposing write methods, which modify the selected color.
*/

#include "snake-options.hpp"
#include "mzapo_regs.h"
#include <array>
#include <type_traits>
#include <climits>

namespace led {

	/*Symbolic constants suitable for selecting one particular color.
	Be aware that these colors are not very exact at the moment beacuse of differences
	in brightness of individual color componenets. Basic colors (RGB and white/black) are exact.*/
	enum class Color {
		black,
		red,
		green,
		blue,
		yellow,
		purple,
		brown,
		white
	};

	/*Wrapper of rgb led peripheral. Supports various means of color selection. */
	class RGB_LED {

		//Internal pointer to the peripheral's base. Offseted by data_reg_.
		inline static unsigned char volatile* peripheral_ = nullptr;
		std::uintptr_t data_reg_ = 0;


	public:
		RGB_LED(std::uintptr_t data_reg);
		~RGB_LED() { write(led::Color::black); }

		//Compose color o the LED from separate components. Each argument must fit into a byte
		void write(unsigned red, unsigned green, unsigned blue);
		//Pass hex code of desired color. Three byte value with one byte per each R, G, B component
		void write(std::uint32_t color);
		//Use selected symbolic constant as LED's color
		void write(Color c);

	};

	//Global objects wrapping the peripheral. 
	//As an array
	inline std::array<RGB_LED, 2> rgbs = { {SPILED_REG_LED_RGB1_o, SPILED_REG_LED_RGB2_o} };
	//As individual named variables
	inline RGB_LED& rgb_left = rgbs.front();
	inline RGB_LED& rgb_right = rgbs.back();

}


