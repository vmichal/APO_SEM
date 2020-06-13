
#include "map.hpp"
#include "display.hpp"

#include <assert.h>

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

	char const* to_string(Direction dir) {
		switch (dir) {
		case Direction::south: return "S";
		case Direction::north: return "N";
		case Direction::east: return "W";
		case Direction::west: return "E";
		}
		assert(false);
	}

	Map Map::load_from_file(std::string file) {

	}

	void Map::draw() const {

	}




}

