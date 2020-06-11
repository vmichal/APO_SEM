#include "menu.hpp"

/*
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
*/

menu_t *menus[MAX_MENU_NUM];
int count_menu = 0;


void menu_add(const char *menu_name, int menu_id)
{
	printf("ooff\n");
	// is the wanted menu in range
	assert(menu_id < MAX_MENU_NUM);

	FILE *menu_file = fopen(menu_name, "r");
	assert(menu_name != NULL);

	menu_t *menu = (menu_t *)malloc(sizeof(menu_t));
	assert(menu != NULL);

	int ret;
	// read the header 
	ret = fscanf(menu_file, "%d\n", &menu->num_options);
	assert(ret == 1);
	
	// read sepecified color
	int r, g, b;
	ret = fscanf(menu_file, "%d %d %d\n", &r, &g, &b);
	assert(ret == 3);
	menu->selected_color = rgb_to_565(r, g, b);

	ret = fscanf(menu_file, "%d %d %d\n", &r, &g, &b);
	assert(ret == 3);
	menu->unselected_color = rgb_to_565(r, g, b);

	// prepare space for options
	menu->options = (char **)malloc(sizeof(char *) * menu->num_options);
	assert(menu->options != NULL);

	// read options until foef
	char *option;
	size_t size;
	size_t chars_read;
	for (int i = 0; i < menu->num_options; i++) {
//		assert(feof(menu_file));
		option = (char *)malloc(sizeof(char));
		assert(option != NULL);
		chars_read = getline(&option, &size, menu_file);
		// check the length for me to be able to write it all to the screen
		if (chars_read < MAX_LINE_LENGTH) {
			option[chars_read - 1] = '\0';
		} else {
			option[MAX_LINE_LENGTH] = '\0';
		}
		menu->options[i] = option;
	}

	fclose(menu_file);

	menu->selected = 1;

	// append the new menu to the menus variable
	menus[menu_id] = menu;
	count_menu += 1;
}

// clean my previously created menus
void menu_clean_up()
{
	for (int i = 0; i < count_menu; i++) {
		// free the meunu entry
		for (int j = 0; j < menus[i]->num_options; j++) {
			free(menus[i]->options[j]);
		}
		free(menus[i]);
	}
}

void display_menu(unsigned short *window, int menu_id)
{
	for(int i = 0; i < menus[menu_id]->num_options; ++i) {
		if (menus[menu_id]->selected == i) {
			write_line_to_fb(i, menus[menu_id]->options[i], window, menus[menu_id]->selected_color);
		} else {
			write_line_to_fb(i, menus[menu_id]->options[i], window, menus[menu_id]->unselected_color);
		}
	}
}

void move_selected(bool up, unsigned short *window, int menu_id)
{
	if (up) {
		if (menus[menu_id]->selected == 0) {
			write_line_to_fb(menus[menu_id]->selected, menus[menu_id]->options[menus[menu_id]->selected], window, menus[menu_id]->unselected_color);
			menus[menu_id]->selected = menus[menu_id]->num_options - 1;
			write_line_to_fb(menus[menu_id]->selected, menus[menu_id]->options[menus[menu_id]->selected], window, menus[menu_id]->selected_color);
		} else {
			write_line_to_fb(menus[menu_id]->selected, menus[menu_id]->options[menus[menu_id]->selected], window, menus[menu_id]->unselected_color);
			--menus[menu_id]->selected;
			write_line_to_fb(menus[menu_id]->selected, menus[menu_id]->options[menus[menu_id]->selected], window, menus[menu_id]->selected_color);
		}
	} else {
		if (menus[menu_id]->selected + 1 == menus[menu_id]->num_options) {
			write_line_to_fb(menus[menu_id]->selected, menus[menu_id]->options[menus[menu_id]->selected], window, menus[menu_id]->unselected_color);
			menus[menu_id]->selected = 0;
			write_line_to_fb(menus[menu_id]->selected, menus[menu_id]->options[menus[menu_id]->selected], window, menus[menu_id]->selected_color);
		} else {
			write_line_to_fb(menus[menu_id]->selected, menus[menu_id]->options[menus[menu_id]->selected], window, menus[menu_id]->unselected_color);
			++menus[menu_id]->selected;
			write_line_to_fb(menus[menu_id]->selected, menus[menu_id]->options[menus[menu_id]->selected], window, menus[menu_id]->selected_color);
		}
	}
}

int get_selected(int menu_id)
{
	return menus[menu_id]->selected;
}
