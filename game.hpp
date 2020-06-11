#pragma once 
#include <vector>
#include <queue>
#include <chrono>
#include <memory>


#include "player.hpp"


namespace game {

	struct coord {
		int x, y;
	};

	constexpr coord operator+(coord const a, coord const b) {
		return coord{ a.x + b.x, a.y + b.y };
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
		std::unique_ptr<Player> player_;
		std::queue<coord> segments_;
		coord last_popped_ {0,0};

		coord head() const { return segments_.front(); }

		void update();

		Snake(std::unique_ptr<Player> player)
			:player_{std::move(player)} {}

	private:
		coord get_new_head() const;

	};



	class Game {

		std::vector<std::vector<Square>> game_board_;
		std::vector<Snake> snakes_;
		std::chrono::steady_clock::time_point last_frame_;

	public:
		Game(int width, int height);


		std::vector<std::vector<Square>> const& board() const { return game_board_; }

		bool frame_elapsed() const { return std::chrono::steady_clock::now() - last_frame_ > std::chrono::milliseconds{ 1000 }; };

		void update();

		void draw();

		void add_player(std::unique_ptr<Player> player);


		Square const* get_square(coord pos) const;

	};



}