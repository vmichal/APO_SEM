
#include "game.hpp"
#include "snake-options.hpp"
#include "display.hpp"
#include "led-line.hpp"
#include"led-rgb.hpp"
#include <functional>
#include <algorithm>
#include <set>
#include <assert.h>
#include <cstdlib>

namespace game {

	static std::random_device random;
	static std::mt19937 generator(random());

	char const* to_string(Powerup p) {
		switch (p) {
		case Powerup::unknown: return "unknown";
		case Powerup::noclip: return "noclip";
		case Powerup::reset_food: return "reset_food";
		default: assert(false);
		}
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

	void Game::turn_off_peripherals() {
		led::line.write(0);
		led::rgb_left.write(led::Color::black);
		led::rgb_right.write(led::Color::black);
	}


	coord Game::find_empty_place() const {

		for (;;) {
			auto const [quot, rem] = std::div(distribution_(generator), map_.size().y);
			printf("Trying empty square at [%d, %d].\n", quot, rem);

			if (get_square({ quot, rem }).entity_ == Entity::none) {
				printf("OK - empty square found.\n");
				return { quot, rem };
			}
		}
	}

	void Game::draw_players() const {
		//Draw dead players as "background" -- they shall be overwritten by living players
		auto const partition = std::partition_point(players_.cbegin(), players_.cend(), std::mem_fn(&Player::dead));

		for (auto it = players_.cbegin(); it < partition; ++it) {
			auto const& player = *it; assert(player->dead());

			if (unsigned const diff = frame_ - death_times_.at(player->id()); diff < death_animation_frames) {
				short const color = diff % 2 ? game::colors::snakes[player->id()] : game::colors::bg;

				for (auto const [col, row] : player->snake()->segments_)
					fill_square_lcd(col, row, color);
			}
		}

		for (auto it = partition; it != players_.cend(); ++it) {
			auto const& player = *it; assert(!player->dead());

			Snake* const snk = player->snake();
			assert(snk);
			for (auto const [col, row] : snk->segments_)
				fill_square_lcd(col, row, game::colors::snakes[player->id()]);

			fill_square_lcd(snk->head().x, snk->head().y, game::colors::snakes[player->id()] / 2);
		}
	}

	void Game::draw_edible_stuff() const {
		fill_square_lcd(food_->position_.x, food_->position_.y, game::colors::food);

		//Draw information about powerup
		if (powerup_.exists_) {//TODO draw some continuous color scheme
			assert(powerup_.ptr_);
			fill_square_lcd(powerup_.ptr_->position_.x, powerup_.ptr_->position_.y, game::colors::snakes[generator() % game::colors::snakes.size()]);
			unsigned const powerup_die_time = powerup_lifetime + powerup_.start_frame_;
			std::uint32_t const writing = LED_line_length * (powerup_die_time - frame_) / powerup_lifetime;
			led::line.write_base_one(writing);
		}
		else
			led::line.write(0);

		led::rgbs.front().write(led::Color::black);
		led::rgbs.back().write(led::Color::black);

		for (auto const [p, data] : powerup_.collected_) {
			LocalPlayer const* player = dynamic_cast<LocalPlayer*>(p);
			if (player)	player->powerup_led_.write(powerup_colors.at(data.second));
		}
	}

	void Game::draw() const {
		if (state_ != State::running)
			return;

		flood_fill_lcd(game::colors::bg);

		map_.draw();

		draw_players();

		draw_edible_stuff();

		display_lcd();
	}

	void Game::update_edible_stuff() {
		if (powerup_.exists_ && frame_ - powerup_.start_frame_ > powerup_lifetime) {
			powerup_.exists_ = false;
			powerup_.ptr_->entity_ = Entity::none;
			powerup_.ptr_ = nullptr;
		}

		if (!powerup_.exists_ && generator() % powerup_random_coef == 0) {
			powerup_.exists_ = true;
			powerup_.start_frame_ = frame_;
			powerup_.ptr_ = &get_square(find_empty_place());
			powerup_.ptr_->entity_ = Entity::edible;
		}

		for (auto& [player, data] : powerup_.collected_) {
			if (data.second == Powerup::unknown && frame_ - data.first > powerup_selection_time) {
				data.second = powerups[generator() % powerups.size()];
				printf("Player %d received powerup %s.\n", player->id(), to_string(data.second));
			}
		}
	}

	void Game::check_collisions(std::unique_ptr<Player>& player, coord const new_head) {
		switch (get_square(new_head).entity_) {
		case Entity::edible: {

			printf("Edible stuff reached by player %d!\n", player->id());
			bool const plain_food = new_head == food_->position_;

			for (int i = plain_food ? 1 : score_gain_powerup; i; --i)
				player->snake()->append_segment(player->snake()->segments_.back());

			if (plain_food) { //Food has been feasted
				food_ = &get_square(find_empty_place()); //Generate new food
				food_->entity_ = Entity::edible;
			}
			else { //Player found a powerup
				powerup_.exists_ = false;
				powerup_.ptr_ = nullptr;
				powerup_.collected_[player.get()] = std::make_pair(frame_, Powerup::unknown);
			}
		}
		case Entity::none: //Inform the square about snakes presence
			get_square(new_head).entity_ = Entity::snake;
			break;
		case Entity::wall: case Entity::snake: {
			printf("Boom by player %d into a %s.\n", player->id_, get_square(new_head).entity_ == Entity::snake ? "snake" : "wall");
			auto const partition = std::partition_point(players_.begin(), players_.end(), std::mem_fn(&Player::dead));

			player->die();
			death_times_[player->id()] = frame_;

			//All dead players must be kept before alive players for saner drawing
			std::iter_swap(partition, std::find(players_.begin(), players_.end(), player));
			break;
		}
		default: assert(false);
		}
	}

	void Game::use_powerup(Player* const player) {
		if (powerup_.collected_.count(player) == 0) {
			printf("This player has no powerup!");
			invalid_action_notification();
			return;
		}

		Powerup const powerup = powerup_.collected_.at(player).second;
		if (powerup == Powerup::unknown) {
			printf("Cannot use not yet selected powerup!");
			invalid_action_notification();
			return;
		}
		printf("Player %d activates powerup %s.\n", player->id(), to_string(powerup));
		powerup_.collected_.erase(player);

		switch (powerup) {
		case Powerup::noclip:
			//TODO implement
			printf("Activating noclip for player %d.\n", player->id());
			break;
		case Powerup::reset_food:
			food_->entity_ = Entity::none;
			food_ = &get_square(find_empty_place());
			food_->entity_ = Entity::edible;
			printf("Successfully reset food.\n");
			break;
		default: assert(false);
		}
	}

	void Game::update() {
		if (state_ != State::running)
			return;

		++frame_;

		printf("Game::update()\n");
		for (auto& player : players_) {
			printf("Player %d %s.\n", player->id(), player->dead_ ? "dead" : "alive");
			if (player->dead_)
				continue;
			assert(player->snake());
			Snake& snk = *player->snake();
			Player::Action const act = player->get_action();
			if (act == Player::Action::use_powerup)
				use_powerup(player.get());
			snk.turn(act);

			coord const old_tail = snk.tail();
			coord const new_head = coord_clamp(snk.get_new_head(), map_.size());

			printf("New head [%d, %d].\n", new_head.x, new_head.y);

			check_collisions(player, new_head);

			snk.segments_.push_front(new_head);
			snk.segments_.pop_back(); //Remove old tail
			if (std::find(snk.segments_.begin(), snk.segments_.end(), old_tail) == snk.segments_.end()) {
				//We have to account for multiple snake extensions (the same square can be in the snake multiple times)
				get_square(old_tail).entity_ = Entity::none;
			}
		}

		update_edible_stuff();

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

			if (local_players == 0)
				players_.emplace_back(std::make_unique<LocalPlayer>(new_id, *this, knobs::red, led::rgb_left));
			else
				players_.emplace_back(std::make_unique<LocalPlayer>(new_id, *this, knobs::blue, led::rgb_right));
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
		turn_off_peripherals();
		printf("Pausing running game.\n");
	}

	void Game::start() {
		assert(state_ == State::initialization);
		assert(players_.size() <= map_.starting_positions().size());

		unsigned const player_count = players_.size();
		std::vector<coord> starts = map_.starting_positions();
		assert(starts.size() >= player_count);
		std::shuffle(starts.begin(), starts.end(), generator);

		for (auto& player : players_) {
			player->dead_ = false;

			coord const start = starts[player->id()];
			get_square(start).entity_ = Entity::snake;

			player->reset_snake();

			std::fill_n(std::front_inserter(player->snake()->segments_), snake_start_length, start);

			death_times_[player->id()] = 0; // Zero out all death times
		}

		food_ = &get_square(find_empty_place());
		food_->entity_ = Entity::edible;

		state_ = State::running;
		printf("Starting game.\n");
	}

	Square& Game::get_square(coord pos) {
		assert(pos.y >= 0 && pos.y < map_.size().y);
		assert(pos.x >= 0 && pos.x < map_.size().x);

		return map_.board()[pos.y][pos.x];
	}

	Square const& Game::get_square(coord pos) const {
		assert(pos.y >= 0 && pos.y < map_.size().y);
		assert(pos.x >= 0 && pos.x < map_.size().x);

		return map_.board()[pos.y][pos.x];
	}
}

