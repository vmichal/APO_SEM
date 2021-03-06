#pragma once 
/*
	Computer architectures semestral assignment - Zelinka Josef & Michal Vojtech

	This file contains most of the gameplay mechanics.

	The Game type encapsulates an entire running game. It is aware of its players, map,
	controls the timing (to make framerate as consistent as possible), indirectly handles
	collision detection, drawing and updating the state of the game.

	Snake is just a queue of game board atoms. As it grows, the controlling player's score increases.
*/

#include <vector>
#include <deque>
#include <chrono>
#include <memory>
#include <random>
#include <algorithm>
#include <functional>
#include <map>

#include "audio.hpp"
#include "player.hpp"
#include "map.hpp"
#include "snake-options.hpp"


namespace game {

	enum class Powerup {
		unknown,
		reset_food,
		freeze_time
	};
	char const* to_string(Powerup);

	constexpr std::array<Powerup, 2> powerups{ {Powerup::freeze_time, Powerup::reset_food} };

	std::map<Powerup, led::Color> const  powerup_colors{
		{Powerup::unknown, led::Color::white},
		{Powerup::reset_food, led::Color::red},
		{Powerup::freeze_time, led::Color::green}
	};


	struct Snake {
		Direction current_direction_ = Direction::north;
		std::deque<coord> segments_;

		coord tail() const { return segments_.back(); }
		coord head() const { return segments_.front(); }
		void append_segment(coord new_head) { segments_.push_back(new_head); }

		coord get_new_head() const;
		void turn(Player::Action action);

	};

	struct PowerupWrapper {
		Square* ptr_;
		bool exists_;
		unsigned remaining_time_;
		std::map<Player*, std::pair<int, Powerup>> collected_;
	};

	struct FreezeData {
		bool on_;
		int caster_id_;
		unsigned remaining_time_;
	};


	class Game {
		friend class AutonomousPlayer;
	public:
		enum class State {

			initialization,
			paused,
			ended,
			running
		};
	private:
		State state_ = State::initialization;

		Map& map_;
		std::vector<std::unique_ptr<Player>> players_;
		std::chrono::steady_clock::time_point last_frame_ = std::chrono::steady_clock::now();
		mutable std::uniform_int_distribution<int> distribution_;
		std::map<int, unsigned> death_times_;
		unsigned frame_ = 0;
		unsigned fps_ = 100;

		bool has_AI_players_ = false;

		Square* food_ = nullptr;
		PowerupWrapper powerup_{ nullptr, false, 0 };
		FreezeData freeze_data_{ false, 0,0 };

		coord find_empty_place() const;

		static void turn_off_peripherals();

		void draw_players() const;
		void draw_edible_stuff() const;

		void update_powerups();
		void check_collisions(std::unique_ptr<Player>& player, coord new_head);
		void use_powerup(Player* player);

		void invalid_action_notification() const {
			pwm::audio.play_for(200ms);
		}

	public:
		Game(Map& map) : map_{ map }, distribution_{ 0, map.size().x * map.size().y - 1 } {	}
		~Game() { turn_off_peripherals(); map_.clear(); }

		bool all_dead() const { return std::all_of(players_.begin(), players_.end(), std::mem_fn(&Player::dead)); }
		std::vector<std::unique_ptr<Player>> const& players() const { return players_; }

		Map& map() const { return map_; }
		coord food() const { return food_->position_; }
		bool frame_elapsed() const { return std::chrono::steady_clock::now() - last_frame_ > std::chrono::microseconds{ 1'000'000 / fps_ }; };
		State state() const { return state_; }
		std::map<Player*, std::pair<int, Powerup>> const& powerup_info() const { return powerup_.collected_; }

		bool has_AI_players() const { return has_AI_players_; }

		void update();

		void draw() const;

		void add_player(Player::Type player_type);

		void resume();
		void pause();
		void start();

		unsigned& fps() { return fps_; }


		Square& get_square(coord pos);
		Square const& get_square(coord pos) const;

	};



}