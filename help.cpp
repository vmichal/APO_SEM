#include "help.hpp"
#include "snake-options.hpp"
#include <assert.h>

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

	void Help::display_help(unsigned int line) const {
		flood_fill_lcd(WHITE);
		assert(line >= 0 && line < text.size());
		std::cout << Help::text.size() << '\n';
		auto it = Help::text.begin() + line;
		for (unsigned int i = 0; i < MAX_LINE_NUMBER; i++, ++it) {
			if (it == text.end()) {
				break;
			}
			write_line_to_display(i, it->c_str(), BLACK, WHITE);
			// there i am gonna print to display
			// std::cout << *(it + (line) + i) << '\n';
		}
	}
}

