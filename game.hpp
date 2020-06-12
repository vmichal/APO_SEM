#pragma once 
#include <vector>
#include <deque>
#include <chrono>
#include <memory>
#include <random>


#include "player.hpp"
#include "snake-options.hpp"


namespace game {

	struct coord {
		int x, y;
	};

	inline constexpr coord operator+(coord const a, coord const b) {
		return coord{ a.x + b.x, a.y + b.y };
	}

	inline constexpr bool operator==(coord const a, coord const b) {
		return a.x == b.x && a.y == b.y;
	}

	inline constexpr int modulo_value(int value, int modulo) {
		if (value < 0) return value + modulo;
		return value >= modulo ? value - modulo : value;
	}
	inline constexpr coord coord_clamp(coord value, coord max) {
		return coord{ modulo_value(value.x, max.x), modulo_value(value.y, max.y) };
	}

	enum class Entity {
		snake, wall, food, none
	};

	enum class Direction {
		north, south, west, east
	};

	struct Square {
		coord position_;
		Entity entity_;
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

		Square& get_square(coord pos);
	public:
		Game(int width, int height);


		std::vector<std::vector<Square>> const& board() const { return game_board_; }

		bool frame_elapsed() const { return std::chrono::steady_clock::now() - last_frame_ > std::chrono::microseconds{ 1'000'000 / FPS }; };
		State state() const { return state_; }

		void update();

		void draw();

		void add_player(Player::Type player_type);

		void resume();
		void pause();
		void start();


		Square const& get_square(coord pos) const;

	};



}