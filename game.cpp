
#include "game.hpp"
#include <functional>
#include <algorithm>
#include <assert.h>

namespace {

	game::coord displacement_in_direction(game::Direction dir) {
		switch (dir) {
		case game::Direction::south: return { 0,-1 };
		case game::Direction::north: return { 0,1 };
		case game::Direction::east: return { 1,0 };
		case game::Direction::west: return { -1,0 };
		}
		assert(false);
	}

	game::Direction turn_left(game::Direction dir) {
		switch (dir) {
		case game::Direction::south: return game::Direction::east;
		case game::Direction::north: return game::Direction::west;
		case game::Direction::east: return game::Direction::north;
		case game::Direction::west: return game::Direction::south;
		}
		assert(false);
	}

	game::Direction turn_right(game::Direction dir) {
		switch (dir) {
		case game::Direction::south: return game::Direction::west;
		case game::Direction::north: return game::Direction::east;
		case game::Direction::east: return game::Direction::south;
		case game::Direction::west: return game::Direction::north;
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

	void Game::add_player(std::unique_ptr<Player> player) {
		snakes_.emplace_back(std::move(player));
	}

	Square const* Game::get_square(coord pos) const {
		assert(pos.y >= 0 && pos.y < (int)game_board_.size());
		assert(pos.x >= 0 && pos.x < (int)game_board_[0].size());

		return &game_board_[pos.y][pos.x];
	}
}

