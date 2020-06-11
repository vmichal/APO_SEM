

#include "audio.hpp"
#include "mzapo_phys.h"
#include <assert.h>


namespace pwm {

	Audio::Audio(std::uintptr_t period_reg, std::uintptr_t strength_reg)
		: period_reg_{ period_reg }, strength_reg_{ strength_reg } {
		if (!peripheral_) {
			peripheral_ = (unsigned char*)map_phys_address(AUDIOPWM_REG_BASE_PHYS, AUDIOPWM_REG_SIZE, 0);
		}
		
		assert(peripheral_);
	}

	void Audio::set_period(std::uint32_t ns) {
		assert(ns < (1 <<24));
		access_register<std::uint32_t>(peripheral_, period_reg_) = ns / 10;
	}

	void Audio::set_strength(std::uint32_t val) {
		assert(val < (1 <<24));
		access_register<std::uint32_t>(peripheral_, strength_reg_) = val / 10;
	}

}
