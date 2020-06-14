#pragma once

/*
	Computer architectures semestral assignment - Zelinka Josef & Michal Vojtìch

	This file allows the user to interface a line of 32 LEDs located near the bottom edge 
	of the board. It is possible to turn individual LEDs on by the means of writing a 32bit 
	binary number or splitting the line in half and displaying two separate numbers in base one.
*/

#include "snake-options.hpp"
#include "mzapo_regs.h"
#include <array>
#include <climits>

namespace led {

	//Wrapper around a single LED in the strip. It is aware of its index and state
	class LED {
		friend class LED_line;

		unsigned index_ = 0;
		unsigned mask_ = 0;
		bool on_ = false;

		//Switches the state to given value, but does not redraw anything. 
		//LED_line::write must be used to write actual data 
		void operator=(bool turn_on) {
			on_ = turn_on;
		}

		operator bool() const {
			return on_;
		}
	};

	/*Wrapper around the line strip managing peripheral. Exposes means of writing and reading 
	the current state, as well as selecting individual LEDS. */
	class LED_line {
		static_assert(LED_line_length == sizeof(std::uint32_t) * CHAR_BIT, "Limited because of std::uint32_t's size.");

		//Base address of the peripheral and offset to control register
		std::uintptr_t data_register_;
		unsigned char volatile* const peripheral_;

		//leds_[0] is the rightmost LED from the front view
		std::array<LED, LED_line_length> leds_;

	public:
		LED_line(std::uintptr_t reg);

		//Displays given number in base two with LSB on the right edge
		void write(std::uint32_t binary_value);

		//Light up given number of LEDs
		void write_base_one(unsigned val);

		//Returns a number with i-th bit on iff i-th LED is on. Counted from the right
		std::uint32_t read() const;

		//Converts both scores (which must not exceed 16) to base one and turn on 
		//that many LEDs from the left and right edge. Used to display player score
		void display_scores_base_one(unsigned left, unsigned right);

	};

	//Global object encapsulating interaction with the peripheral
	inline LED_line line{ SPILED_REG_LED_LINE_o };

}

