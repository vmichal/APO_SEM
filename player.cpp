

#include "player.hpp"
#include "game.hpp"
#include <assert.h>
#include <random>


namespace game {

	int Player::score() const {
		if (dead_)
			return 16; //TODO indicate death
		return snake_->segments_.size() - snake_start_length;
	}

	void Player::die() {
		dead_ = true;
		for (coord const segment : snake_->segments_)
			my_game_.get_square(segment).entity_ = Entity::none;
		snake_.reset();
		//TODO do something more to indicate death
	}

	void Player::reset_snake() {
		snake_ = std::make_unique<Snake>();
	}


	Player::Action LocalPlayer::get_action() {

		if (controller_.pressed())
			return Action::use_powerup;

		switch (controller_.movement()) {
		case knobs::Rotation::none:				return Action::none;
		case knobs::Rotation::counterclockwise:	return Action::turn_left;
		case knobs::Rotation::clockwise:		return Action::turn_right;
		}
		assert(false);

	}

	Player::Action RemotePlayer::get_action() {
		return Action::none;
	}

	Player::Action AutonomousPlayer::get_action() {
		//TODO find out how to use powerups
		auto const& board = my_game_.board();

		std::random_device random;
		std::mt19937 gen(random());
		std::uniform_int_distribution<int> distribution(0, 11);

		int const r = distribution(gen);
		if (r < 10)
			return Action::none;
		return r == 10 ? Action::turn_left : Action::turn_right;

	}




}