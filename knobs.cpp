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
		using namespace std::chrono_literals;
		if (std::chrono::steady_clock::now() - last_press_ > debounce_delay && knob_.pressed()) {
			pressed_ = true;
			last_press_ = std::chrono::steady_clock::now();
		}

		int const current = knob_.angle();
		rotation_ += current - last_position_;
		last_position_ = current;
	}


	bool KnobManager::pressed() {
		bool const cached = pressed_;
		pressed_ = false;
		return cached;
	}

	Rotation KnobManager::movement() {
		Rotation const cached = rotation_ == 0
			? Rotation::none
			: rotation_ > 0 ? Rotation::clockwise : Rotation::counterclockwise;//TODO make sure this is correct

		rotation_ = 0;
		return cached;
	}


}