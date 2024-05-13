/**
 * @file main.cpp
 * @author Dylan Wright
 * @brief 
 * @version 1.0
 */

#include <iostream>
#include <sstream>
#include <string>

#include "KanoodleSolver.h"

int main() {
	std::vector<std::vector<int>> sparse_matrix;
	std::string line;
	int c;

	while(std::getline(std::cin, line)) {
		std::vector<int> row;
		std::stringstream ss(line);
		while (ss >> c) {
			row.push_back(c);
		}
		sparse_matrix.push_back(row);
	}

	KanoodleSolver slvr(sparse_matrix);
	slvr.solve();

	return 0;

}