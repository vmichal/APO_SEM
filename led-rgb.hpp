#pragma once

#include "snake-options.hpp"
#include "mzapo_regs.h"
#include <array>
#include <type_traits>
#include <climits>

namespace led {


	class RGB_LED {

		inline static unsigned char volatile * peripheral_ = nullptr;

		std::uintptr_t data_reg_ = 0;

	public:
		RGB_LED(std::uintptr_t data_reg);

		void write(unsigned red, unsigned green, unsigned blue);

	};

	extern std::array<RGB_LED, 2> rgbs;

}
