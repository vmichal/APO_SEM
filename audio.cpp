

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

		if (playing_) {
			std::uint32_t const ns = 1'000'000'000 / hertz;
			assert(ns < (1 << 24));
			access_register<std::uint32_t>(peripheral_, period_reg_) = ns / 10;
		}
		frequency_ = hertz;
	}

	void Audio::strength(unsigned val) {

		if (playing_) {
			assert(val < (1 << 24));
			access_register<std::uint32_t>(peripheral_, strength_reg_) = val / 10;
		}

		strength_ = val;
	}
	void Audio::turn_on() {
		state_ = State::constant;
		if (playing())
			return;
		access_register<std::uint32_t>(peripheral_, strength_reg_) = strength_ / 10;
		playing_ = true;
	}

	void Audio::turn_off() {
		state_ = State::constant;
		if (!playing())
			return;
		access_register<std::uint32_t>(peripheral_, strength_reg_) = 0;
		playing_ = false;
	}

	void Audio::play_for(std::chrono::steady_clock::duration how_long) {
		constexpr int ns = 1'000'000, strength = 4000;
		turn_off();

		state_ = State::timed_sound;
		start_ = std::chrono::steady_clock::now();
		duration_ = how_long;

		access_register<std::uint32_t>(peripheral_, strength_reg_) = strength / 10;
		access_register<std::uint32_t>(peripheral_, period_reg_) = ns / 10;
	}

	void Audio::tick() {
		if (state_ == State::timed_sound && std::chrono::steady_clock::now() - start_ > duration_) {
			state_ = State::constant;
			access_register<std::uint32_t>(peripheral_, strength_reg_) = 0;
		}

	}


}

