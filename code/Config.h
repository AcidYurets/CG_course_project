#pragma once
#include <fstream>
#include <vector>

struct Config {
	bool isParallel = false;
	unsigned int threadCount = 8; // used if is parallel

	std::ofstream of;
	std::vector<long long> tmp = std::vector<long long>(0);
};

extern Config config;
