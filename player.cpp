
#include "player.hpp"
#include "game.hpp"
#include <assert.h>
#include <random>


namespace game {


	Action LocalPlayer::get_action() {

		if (controller_.pressed())
			return Action::use_powerup;

		switch (controller_.movement()) {
		case knobs::Rotation::none:				return Action::none;
		case knobs::Rotation::counterclockwise:	return Action::turn_left;
		case knobs::Rotation::clockwise:		return Action::turn_right;
		}
		assert(false);

	}

	Action AutonomousPlayer::get_action() {
		//TODO find out how to use powerups
		auto const& board = my_game_.board();

		std::uniform_int_distribution<int> random(0, 1);

		return random() ? Action::turn_left : Action::turn_right;

	}




}