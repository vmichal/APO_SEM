#include "help.hpp"

namespace help {
	Help::Help(const char *fname) {
		std::string line;
		std::ifstream file(fname);
		if (file.is_open()) {
			while (std::getline(file, line)) {
				Help::text.push_back(line);
			}
			file.close();
		} else {

			std::cout << fname << "Unable to open file\n";
		}
	}

	void Help::display_help(unsigned int line) {
		flood_fill_lcd(WHITE);
		if (line < Help::text.size() && line >= 0) {
			std::cout << Help::text.size()<< '\n';
			auto it = Help::text.begin();
			for (unsigned int i = 0; i < MAX_LINE_NUMBER; i++) {
				if (line + i + 1 > Help::text.size()) {
					break;
				}
				write_line_to_display(i, (*(it + (line) + i)).c_str() , BLACK, WHITE);
				// there i am gonna print to display
				// std::cout << *(it + (line) + i) << '\n';
			}
		} else {
			std::cout << "Line number out of range\n";
		}
	}
}

