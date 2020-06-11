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

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	char **options;
	unsigned short selected_color;
	unsigned short unselected_color;
	int selected;
	int num_options;
} menu_t; 

void menu_add(const char *menu_name, int menu_id);

void menu_clean_up();

void display_menu(int menu_id);

void move_selected(bool up, int menu_id);

int get_selected(int menu_id);

#ifdef __cplusplus
}
#endif
#endif
