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

namespace pwm {

	/* Wrapper around the AudioPWM peripheral. Exposes functions setting the period and volume of generated sound.*/
	class Audio {

		//Internal pointer to the peripheral's base. Offseted by reg_ fields
		inline static unsigned char volatile* peripheral_ = nullptr;
		std::uintptr_t const period_reg_;
		std::uintptr_t const strength_reg_;
		unsigned frequency_, strength_;

	public:
		Audio(std::uintptr_t period_reg, std::uintptr_t strength_reg);

		//TODO improve API, make the object statefull

		//Set and get frequency for PWM sound generation. Range [300..2000] sounds pretty good 
		void frequency(unsigned Hz);
		unsigned frequency() const { return frequency_; }
		//Probably something like duty cycle - higher number means stronger (value 4000 is pretty ok)
		void strength(unsigned val);
		unsigned strength() const { return strength_; }

	};

	//Global objects wrapping the peripheral. 
	inline Audio audio{ AUDIOPWM_REG_PWMPER_o, AUDIOPWM_REG_PWM_o };

}


