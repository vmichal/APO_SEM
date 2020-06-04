
#include "led-rgb.hpp"
#include "mzapo_phys.h"

#include <assert.h>

namespace led {

	RGB_LED::RGB_LED(std::uintptr_t data_reg)
		:data_reg_{ data_reg } {

		if (!peripheral_) {
			peripheral_ = (unsigned char*)map_phys_address(SPILED_REG_BASE_PHYS, SPILED_REG_SIZE, 0);
		}
		assert(peripheral_);
	}

	void RGB_LED::write(unsigned red, unsigned green, unsigned blue) {
		assert(red < 256 && green < 256 && blue < 256);

		unsigned const new_value = red << 16 | green << 8 | blue;

		access_register<std::uint32_t>(peripheral_, data_reg_) = new_value;
	}


}