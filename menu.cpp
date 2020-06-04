#include "menu.hpp"

option_t main_options[] = {
	{.text = "Play"},
	{.text = "Help"}, 
	{.text = "Info"},
	{.text = "Settings"},
};

option_t pause_options[] = {
	{.text = "Continue"},
	{.text = "Back to main"}, 
};

menu_t menus[] = {
{
	.options = main_options,
	.selected_color = RED,
	.unselected_color = WHITE,
	.selected = 0,
	.num_options = 4,
},
{
	.options = pause_options,
	.selected_color = BLUE,
	.unselected_color = WHITE,
	.selected = 0,
	.num_options = 2,
}
};


void display_menu(unsigned short *window, int menu_id)
{
	for(int i = 0; i < menus[menu_id].num_options; ++i) {
		if (menus[menu_id].selected == i) {
			write_line_to_fb(i, menus[menu_id].options[i].text, window, menus[menu_id].selected_color);
		} else {
			write_line_to_fb(i, menus[menu_id].options[i].text, window, menus[menu_id].unselected_color);
		}
	}
}

void move_selected(bool up, unsigned short *window, int menu_id)
{
	if (up) {
		if (menus[menu_id].selected == 0) {
			write_line_to_fb(menus[menu_id].selected, menus[menu_id].options[menus[menu_id].selected].text, window, menus[menu_id].unselected_color);
			menus[menu_id].selected = menus[menu_id].num_options - 1;
			write_line_to_fb(menus[menu_id].selected, menus[menu_id].options[menus[menu_id].selected].text, window, menus[menu_id].selected_color);
		} else {
			write_line_to_fb(menus[menu_id].selected, menus[menu_id].options[menus[menu_id].selected].text, window, menus[menu_id].unselected_color);
			--menus[menu_id].selected;
			write_line_to_fb(menus[menu_id].selected, menus[menu_id].options[menus[menu_id].selected].text, window, menus[menu_id].selected_color);
		}
	} else {
		if (menus[menu_id].selected + 1 == menus[menu_id].num_options) {
			write_line_to_fb(menus[menu_id].selected, menus[menu_id].options[menus[menu_id].selected].text, window, menus[menu_id].unselected_color);
			menus[menu_id].selected = 0;
			write_line_to_fb(menus[menu_id].selected, menus[menu_id].options[menus[menu_id].selected].text, window, menus[menu_id].selected_color);
		} else {
			write_line_to_fb(menus[menu_id].selected, menus[menu_id].options[menus[menu_id].selected].text, window, menus[menu_id].unselected_color);
			++menus[menu_id].selected;
			write_line_to_fb(menus[menu_id].selected, menus[menu_id].options[menus[menu_id].selected].text, window, menus[menu_id].selected_color);
		}
	}
}

int get_selected(int menu_id)
{
	return menus[menu_id].selected;
}
