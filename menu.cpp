#include "menu.hpp"

bool used_menus[MAX_MENU_NUM];
menu_t *menus[MAX_MENU_NUM];

void menu_init()
{
	for (int i = 0; i < MAX_MENU_NUM; i++) {
		used_menus[i] = false;
	}
}

void menu_add(const char *menu_name, int menu_id)
{
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

	ret = fscanf(menu_file, "%d %d %d\n", &r, &g, &b);
	assert(ret == 3);
	menu->bg_color = rgb_to_565(r, g, b);

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
		size = 1;
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

	menu->selected = 0;

	// append the new menu to the menus variable
	menus[menu_id] = menu;
	used_menus[menu_id] = true;
}

// clean my previously created menus
void menu_clean_up()
{
	for (int i = 0; i < MAX_MENU_NUM; i++) {
		// free the meunu entry
		if (used_menus[i]) {
			for (int j = 0; j < menus[i]->num_options; j++) {
				free(menus[i]->options[j]);
			}
			free(menus[i]->options);
			free(menus[i]);
		}
	}
}

void display_menu(int menu_id)
{
	flood_fill_lcd(menus[menu_id]->bg_color);
	for(int i = 0; i < menus[menu_id]->num_options; ++i) {
		if (menus[menu_id]->selected == i) {
			write_line_to_display(i, menus[menu_id]->options[i], menus[menu_id]->selected_color, menus[menu_id]->bg_color);
		} else {
			write_line_to_display(i, menus[menu_id]->options[i], menus[menu_id]->unselected_color, menus[menu_id]->bg_color);
		}
	}
}

void move_selected(bool up, int menu_id)
{
	// change color of the old selected option
	// add backgroud color to the mix
	write_line_to_display(menus[menu_id]->selected, menus[menu_id]->options[menus[menu_id]->selected], menus[menu_id]->unselected_color, menus[menu_id]->bg_color);
	if (up) {
		if (menus[menu_id]->selected == 0) {
			menus[menu_id]->selected = menus[menu_id]->num_options - 1;
		} else {
			--menus[menu_id]->selected;
		}
	} else {
		if (menus[menu_id]->selected + 1 == menus[menu_id]->num_options) {
			menus[menu_id]->selected = 0;
		} else {
			++menus[menu_id]->selected;
		}
	}
	// change color of the new selected option
	write_line_to_display(menus[menu_id]->selected, menus[menu_id]->options[menus[menu_id]->selected], menus[menu_id]->selected_color, menus[menu_id]->bg_color);
}

int get_selected(int menu_id)
{
	return menus[menu_id]->selected;
}

