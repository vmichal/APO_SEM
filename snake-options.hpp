#pragma once
/*
	Computer architectures semestral assignment - Zelinka Josef & Michal Vojtech

	File snake-options.hpp holds configuration values of the program.
*/

#include <array>
#include <chrono>
using namespace std::chrono_literals;

/*Dimenstion of the builtin LCD in pixels.*/
constexpr int LCD_WIDTH = 480;
constexpr int LCD_HEIGHT = 320;

//Side (in pixels) of one elementary square
constexpr int SIDE = 8;

//Number of squares vertically and horizontally
constexpr int COLUMNS = LCD_WIDTH / SIDE;
constexpr int ROWS = LCD_HEIGHT / SIDE;

constexpr int LED_line_length = 32;


//Static tests
static_assert(LCD_WIDTH% SIDE == 0, "Your squares have dimenstion incpomatible with used hardware!");
static_assert(LCD_HEIGHT% SIDE == 0, "Your squares have dimenstion incpomatible with used hardware!");


//Constants for dispalying text
#define STARTING_OFFSET 16
#define LINE_HEIGHT 16
#define BITS sizeof(uint16_t) * 8
#define MAX_LINE_LENGTH 60
#define MAX_LINE_NUMBER (LCD_HEIGHT - STARTING_OFFSET * 2) / LINE_HEIGHT
#define MIDDLE_FOR_HEADER ((LCD_HEIGHT - 88) / 2 - 10) 
#define PRESS_ANY_KEY_POS (((LCD_HEIGHT - 88) / 2) + 88 + 10)

//Constants for menus
#define MAX_MENU_NUM 5

//Menu enums
namespace menu {
	enum menus_t {
		MAIN_MENU,
		PAUSED_MENU,
		NUM_MENUS,
	};

	enum main_menu_opts {
		NEW_GAME_OPT,
		HELP_OPT,
		QUIT_OPT,
	};

	enum paused_menu_opts {
		PLAY_OPT,
		RETURN_OPT,
	};

}

//Handy colors
constexpr inline unsigned short rgb_to_565(const unsigned char r, const unsigned char g, const unsigned int b) {
	return ((r & 0b11111000) << 8) | ((g & 0b11111100) << 3) | (b >> 3);
}

constexpr short RED = rgb_to_565(255, 0, 0);
constexpr short PINK = rgb_to_565(255, 51, 204);
constexpr short GREEN = rgb_to_565(0, 255, 0);
constexpr short BLUE = rgb_to_565(0, 0, 255);
constexpr short WHITE = rgb_to_565(255, 255, 255);
constexpr short BLACK = rgb_to_565(0, 0, 0);
constexpr short ORANGE = rgb_to_565(255, 153, 0);
constexpr short PURPLE = rgb_to_565(153, 0, 255);
constexpr short YELLOW = rgb_to_565(204, 204, 0);


//Display text consts
#define UP true
#define DOWN false

namespace game::colors {

	constexpr std::array<short, 7> snakes = {
		PINK, GREEN, BLUE, RED, ORANGE, PURPLE, YELLOW
	};

	constexpr short bg = BLACK;
	constexpr short food = RED;
	constexpr short wall = WHITE;
	constexpr short starting_position = PINK;
}

constexpr int snake_start_length = 15;
constexpr auto debounce_delay = 250ms;
constexpr auto turn_threshold = 5;

constexpr unsigned death_animation_frames = 20;
constexpr unsigned powerup_lifetime = LED_line_length*4;
constexpr unsigned powerup_selection_time = 50;
constexpr unsigned powerup_random_coef = 64;

constexpr unsigned freeze_duration = 30;

constexpr auto welcome_screen_beep_frequency = 200ms;
constexpr auto welcome_screen_led_frequency = 50ms;

constexpr int score_gain_powerup = 5;
