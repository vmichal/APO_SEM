
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
		write(led::Color::black);
	}

	void RGB_LED::write(unsigned red, unsigned green, unsigned blue) {
		assert(red < 256 && green < 256 && blue < 256);

		unsigned const new_value = red << 16 | green << 8 | blue;

		access_register<std::uint32_t>(peripheral_, data_reg_) = new_value;
	}

	void RGB_LED::write(std::uint32_t const color) {
		write((color >> 16) & 0xff, (color >> 8) & 0xff, color & 0xff);
	}

	void RGB_LED::write(Color const c) {
		switch (c) {
		case Color::black:  return write(0, 0, 0);
		case Color::red:    return write(255, 0, 0);
		case Color::green:  return write(0, 255, 0);
		case Color::blue:   return write(0, 0, 255);
		case Color::white:  return write(255, 255, 255);

		case Color::brown:  return write(255, 255, 0);
		case Color::purple: return write(255, 0, 255);
		case Color::yellow: return write(0, 255, 255);
		}
	}



}

