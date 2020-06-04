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



