
#include "map.hpp"
#include "display.hpp"
#include "snake-options.hpp"

#include <filesystem>
#include <assert.h>
#include <fstream>

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

	Map Map::load_from_file(std::string fname) {

		std::ifstream file(fname);
		assert(file.is_open());

		int width, height;
		file >> width >> height;

		// ignore characters to read the map
		file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		Map new_map(width, height);

		int row = 0;
		std::string line;
		while (std::getline(file, line) && row < height) {
			assert(line.length() == width);
			for (int i = 0; i < width; i++) {
				switch (line[i]) {
				case ' ':
					new_map.game_board_[row][i].entity_ = Entity::none;
					break;
				case 'x':
					new_map.game_board_[row][i].entity_ = Entity::wall;
					break;
				case 's':
					new_map.game_board_[row][i].entity_ = Entity::none;
					new_map.starts_.push_back({ i, row });
					break;
				default:
					printf("Parsing unknown char '%c' on line %d:%d.\n", line[i], row, i);
					new_map.game_board_[row][i].entity_ = Entity::none;
					break;
				}
				new_map.game_board_[row][i].position_.x = i;
				new_map.game_board_[row][i].position_.y = row;
			}
			++row;
		}
		return new_map;
	}

	void Map::draw() const {
		for (int y = 0; y < size_.y; y++) {
			for (int x = 0; x < size_.x; x++) {
				if (game_board_[y][x].entity_ == Entity::wall) {
					fill_square_lcd(x, y, game::colors::wall);
				}
			}
		}
	}

	void Map::load_maps(std::string directory) {
		assert(loaded_maps_.empty()); //Can be called only once 

		assert(std::filesystem::is_directory(directory));
		for (auto& entry : std::filesystem::directory_iterator(directory))
			if (entry.path().string().find(".map"))
				loaded_maps_.emplace_back(load_from_file(entry.path().string()));

	}

	void Map::clear() {
		for (auto& line : game_board_)
			for (auto& square : line)
				if (square.entity_ != Entity::wall)
					square.entity_ = Entity::none;
	}

}

