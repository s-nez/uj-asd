#include <iostream>
#include <fstream>
#include <limits>
#include <utility>
#include <cstring>
#include "museum.h"

int main (int argc, char** argv)
{
	if (argc < 2) {
		std::cout << "You need to specify a filename\n";
		return 1;
	}

	std::ifstream input;
	input.open (argv[1]);

	if (!input) {
		std::cout << "Error while opening file\n";
		return 1;
	}

	int height, width;
	input >> height >> width;

	Museum ug_museum (height, width);
	if (argc > 2 and std::strcmp (argv[2], "-g") == 0)
		ug_museum.graphics = true;
	for (int h = 1; h <= height; ++h)
		for (int w = 1; w <= width; ++w)
			input >> ug_museum.room (h, w);

	int target_h, target_w;
	input >> target_h >> target_w;
	while (target_h != 0 && target_w != 0) {
		std::cout << ug_museum.tour (target_h, target_w) << '\n';
		input >> target_h >> target_w;
	}
	std::cout << ug_museum.entry_to_exit() << '\n';

	input.close();
	return 0;
}

