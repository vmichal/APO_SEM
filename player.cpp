

#include "player.hpp"
#include "game.hpp"
#include <assert.h>
#include <random>
#include <queue>


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
		coord const size = my_game_.map().size();
		std::vector<std::vector<bool>> visited_matrix(size.y, std::vector<bool>(size.x, false));
		auto const visited = [&visited_matrix](coord c) {return visited_matrix[c.y][c.x]; };

		std::queue<std::pair<coord, Direction>> queue;
		queue.push({ my_game_.food(), Direction::east });
		visited(my_game_.food()) = true;

		while (!queue.empty()) {
			auto const [current, _] = queue.front();
			queue.pop();

			for (Direction const dir : {Direction::north, Direction::south, Direction::east, Direction::west}) {
				coord const neighbour = coord_clamp(current + displacement_in_direction(dir), size);

				if (neighbour == snake_->head()) {
					Direction const desired = opposite_direction(dir);
					printf("Reached head, desired direction is %s.\n", to_string(desired));
					if (snake_->current_direction_ == desired)
						return Action::none;
					else if (snake_->current_direction_ == turn_left(dir))
						return Action::turn_left;
					else if (snake_->current_direction_ == turn_right(dir))
						return Action::turn_right;
					else assert(false); //Cannot turn back
				}

				if (my_game_.get_square(neighbour).entity_ == Entity::none && !visited(neighbour)) {
					visited(neighbour) = true;
					queue.push({ neighbour, dir });
				}
			}
		}

		printf("BFS was unable to find a path leading to target.\n");
		return Action::none;
	}




}