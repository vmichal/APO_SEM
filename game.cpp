
#include "game.hpp"
#include "snake-options.hpp"
#include "display.hpp"
#include <functional>
#include <algorithm>
#include <set>
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

	void Snake::move() {

		segments_.push_front(get_new_head());

		last_popped_ = segments_.back();
		segments_.pop_back();
	}

	coord Game::generate_food() const {
		static std::mt19937 random(420);
		coord food;

		do {
			food = coord{ distribution_(random), distribution_(random) };
		} while (get_square(food).entity_ != Entity::none);

		return food;
	}


	Game::Game(int width, int height)
		:game_board_(height, std::vector<Square>(height)), last_frame_{ std::chrono::steady_clock::now() },
		distribution_{ 0, width * height - 1 } {
		assert(width > 0 && height > 0);

		for (int row = 0; row < height; ++row) {

			std::generate_n(game_board_[row].begin(), width, [row, col = 0]() mutable {
				return Square{ col++, row, Entity::none };
			});
		}

	}


	void Game::draw() {

		flood_fill_lcd(game_bg_color);

		for (auto const& snake : snakes_) {
			for (auto const [col, row] : snake->segments_)
				fill_square_lcd(col, row, snake_colors[snake->id_]);
		}

		display_lcd();
	}

	void Game::update() {

		if (state_ != State::running)
			return;


		for (auto& snake : snakes_) {
			snake->move();
			if (std::find(snake->segments_.begin(), snake->segments_.end(), snake->last_popped_) == snake->segments_.end()) {
				//We have to account for multiple snake extensions (the same square can be in the snake multiple times)
				get_square(snake->last_popped_).entity_ = Entity::none;
			}
		}

		std::set<int> dead;

		for (auto& snake : snakes_)
			switch (get_square(snake->head()).entity_) {
			case Entity::food:
				snake->add_segment(snake->segments_.back());

				//Generate new food
				food_->entity_ = Entity::snake;
				food_ = &get_square(generate_food());
				food_->entity_ = Entity::food;
				break;
			case Entity::wall:
				printf("Boom by player %d into a wall.\n", snake->id_);
				dead.insert(snake->id_);
				pause();
				break;
			case Entity::snake:
				printf("Boom by player %d into a wall.\n", snake->id_);
				dead.insert(snake->id_);
				pause();
				break;
			}

		snakes_.erase(std::remove_if(snakes_.begin(), snakes_.end(), [&](std::unique_ptr<Snake>& ptr) {
			return dead.count(ptr->id_);
			}), snakes_.end());

		if (snakes_.size() <= 1) {
			printf("Game ended, player count is %d.\n", snakes_.size());
		}

		last_frame_ = std::chrono::steady_clock::now();
	}

	void Game::add_player(std::unique_ptr<Player> player) {
		//Sorry, you cannot add players after the game has started
		assert(state_ == State::initialization);

		snakes_.emplace_back(std::make_unique<Snake>(snakes_.size(), std::move(player)));
	}

	void Game::resume() {
		assert(state_ == State::paused);
		state_ = State::running;
		printf("Resuming paused game.\n");
	}

	void Game::pause() {
		assert(state_ == State::running);
		state_ = State::paused;
		printf("Pausing running game.\n");
	}

	void Game::start() {
		assert(state_ == State::initialization);

		int const player_count = snakes_.size();
		int const step = COLUMNS / (player_count + 1);

		for (auto& s : snakes_) {
			coord const start = { step * (s->id_ + 1), ROWS / 2 };
			get_square(start).entity_ = Entity::snake;

			std::fill_n(std::front_inserter(s->segments_), snake_start_length, start);
		}

		food_ = &get_square(generate_food());
		food_->entity_ = Entity::food;

		state_ = State::paused;
		printf("Starting game.\n");
	}

	Square& Game::get_square(coord pos) {
		assert(pos.y >= 0 && pos.y < (int)game_board_.size());
		assert(pos.x >= 0 && pos.x < (int)game_board_[0].size());

		return game_board_[pos.y][pos.x];
	}

	Square const& Game::get_square(coord pos) const {
		assert(pos.y >= 0 && pos.y < (int)game_board_.size());
		assert(pos.x >= 0 && pos.x < (int)game_board_[0].size());

		return game_board_[pos.y][pos.x];
	}
}

