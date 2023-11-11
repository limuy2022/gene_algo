#pragma once
#include <matrix.hpp>
#include <rand.hpp>
#include <mnist.hpp>
#include <initializer_list>

namespace nervenet {
	class nervenet {
		// 每一列的矩阵
		std::vector<matrix> nerves;
		// 偏置参数
		std::vector<double> bias;
		std::initializer_list<size_t> netsize;
	public:
		nervenet(std::initializer_list<size_t> input);
		
		void randinit();
		
		nervenet reproduction(const nervenet& mother);
		
		std::vector<double> calans(const mnist::pic& data);
		
		double valfunc(const mnist::train_data& data);
	};
}

