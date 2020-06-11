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

	bool Knob::pressed() {

		std::uint32_t const value = access_register<std::uint32_t>(peripheral_, data_reg);

		return value & (1 << (24 + index_));

	}

	unsigned Knob::angle() const {

		std::uint32_t const value = access_register<std::uint32_t>(peripheral_, data_reg);

		return (value >> 8 * index_) & 0xff;


	}

}