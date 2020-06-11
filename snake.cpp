#define _POSIX_C_SOURCE 200112L

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <algorithm>
#include <functional>
#include <thread>
#include <chrono>


#include "peripherals.hpp"
#include "led-line.hpp"
#include "snake-options.hpp"
#include "led-rgb.hpp"
#include "text.hpp"
#include "knobs.hpp"
#include "menu.hpp"
#include "audio.hpp"

int main(int argc, char* argv[]) {
	int board[COLUMNS * ROWS];

	bool som = false;
	for (int y = 0; y < ROWS; ++y) {
		for (int i = 0; i < COLUMNS; ++i) {
			board[y * COLUMNS + i] = som;
			som = !som;
		}
		som = !som;
	}

	peripherals_init();

	// lcd test
	flood_fill_lcd(PINK);
	getchar();
	fill_pixel_lcd(400, 300, RED); 
	getchar();
	fill_square_lcd(1, 1, BLUE);
	getchar();

	draw_board(board, COLUMNS, ROWS, SIDE);

	unsigned short* window = (unsigned short*)malloc(sizeof(short) * LCD_WIDTH * LCD_HEIGHT);
	for (int i = 0; i < LCD_WIDTH * LCD_HEIGHT; ++i) {
		window[i] = rgb_to_565(0, 0, 0);
	}

	write_line_to_fb(1, "text", window, BLUE);

	write_line_to_fb(14, "problem", window, RED);

	//struct timespec sleep_time { 0, 1000 * 1000 * 500 };
	pwm::audio.set_period(10'000'000); //Corresponds to cca 1ms
	pwm::audio.set_strength(4000);

	std::this_thread::sleep_for(std::chrono::milliseconds{ 2000 });
	//nanosleep(&sleep_time, nullptr);

	pwm::audio.set_strength(0);

	printf("poof\n");
	/*


	for (int i = 0; i < 1000; ++i) {

		led::rgb1.write(0xff7f00); //color orange
		led::rgb2.write(0xff7f00);
		led::line.write(i);
	}
	*/

	// add new menu to menus
	menu_add("menus/paused.menu", 0);
	menu_add("menus/main.menu", 1);

	std::chrono::steady_clock::time_point last_move = std::chrono::steady_clock::now();

	for (;;) {
		//Sample all knobs
		std::for_each(knobs::knobs.begin(), knobs::knobs.end(), std::mem_fn(&knobs::KnobManager::sample));

		// clear_line(line, window, BLACK);
		draw_window(window);
		led::rgb_left.write(knobs::raw::red.angle() % 100);
		led::rgb_right.write(knobs::blue.pressed() ? led::Color::white : led::Color::black);

		if (std::chrono::steady_clock::now() - last_move > std::chrono::milliseconds{ 500 }) {
			last_move = std::chrono::steady_clock::now();
			if (knobs::Rotation const movement = knobs::green.movement(); movement != knobs::Rotation::none) {
				move_selected(movement == knobs::Rotation::counterclockwise ? DOWN : UP, window, 0);
				draw_window(window);
			}
		}

		if (knobs::green.pressed()) {
			display_menu(window, 0);
			draw_window(window);
			break;
		}
	}

	printf("Goodbye world\n");
	menu_clean_up();

	return 0;
}

