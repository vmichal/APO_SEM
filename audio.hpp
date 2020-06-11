#pragma once

/*
	Computer architectures semestral assignment - Zelinka Josef & Michal Vojt�ch

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


	public:
		Audio(std::uintptr_t period_reg, std::uintptr_t strength_reg);

		//TODO improve API, make the object statefull

		//Use given number of nanoseconds as PWM period (around 10ms period is ok ... 200 Hz)
		void set_period(std::uint32_t ns);
		//Probably something like duty cycle - higher number means stronger (value 4000 is pretty ok)
		void set_strength(std::uint32_t val);

	};

	//Global objects wrapping the peripheral. 
	inline Audio audio{AUDIOPWM_REG_PWMPER_o, AUDIOPWM_REG_PWM_o};

}

