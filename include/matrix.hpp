#pragma once
#include <vector>
#include <cassert>

class matrix {
public:
	int n, m;
	std::vector<std::vector<double>> data;
	
	matrix() = default;
	
	matrix(int n, int m);
	
	matrix operator*(const matrix&b);
	
	matrix operator+(const matrix&b);
	
	void randset();
};

