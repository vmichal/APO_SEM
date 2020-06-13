/*
	Computer architectures semestral assignment - Zelinka Josef & Michal Vojtìch

	This file allows making menu various menu interfaces and their options.
	User can specify color of normal menu intem ande the selected option.
	File provides interface to interact with menu.
*/

#ifndef __MENU_H__
#define __MENU_H__

#include "display.hpp"
#include "snake-options.hpp"
#include "text.hpp"
#include <stdbool.h>
#include <stdio.h>
#include <assert.h>

namespace menu {


	typedef struct {
		char** options;
		unsigned short selected_color;
		unsigned short unselected_color;
		unsigned short bg_color;
		int selected;
		int num_options;
	} menu_t;

	void init();

	void add(const char* menu_name, menus_t menu_id);

	void clean_up();

	void display(menus_t menu);

	void move_selected(bool up, menus_t menu_id);

	int get_selected(menus_t menu_id);

}
#endif
