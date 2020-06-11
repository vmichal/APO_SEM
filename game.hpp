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

	constexpr coord operator+(coord const a, coord const b) {
		return coord{ a.x + b.x, a.y + b.y };
	}

	constexpr bool operator==(coord const a, coord const b) {
		return a.x == b.x && a.y == b.y;
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
		int id_;
		std::unique_ptr<Player> player_;
		std::deque<coord> segments_;
		coord last_popped_{ 0,0 };

		Snake(int id, std::unique_ptr<Player> player)
			:id_{ id }, player_{ std::move(player) } {}

		coord head() const { return segments_.front(); }
		void move();

		void add_segment(coord new_head) { segments_.push_back(new_head); }

		coord get_new_head() const;

	};



	class Game {

		enum class State {

			initialization,
			paused,
			ended,
			running
		} state_ = State::initialization;

		std::vector<std::vector<Square>> game_board_;
		std::vector<std::unique_ptr<Snake>> snakes_;
		std::chrono::steady_clock::time_point last_frame_;
		mutable std::uniform_int_distribution<int> distribution_;

		Square* food_ = nullptr;

		coord generate_food() const;

		Square& get_square(coord pos);
	public:
		Game(int width, int height);


		std::vector<std::vector<Square>> const& board() const { return game_board_; }

		bool frame_elapsed() const { return std::chrono::steady_clock::now() - last_frame_ > std::chrono::microseconds{ 1'000'000 / FPS }; };

		void update();

		void draw();

		void add_player(std::unique_ptr<Player> player);

		void resume();
		void pause();
		void start();


		Square const& get_square(coord pos) const;

	};



}