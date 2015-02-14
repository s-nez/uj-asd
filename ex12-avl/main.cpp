#include <iostream>
#include "avl.h"
#include <random>

int main (int argc, char** argv)
{
	AVL tree;
	int element;
	std::cout << "> ";
	std::cin >> element;
	while (std::cin and element != -2) {
		if (element == -1) {
			tree.draw();
		} else if (tree.exists(element)) {
			tree.remove(element);
			std::cout << tree.operation_summary() << std::endl;
		} else {
			tree.insert(element);
			std::cout << tree.operation_summary() << std::endl;
		}
		std::cout << "> ";
		std::cin >> element;
	}
	return 0;
}

