#pragma once
#include <vector>
#include <cassert>

double sigmoid_interal(double x);

class matrix {
public:
	int n, m;
	std::vector<std::vector<double>> data;
	
	matrix() = delete;
	
	matrix(int n, int m);
	
	matrix operator*(const matrix&b);
	
	matrix operator+(const matrix&b);
	
	void randset();

	/**
	 * @brief 将所有项进行sigmoid，映射到(0,1)之间
	 */
	void sigmoid();
};

