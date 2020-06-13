
#include "game.hpp"
#include "snake-options.hpp"
#include "display.hpp"
#include "led-line.hpp"
#include <functional>
#include <algorithm>
#include <set>
#include <assert.h>
#include <cstdlib>

namespace game {

	coord displacement_in_direction(Direction dir) {
		switch (dir) {
		case Direction::south: return { 0,1 };
		case Direction::north: return { 0,-1 };
		case Direction::east: return { 1,0 };
		case Direction::west: return { -1,0 };
		}
		assert(false);
	}

	Direction turn_left(Direction dir) {
		switch (dir) {
		case Direction::south: return Direction::east;
		case Direction::north: return Direction::west;
		case Direction::east: return Direction::north;
		case Direction::west: return Direction::south;
		}
		assert(false);
	}

	Direction turn_right(Direction dir) {
		switch (dir) {
		case Direction::south: return Direction::west;
		case Direction::north: return Direction::east;
		case Direction::east: return Direction::south;
		case Direction::west: return Direction::north;
		}
		assert(false);
	}
	Direction opposite_direction(Direction dir) {
		switch (dir) {
		case Direction::south: return Direction::north;
		case Direction::north: return Direction::south;
		case Direction::east: return Direction::west;
		case Direction::west: return Direction::east;
		}
		assert(false);
	}

	char const*  to_string(Direction dir) {
		switch (dir) {
		case Direction::south: return "S";
		case Direction::north: return "N";
		case Direction::east: return "W";
		case Direction::west: return "E";
		}
		assert(false);
	}



	coord Snake::get_new_head() const {
		return head() + displacement_in_direction(current_direction_);
	}

	void Snake::turn(Player::Action action) {
		switch (action) {
		case Player::Action::none: case Player::Action::use_powerup:
			printf("Direction unchanged.\n");
			break;
		case Player::Action::turn_left:
			current_direction_ = turn_left(current_direction_);
			printf("Turning left.\n");
			break;
		case Player::Action::turn_right:
			current_direction_ = turn_right(current_direction_);
			printf("Turning right.\n");
			break;
		default:
			assert(false);
		}
	}


	coord Game::generate_food() const {
		static std::random_device random;
		static std::mt19937 generator(random());
		coord food;

		do {
			auto const [quot, rem] = std::div(distribution_(generator), size_.y);
			printf("Tyring food at [%d, %d].\n", quot, rem);
			food = coord{ quot, rem };
		} while (get_square(food).entity_ != Entity::none);
		printf("OK - food found.\n");

		return food;
	}


	Game::Game(int width, int height)
		:size_{ width, height }, game_board_(height, std::vector<Square>(width)),
		last_frame_{ std::chrono::steady_clock::now() }, distribution_{ 0, width * height - 1 } {
		assert(width > 0 && height > 0);

		for (int row = 0; row < height; ++row) {

			std::generate_n(game_board_[row].begin(), width, [row, col = 0]() mutable {
				return Square{ col++, row, Entity::none };
			});
		}

	}


	void Game::draw() {
		if (state_ != State::running)
			return;

		flood_fill_lcd(game::colors::bg);

		for (auto const& player : players_) {
			if (player->dead_)
				continue;

			Snake* const snk = player->snake();
			assert(snk);
			for (auto const [col, row] : snk->segments_)
				fill_square_lcd(col, row, game::colors::snakes[player->id()]);
		}

		fill_square_lcd(food_->position_.x, food_->position_.y, game::colors::food);

		display_lcd();
		led::line.display_scores_base_one(players_.front()->score(), players_.back()->score());
	}

	void Game::update() {

		if (state_ != State::running)
			return;

		//TODO Store old tails to increase redraw speed
		printf("Game::update()\n");
		for (auto& player : players_) {
			printf("Player %d %s.\n", player->id(), player->dead_ ? "dead" : "alive");
			if (player->dead_)
				continue;
			assert(player->snake());
			Snake& snk = *player->snake();
			snk.turn(player->get_action());

			coord const old_tail = snk.tail();
			coord const new_head = coord_clamp(snk.get_new_head(), size_);

			printf("New head [%d, %d].\n", new_head.x, new_head.y);

			switch (get_square(new_head).entity_) {
			case Entity::food:
				printf("Food eaten by player %d!\n", player->id());
				player->snake()->append_segment(player->snake()->segments_.back());

				//Generate new food
				food_->entity_ = Entity::snake;
				food_ = &get_square(generate_food());
				food_->entity_ = Entity::food;
				break;
			case Entity::wall:
				printf("Boom by player %d into a wall.\n", player->id_);
				player->die();
				break;
			case Entity::snake:
				printf("Boom by player %d into another snake.\n", player->id_);
				player->die();
				break;
			}

			snk.segments_.push_front(new_head);
			snk.segments_.pop_back(); //Remove old tail
			if (std::find(snk.segments_.begin(), snk.segments_.end(), old_tail) == snk.segments_.end()) {
				//We have to account for multiple snake extensions (the same square can be in the snake multiple times)
				get_square(old_tail).entity_ = Entity::none;
			}
			get_square(new_head).entity_ = Entity::snake;
		}

		last_frame_ = std::chrono::steady_clock::now();
	}

	void Game::add_player(Player::Type player_type) {
		//Sorry, you cannot add players after the game has started
		assert(state_ == State::initialization);

		int const new_id = players_.size();

		const char* debug_info = nullptr;
		switch (player_type) {
		case Player::Type::local: {
			int const local_players = std::count_if(players_.begin(), players_.end(), [](auto const& player) {
				return player->type_ == Player::Type::local;
				});
			assert(local_players < 2); //Sorry, cannot have more than two players locally

			players_.emplace_back(std::make_unique<LocalPlayer>(new_id, *this, local_players == 0 ? knobs::red : knobs::blue));
			debug_info = "local";
			break;
		}
		case Player::Type::autonomous:
			players_.emplace_back(std::make_unique<AutonomousPlayer>(new_id, *this));
			debug_info = "autonomous";
			break;
		case Player::Type::remote:
			players_.emplace_back(std::make_unique<RemotePlayer>(new_id, *this));
			debug_info = "remote";
			break;
		default:
			assert(false);
		}
		printf("Adding %s player %d.\n", debug_info, new_id);
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

		int const player_count = players_.size();
		int const step = size_.x / (player_count + 1);

		for (auto& player : players_) {
			player->dead_ = false;

			coord const start = { step * (player->id_ + 1), size_.y / 2 };
			get_square(start).entity_ = Entity::snake;

			player->reset_snake();

			std::fill_n(std::front_inserter(player->snake()->segments_), snake_start_length, start);
		}

		food_ = &get_square(generate_food());
		food_->entity_ = Entity::food;

		state_ = State::running;
		printf("Starting game.\n");
	}

	Square& Game::get_square(coord pos) {
		assert(pos.y >= 0 && pos.y < size_.y);
		assert(pos.x >= 0 && pos.x < size_.x);

		return game_board_[pos.y][pos.x];
	}

	Square const& Game::get_square(coord pos) const {
		assert(pos.y >= 0 && pos.y < size_.y);
		assert(pos.x >= 0 && pos.x < size_.x);

		return game_board_[pos.y][pos.x];
	}
}

