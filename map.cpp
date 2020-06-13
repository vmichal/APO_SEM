
#include "map.hpp"
#include "display.hpp"

#include <filesystem>
#include <assert.h>

namespace game {

	std::vector<Map> Map::loaded_maps_;


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

	Map::Map(int width, int height)
		: size_{ width, height }, game_board_(height, std::vector<Square>(width)) {

		for (int row = 0; row < height; ++row)
			for (int col = 0; col < width; ++col) {
				game_board_[row][col].position_ = { col, row };
				game_board_[row][col].entity_ = Entity::none;
			}
	}

	Map Map::load_from_file(std::string file) {

	}

	void Map::draw() const {

	}

	void Map::load_maps(std::string directory) {
		assert(loaded_maps_.empty()); //Can be called only once 

		assert(std::filesystem::is_directory(directory));
		for (auto &entry : std::filesystem::directory_iterator(directory)) {
			if (!entry.is_regular_file())
				continue;

			if (entry.path().string().find(".map"))
				loaded_maps_.emplace_back(load_from_file(entry.path().string()));
		}

	}

}

