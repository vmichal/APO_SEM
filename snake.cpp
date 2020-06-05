#define _POSIX_C_SOURCE 200112L

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h>

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

	peripherals_intit();

	draw_board(board, COLUMNS, ROWS, SIDE);

	unsigned short *window = (unsigned short *)malloc(sizeof(short) * LCD_WIDTH * LCD_HEIGHT);
	for (int i = 0; i < LCD_WIDTH * LCD_HEIGHT; ++i) {
		window[i] = rgb_to_565(0, 0, 0);
	}

	write_line_to_fb(1, "text", window, BLUE);

	write_line_to_fb(14, "problem", window, RED);

	display_menu(window, 1);
	draw_window(window);

	struct timespec sleep_time { 0, 1000 * 1000 * 500 };
	pwm::audio.set_period(10'000'000); //Corresponds to cca 1ms
	pwm::audio.set_strength(4000);

	nanosleep(&sleep_time, nullptr);

	pwm::audio.set_strength(0);

	printf("poof\n");
	/*


	for (int i = 0; i < 1000; ++i) {

		led::rgb1.write(0xff7f00); //color orange
		led::rgb2.write(0xff7f00);
		led::line.write(i);
	}
	*/
	int line = 10;
	unsigned int rotation = knob_green.angle();
	for (;;) {
		clear_line(line, window, BLACK);
		line = knob_blue.angle() / 6 % 18;
		// write_line_to_fb(line, "no to snad neni mozny", window, RED);
		draw_window(window);
		led::rgb1.write(knob_red.angle() % 100);
		led::rgb2.write(knob_blue.pressed() ? led::Color::white : led::Color::black);
		if (rotation != knob_green.angle()) {
			move_selected(DOWN, window, 1);
			draw_window(window);
			rotation = knob_green.angle();
		}

		if (knob_green.pressed()) {
			display_menu(window, 0);
			draw_window(window);
			break;
		}
	}

	printf("Goodbye world\n");

	return 0;
}

