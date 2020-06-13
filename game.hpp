#pragma once 
#include <vector>
#include <deque>
#include <chrono>
#include <memory>
#include <random>


#include "player.hpp"
#include "map.hpp"
#include "snake-options.hpp"


namespace game {


	struct Snake {
		Direction current_direction_ = Direction::north;
		std::deque<coord> segments_;

		coord tail() const { return segments_.back(); }
		coord head() const { return segments_.front(); }
		void append_segment(coord new_head) { segments_.push_back(new_head); }

		coord get_new_head() const;
		void turn(Player::Action action);

	};



	class Game {
	public:
		enum class State {

			initialization,
			paused,
			ended,
			running
		};
	private:
		coord const size_;
		State state_ = State::initialization;

		std::vector<std::vector<Square>> game_board_;
		std::vector<std::unique_ptr<Player>> players_;
		std::chrono::steady_clock::time_point last_frame_;
		mutable std::uniform_int_distribution<int> distribution_;

		Square* food_ = nullptr;

		coord generate_food() const;

	public:
		Game(int width, int height);


		std::vector<std::vector<Square>> const& board() const { return game_board_; }
		coord food() const { return food_->position_; }
		coord size() const { return size_; }
		bool frame_elapsed() const { return std::chrono::steady_clock::now() - last_frame_ > std::chrono::microseconds{ 1'000'000 / FPS }; };
		State state() const { return state_; }

		void update();

		void draw();

		void add_player(Player::Type player_type);

		void resume();
		void pause();
		void start();


		Square& get_square(coord pos);
		Square const& get_square(coord pos) const;

	};



}