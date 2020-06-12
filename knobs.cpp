#include "knobs.hpp"
#include "mzapo_phys.h"
#include <assert.h>

namespace knobs {

	Knob::Knob(int index)
		: index_{ index } {
		if (!peripheral_) {
			peripheral_ = (unsigned char*)map_phys_address(SPILED_REG_BASE_PHYS, SPILED_REG_SIZE, 0);
		}
		assert(peripheral_);

	}

	bool Knob::pressed() const {

		std::uint32_t const value = access_register<std::uint32_t>(peripheral_, data_reg);

		return value & (1 << (24 + index_));

	}

	int Knob::angle() const {

		std::uint32_t const value = access_register<std::uint32_t>(peripheral_, data_reg);

		return (value >> 8 * index_) & 0xff;


	}

	void KnobManager::sample() {
		if (std::chrono::steady_clock::now() - last_poll_ < debounce_delay)
			return;
		pressed_ = pressed_ || knob_.pressed();

		int const current = knob_.angle();
		rotation_ += current - last_position_;
		last_position_ = current;
	}


	bool KnobManager::pressed() {
		if (std::chrono::steady_clock::now() - last_poll_ < debounce_delay)
			return false;

		bool const cached = pressed_;
		pressed_ = false;
		last_poll_ = std::chrono::steady_clock::now();
		return cached;
	}

	Rotation KnobManager::movement() {
		if (std::chrono::steady_clock::now() - last_poll_ < debounce_delay)
			return Rotation::none;

		Rotation const cached = rotation_ == 0
			? Rotation::none
			: rotation_ > 0 ? Rotation::clockwise : Rotation::counterclockwise;//TODO make sure this is correct

		rotation_ = 0;
		last_poll_ = std::chrono::steady_clock::now();
		return cached;
	}


}