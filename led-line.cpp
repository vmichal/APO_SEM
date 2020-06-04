
#include "led-line.hpp"
#include "mzapo_phys.h"
#include "mzapo_regs.h"

#include <assert.h>
#include <algorithm>

namespace led {

	//Define global object from led-line.hpp
	LED_line line;


	LED_line::LED_line()
		:peripheral_{ (unsigned char*)map_phys_address(SPILED_REG_BASE_PHYS, SPILED_REG_SIZE, 0) } {

		assert(peripheral_);

		for (unsigned i = 0; i < leds_.size(); ++i) {
			leds_[i].index_ = i;
			leds_[i].mask_ = 1 << i;
		}

	}

	void LED_line::write(std::uint32_t binary_value) {

		std::for_each(leds_.begin(), leds_.end(), [binary_value](LED& led) {
			led = binary_value & led.mask_;
			});

		write_register(data_register, read());
	}

	std::uint32_t LED_line::read() const {
		std::uint32_t result = 0;
		for (unsigned i = 0; i < leds_.size(); ++i) {
			if (leds_[i]) {
				result |= leds_[i].mask_;
			}
		}
		return result;
	}

	void LED_line::display_scores_base_one(unsigned left, unsigned right) {
		assert(left >= 0 && right >= 0 && left < leds_.size() && right < leds_.size());

		std::uint32_t const left_base1 = -1u & ~((1u << (leds_.size() - left)) - 1);
		std::uint32_t const right_base1 = (1 << right) - 1;

		write(left_base1 | right_base1);
	}


	void LED_line::write_register(std::uintptr_t offset, std::uint32_t word) {
		*(std::uint32_t volatile*)(peripheral_ + offset) = word;
	}

}