#pragma once

/*
	Computer architectures semestral assignment - Zelinka Josef & Michal Vojtìch

	This file contains interface to three knobs available on the board.
	It is possible to poll their current angle as well as information whether they are pressed.
*/

#include "snake-options.hpp"
#include "mzapo_regs.h"
#include <array>
#include <type_traits>
#include <climits>

namespace knobs {


	/* Wrapper of one knob. It is aware of its index (necessary for register masking). Exposes methods
		returning information whether the knob is currently pressed and its current position. */
	class Knob {

		//Peripheral base address and offset registrer
		constexpr static std::uintptr_t data_reg = SPILED_REG_KNOBS_8BIT_o;
		inline static unsigned char volatile* peripheral_;

		int index_;

	public:
		Knob(int index);

		/* Returns true iff the given knob is presed right now. */
		bool pressed();

		/* Returns relative angle of the knob where one turn corresponds to a difference of 100 units.
		It is not possible to calibrate the knob to give an absolute position, thus only changes of this
		reading have some meaning.*/
		unsigned angle() const;

	};

	//Global objects encapsulating the three knobs
	//As a single array
	inline std::array<Knob, 3> knobs{ {2,1,0} };

	//As three individual variables
	inline Knob& red = knobs[0];
	inline Knob& green = knobs[1];
	inline Knob& blue = knobs[2];

}
