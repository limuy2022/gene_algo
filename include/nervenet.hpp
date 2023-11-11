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
		std::vector<matrix> bias;
		// 矩阵规格
		std::initializer_list<size_t> netsize;
		// 神经元个数
		int nervenum = 0;
		// 神经元层数
		int layer = 0;
	public:
		// 上一次估价函数的得分
		double rate = 0;

		nervenet(std::initializer_list<size_t> input);
		
		void randinit();
		
		std::vector<nervenet> reproduction(const nervenet& mother);
		
		/**
		@brief 计算一张图片的神经网络输出
		*/
		std::vector<double> calans(const mnist::pic& data);
		
		/**
		@brief 估价函数，估计该神经网络的正确率
		*/
		double valfunc(const mnist::train_data& data);
	};
}
