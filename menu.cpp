#include "menu.hpp"

option_t main_options[] = {
	{.text = "Play", .selected = true},
	{.text = "Play", .selected = false}, 
	{.text = "Play", .selected = false},
};

menu_t menu = {
	.options = main_options,
	.selected_color = RED,
	.unselected_color = WHITE,
	.num_options = 3,
};

void display_menu(unsigned short *window)
{
	for(int i = 0; i < menu.num_options; ++i) {
		if (menu.options[i].selected) {
			write_line_to_fb(i, menu.options[i].text, window, menu.selected_color);
		} else {
			write_line_to_fb(i, menu.options[i].text, window, menu.unselected_color);
		}
	}
}
