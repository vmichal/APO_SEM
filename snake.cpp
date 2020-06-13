#define _POSIX_C_SOURCE 200112L

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <algorithm>
#include <functional>
#include <thread>
#include <chrono>


#include "display.hpp"
#include "led-line.hpp"
#include "snake-options.hpp"
#include "led-rgb.hpp"
#include "text.hpp"
#include "knobs.hpp"
#include "menu.hpp"
#include "audio.hpp"
#include "game.hpp"

int main(int argc, char* argv[]) {

	init_lcd();

	welcome_screen();
	display_lcd();

	// lcd test
	flood_fill_lcd(PINK);
	fill_pixel_lcd(400, 300, RED);
	fill_square_lcd(1, 1, BLUE);

	display_lcd();


	pwm::audio.frequency(440);
	pwm::audio.strength(4000);

	std::this_thread::sleep_for(std::chrono::milliseconds{ 2000 });

	pwm::audio.strength(0);

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

	std::unique_ptr<game::Game> g = std::make_unique<game::Game>(COLUMNS, ROWS);
	g->add_player(game::Player::Type::local);
	g->add_player(game::Player::Type::local);

	for (;;) {
		//Sample all knobs
		std::for_each(knobs::knobs.begin(), knobs::knobs.end(), std::mem_fn(&knobs::KnobManager::sample));

		if (g->frame_elapsed()) {
			g->update();
			g->draw();
		}

		/*
		if (std::chrono::steady_clock::now() - last_move > std::chrono::milliseconds{ 500 }) {
			last_move = std::chrono::steady_clock::now();
			if (knobs::Rotation const movement = knobs::green.movement(); movement != knobs::Rotation::none) {
				move_selected(movement == knobs::Rotation::counterclockwise ? DOWN : UP, 0);
				display_lcd();
			}
		}
		*/

		if (knobs::red.pressed()) {
			g->start();
		}
		if (knobs::blue.pressed()) {
			if (g->state() == game::Game::State::paused)
				g->resume();
			else
				g->pause();
		}

		if (knobs::green.pressed()) {
			display_menu(0);
			display_lcd();
			break;
		}
	}

	printf("Goodbye world\n");
	menu_clean_up();

	return 0;
}

