#pragma once
/*
	Computer architectures semestral assignment - Zelinka Josef & Michal Vojtìch

	File snake-options.hpp holds configuration values of the program.
*/

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
static_assert(LCD_WIDTH % SIDE == 0, "Your squares have dimenstion incpomatible with used hardware!");
static_assert(LCD_HEIGHT% SIDE == 0, "Your squares have dimenstion incpomatible with used hardware!");


//Constants for dispalying text
#define STARTING_OFFSET 16
#define LINE_HEIGHT 16
#define BITS sizeof(uint16_t) * 8
#define MAX_LINE_LENGTH 30
#define MAX_LINE_NUMBER (LCD_HEIGHT - STARTING_OFFSET * 2) / LINE_HEIGHT

//Constants for menus
#define MAX_MENU_NUM 5

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


//Display text consts
#define UP true
#define DOWN false
