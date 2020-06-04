#pragma once

#include "snake-options.hpp"
#include "mzapo_regs.h"
#include <array>
#include <type_traits>
#include <climits>

class Knob {

	constexpr static std::uintptr_t data_reg = SPILED_REG_KNOBS_8BIT_o;

	int index_;
	unsigned char volatile* const peripheral_;

public:
	Knob(int index);

	bool pressed();

	unsigned angle() const;

};

inline std::array<Knob, 3> knobs{ {2,1,0} };

inline Knob& knob_red = knobs[0];
inline Knob& knob_green = knobs[1];
inline Knob& knob_blue = knobs[2];

