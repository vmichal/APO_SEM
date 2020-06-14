#pragma once

/*
	Computer architectures semestral assignment - Zelinka Josef & Michal Vojtìch

	This file contains interface to the connected PWM sound generator.
*/

#include "snake-options.hpp"
#include "mzapo_regs.h"
#include <array>
#include <type_traits>
#include <climits>
#include <chrono>

namespace pwm {

	/* Wrapper around the AudioPWM peripheral. Exposes functions setting the period and volume of generated sound.*/
	class Audio {

		enum class State {
			timed_sound,
			constant
		} state_ = State::constant;

		//Internal pointer to the peripheral's base. Offseted by reg_ fields
		inline static unsigned char volatile* peripheral_ = nullptr;
		std::uintptr_t const period_reg_;
		std::uintptr_t const strength_reg_;
		unsigned frequency_ = 440, strength_ = 4000;
		bool playing_ = false;
		std::chrono::steady_clock::time_point start_;
		std::chrono::steady_clock::duration duration_;

	public:
		Audio(std::uintptr_t period_reg, std::uintptr_t strength_reg);

		//TODO improve API, make the object statefull

		//Set and get frequency for PWM sound generation. Range [300..2000] sounds pretty good 
		void frequency(unsigned Hz);
		unsigned frequency() const { return frequency_; }
		//Probably something like duty cycle - higher number means stronger (value 4000 is pretty ok)
		void strength(unsigned val);
		unsigned strength() const { return strength_; }

		/* Turn the music on using last used frequency and strength*/
		void turn_on();

		/* Turn the music off (preserves configuration).*/
		void turn_off();

		/* Returns true iff the device is currently making sound. */
		bool playing() const { return playing_; }
		/* Turn the sound generator on or off depending on the argument. */
		void playing(bool play) { if (play) turn_on(); else turn_off(); }

		void play_for(std::chrono::steady_clock::duration how_long);

		void tick();
	};

	//Global objects wrapping the peripheral. 
	inline Audio audio{ AUDIOPWM_REG_PWMPER_o, AUDIOPWM_REG_PWM_o };

}


