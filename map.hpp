#pragma once

#include <vector>
#include <string>


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

	coord displacement_in_direction(Direction);
	Direction turn_left(Direction);
	Direction turn_right(Direction);
	Direction opposite_direction(Direction);
	char const* to_string(Direction d);

	struct Square {
		coord position_;
		Entity entity_;
	};

	class Map {

		coord const size_;
		std::vector<std::vector<Square>> game_board_;
		std::vector<coord> starts_;

		static Map load_from_file(std::string fname);
		static std::vector<Map> loaded_maps_;

	public:
		static std::vector<Map>& maps() { return loaded_maps_; }
		static void load_maps(std::string directory);

		Map(int width, int height);

		void draw() const;
		coord size() const { return size_; }
		std::vector<std::vector<Square>>& board() { return game_board_; }
		std::vector<coord> starting_positions() const { return starts_; }
		void clear();

	};


}


