#pragma once

#include "snake-options.hpp"
#include "mzapo_regs.h"
#include <array>
#include <type_traits>
#include <climits>

namespace led {


	struct LED {

		unsigned index_ = 0;
		unsigned mask_ = 0;
		bool on_ = false;

		void operator=(bool turn_on) {
			on_ = turn_on;
		}

		operator bool() const {
			return on_;
		}
	};

	class LED_line {
		static_assert(LED_line_length == sizeof(std::uint32_t) * CHAR_BIT, "Limited because of std::uint32_t's size.");

		std::uintptr_t data_register_;

		std::array<LED, LED_line_length> leds_;
		unsigned char volatile * const peripheral_;

	public:
		LED_line(std::uintptr_t reg);

		void write(std::uint32_t binary_value);
		std::uint32_t read() const;

		void display_scores_base_one(unsigned left, unsigned right);
	};

	inline LED_line line{ SPILED_REG_LED_LINE_o };

}

