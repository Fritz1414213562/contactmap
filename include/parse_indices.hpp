#ifndef CONTACTMAP_PARSE_TARGET_INDICES_OPTION
#define CONTACTMAP_PARSE_TARGET_INDICES_OPTION
#include "utility.hpp"
#include<iostream>
#include<fstream>
#include<vector>
#include<string>

std::vector<int> parse_indices_in(const std::string& fname) {
	std::ifstream ifs(fname, std::ios::in);
	std::string line;
	std::vector<int> retval;
	while (std::getline(ifs, line)) {
		if (line.empty()) continue;
		else if (Utility::startswith(line, "#")) continue;
		else {
			retval.push_back(std::stoi(line));
		}
	}
	ifs.close();
	return retval;
}


#endif // CONTACTMAP_PARSE_TARGET_INDICES_OPTION
