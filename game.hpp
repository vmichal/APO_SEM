#pragma once 
#include <vector>
#include <queue>
#include "player.hpp"


namespace game {

	enum class Entity {
		snake, wall, food, none
	};

	struct Square {
		int column_, row_;
		Entity entity_;
	};

	class Snake {

		std::queue<Square*> segments_;

		Square* head() const { return segments_.front(); }

	};



	class Game {

		std::vector<std::vector<Square>> squares_;

	public:
		Game(int width, int height);


		std::vector<std::vector<Square>> const& board() const { return squares_; }



	};



}