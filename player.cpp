

#include "player.hpp"
#include "game.hpp"
#include <assert.h>
#include <random>
#include <queue>


namespace game {

	std::vector<std::vector<AutonomousPlayer::SquareData>> AutonomousPlayer::to_food_;
	std::vector<std::vector<AutonomousPlayer::SquareData>> AutonomousPlayer::to_powerup_;
	bool AutonomousPlayer::powerup_lives_ = true;

	int Player::score() const {
		return snake_->segments_.size() - snake_start_length;
	}

	void Player::die() {
		dead_ = true;
		for (coord const segment : snake_->segments_)
			my_game_.get_square(segment).entity_ = Entity::none;
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

	void AutonomousPlayer::bfs(Map const& map, coord const start, std::vector<std::vector<SquareData>>& target_matrix) {
		coord const size = map.size();
		std::vector<std::vector<int>> distances_matrix(size.y, std::vector<int>(size.x, -1));
		auto const distances = [&distances_matrix](coord c)->int& {return distances_matrix[c.y][c.x]; };
		auto const result = [&target_matrix](coord c) {return target_matrix[c.y][c.x]; };

		std::queue<std::pair<coord, Direction>> queue;
		queue.push({ start, Direction::unknown });
		distances(start) = 0;

		while (!queue.empty()) {
			auto const [current, came_from] = queue.front();
			queue.pop();

			int const distance = distances(current);
			result(current) = SquareData{ distance, came_from, true };

			if (auto const ent = map.board()[current.y][current.x].entity_; ent == Entity::wall || ent == Entity::snake)
				continue;

			for (Direction const dir : {Direction::north, Direction::south, Direction::east, Direction::west}) {
				coord const neighbour = coord_clamp(current + displacement_in_direction(dir), size);

				if (distances(neighbour) == -1) {
					distances(neighbour) = distance + 1;
					queue.push({ neighbour, opposite_direction(dir) });
				}
			}
		}
	}

	Player::Action AutonomousPlayer::get_action() {


		//TODO find out how to use powerups
		coord const snake_head = snake_->head();
		SquareData const data = to_food_[snake_head.y][snake_head.x];

		Direction desired = data.desired_dir;
		if (desired == Direction::unknown) {
			desired = snake_->current_direction_; //TODO try fallback strategy
			printf("Could not find path to food.\n")
		}

		if (snake_->current_direction_ == desired)
			return Action::none;
		else if (snake_->current_direction_ == turn_left(desired))
			return Action::turn_right;
		else if (snake_->current_direction_ == turn_right(desired))
			return Action::turn_left;
		else assert(false); //Cannot turn back


	}

	void AutonomousPlayer::learn_map(Game const& game) {
		auto const [width, height] = game.map().size();

		to_food_.resize(height);
		for (auto& line : to_food_)
			line.resize(width);

		to_powerup_.resize(height);
		for (auto& line : to_powerup_)
			line.resize(width);

	}

	void AutonomousPlayer::consider_actions(Game const& game) {
		assert(to_food_.size() == game.map().board().size());
		assert(to_powerup_.size() == game.map().board().size());

		for (auto& line : to_food_)
			for (auto& square : line)
				square.reachable = false;

		for (auto& line : to_powerup_)
			for (auto& square : line)
				square.reachable = false;


		bfs(game.map(), game.food_->position_, to_food_);
		printf("Searching for paths to food.\n");

		powerup_lives_ = game.powerup_.exists_;
		if (powerup_lives_) {
			printf("Searching for paths to powerup.\n");
			bfs(game.map(), game.powerup_.ptr_->position_, to_powerup_);
		}
	}



}