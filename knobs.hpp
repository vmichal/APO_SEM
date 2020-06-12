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
#include <chrono>

namespace knobs {


	/* Wrapper of one knob. It is aware of its index (necessary for register masking). Exposes methods
		returning information whether the knob is currently pressed and its current position. */
	class Knob {

		//Peripheral base address and offset registrer
		constexpr static std::uintptr_t data_reg = SPILED_REG_KNOBS_8BIT_o;
		inline static unsigned char volatile* peripheral_;

		int index_;

	public:
		explicit Knob(int index);

		/* Returns true iff the given knob is presed right now. */
		bool pressed() const;

		/* Returns relative angle of the knob where one turn corresponds to a difference of 100 units.
		It is not possible to calibrate the knob to give an absolute position, thus only changes of this
		reading have some meaning.*/
		int angle() const;

	};

	//Since trying to interpret the reading of knob sensor as absolute position is totally insane
	//it is strongly recommended to use KnobManager that returns this 
	enum class Rotation {
		none,
		clockwise,
		counterclockwise
	};

	/* Monitors given knob and records all actions that occured since the last poll.
	The user has to call a method 'sample' in the main loop and later receive all the results
	via a call to 'pressed or movement methods'.*/
	class KnobManager {

		Knob const& knob_;
		bool pressed_ = false;
		int rotation_ = 0;
		int last_position_ = 0;

		std::chrono::steady_clock::time_point last_poll_ = std::chrono::steady_clock::now();

	public:
		KnobManager(Knob const& knob) : knob_{ knob } {}

		/*Queries the position of the knob and updates internal cache. */
		void sample();

		/*Returns true iff the knob was pressed since last poll and clears the cached information.*/
		bool pressed();

		/* Returns information whether the knob moved since the last poll and clears internal cache.*/
		Rotation movement();

	};

	namespace raw {
		//Objects encapsulating raw knob objects

		inline std::array<Knob, 3> knobs{ {Knob{2},Knob{1},Knob{0}} };

		inline Knob& red = knobs[0];
		inline Knob& green = knobs[1];
		inline Knob& blue = knobs[2];
	}

	//Prefered method of accessing knobs is through a manager
	inline std::array<KnobManager, 3> knobs{ raw::red, raw::green, raw::blue };

	//As three individual variables
	inline KnobManager& red = knobs[0];
	inline KnobManager& green = knobs[1];
	inline KnobManager& blue = knobs[2];
}
