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

		static constexpr std::uintptr_t data_register = SPILED_REG_LED_LINE_o;

		static_assert(LED_line_length == sizeof(std::uint32_t) * CHAR_BIT, "Limited because of std::uint32_t's size.");
		std::array<LED, LED_line_length> leds_;
		unsigned char volatile *  const peripheral_;

		void write_register(std::uintptr_t offset, std::uint32_t word);

	public:
		LED_line();

		void write(std::uint32_t binary_value);
		std::uint32_t read() const;

		void display_scores_base_one(unsigned left, unsigned right);
	};

	extern LED_line line;

}
