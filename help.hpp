#pragma once

#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include "text.hpp"
#include "snake-options.hpp"

namespace help {

	class Help {
	protected:
		std::vector<std::string> text;
	public:
		Help(const char *fname);
		void display_help(unsigned int line) const;
		std::size_t size() const {return text.size();}
	}; 

}

