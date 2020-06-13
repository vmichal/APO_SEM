

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

	void Audio::frequency(unsigned hertz) {
		std::uint32_t const ns = 1'000'000'000 / hertz;
		assert(ns < (1 << 24));
		access_register<std::uint32_t>(peripheral_, period_reg_) = ns / 10;
		frequency_ = hertz;
	}

	void Audio::strength(unsigned val) {
		assert(val < (1 << 24));
		access_register<std::uint32_t>(peripheral_, strength_reg_) = val / 10;
		strength_ = val;
	}

	void Audio::on() {
		if (playing_)
			return;
		access_register<std::uint32_t>(peripheral_, strength_reg_) = strength_ / 10;
		playing_ = true;
	}

	void Audio::off() {
		if (!playing_)
			return;
		access_register<std::uint32_t>(peripheral_, strength_reg_) = 0;
		playing_ = false;
	}

}

