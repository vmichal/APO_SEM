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
#include "application.hpp"
#include "help.hpp"

int main(int argc, char* argv[]) {

	init_lcd();

	menu_add("menus/paused.menu", 1);
	menu_add("menus/main.menu", 0);

	Application app;

	for (; app.running();)
		app.process();


	printf("Goodbye world\n");
	menu_clean_up();

	return 0;
}

