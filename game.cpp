
#include "game.hpp"
#include <functional>
#include <algorithm>
#include <assert.h>

namespace {

	game::coord displacement_in_direction(game::Direction dir) {
		switch (dir) {
		case game::Direction::down: return { 0,-1 };
		case game::Direction::up: return { 0,1 };
		case game::Direction::right: return { 1,0 };
		case game::Direction::left: return { -1,0 };
		}
		assert(false);
	}

	game::Direction turn_left(game::Direction dir) {
		switch (dir) {
		case game::Direction::down: return game::Direction::right;
		case game::Direction::up: return game::Direction::left;
		case game::Direction::right: return game::Direction::up;
		case game::Direction::left: return game::Direction::down;
		}
		assert(false);
	}

	game::Direction turn_right(game::Direction dir) {
		switch (dir) {
		case game::Direction::down: return game::Direction::left;
		case game::Direction::up: return game::Direction::right;
		case game::Direction::right: return game::Direction::down;
		case game::Direction::left: return game::Direction::up;
		}
		assert(false);
	}
}

namespace game {

	coord Snake::get_new_head() const {
		coord const current = head();

		switch (player_->get_action()) {
		case Player::Action::none: case Player::Action::use_powerup:
			return current + displacement_in_direction(current_direction_);
		case Player::Action::turn_left:
			return current + displacement_in_direction(turn_left(current_direction_));
		case Player::Action::turn_right:
			return current + displacement_in_direction(turn_right(current_direction_));

		}
		assert(false);
	}

	void Snake::update() {

		coord const new_head = get_new_head();
		segments_.push(new_head);

		last_popped_ = segments_.front();
		segments_.pop();
	}

	Game::Game(int width, int height) {
		assert(width > 0 && height > 0);

		game_board_.resize(height);
		for (int row = 0; row < height; ++row) {
			game_board_[row].resize(width);

			std::generate_n(game_board_[row].begin(), width, [row, col = 0]() mutable {
				return Square{ col++, row, Entity::none };
			});
		}

	}


	void Game::draw() {




	}

	void Game::update() {

		for (auto &snake : snakes_)
			snake.update();

		last_frame_ = std::chrono::steady_clock::now();
	}

	Square const* Game::get_square(coord pos) const {
		assert(pos.y >= 0 && pos.y < (int)game_board_.size());
		assert(pos.x >= 0 && pos.x < (int)game_board_[0].size());

		return &game_board_[pos.y][pos.x];
	}
}

