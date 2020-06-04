#pragma once

#include "snake-options.hpp"
#include "mzapo_regs.h"
#include <array>
#include <type_traits>
#include <climits>

namespace led {

	enum class Color {
		black,
		red,
		green,
		blue,
		yellow,
		purple,
		brown,
		white
	};

	class RGB_LED {

		inline static unsigned char volatile * peripheral_ = nullptr;

		std::uintptr_t data_reg_ = 0;

	public:
		RGB_LED(std::uintptr_t data_reg);

		void write(unsigned red, unsigned green, unsigned blue);
		void write(std::uint32_t color);
		void write(Color c);

	};


	inline std::array<RGB_LED, 2> rgbs = { {SPILED_REG_LED_RGB1_o, SPILED_REG_LED_RGB2_o} };
	inline RGB_LED& rgb1 = rgbs.front();
	inline RGB_LED& rgb2 = rgbs.back();

}


