#pragma once
#include <matrix.hpp>
#include <rand.hpp>
#include <mnist.hpp>
#include <initializer_list>

namespace nervenet {
	class nervenet {
	public:
		// 每一列的矩阵
		std::vector<matrix> nerves;
		// 偏置参数
		std::vector<matrix> bias;
		// 矩阵规格
		const std::vector<size_t>& netsize;
		// 神经元个数
		size_t nervenum = 0;
		// 神经元层数
		int layer = 0;
		// 需要的参数个数
		size_t require_num = 0;
		// 上一次估价函数的得分
		double rate = 0;

		nervenet() = delete;

		nervenet(const std::vector<size_t>& input);
		
		void randinit();

		void load(const std::vector<double>& weights, const std::vector<double>& bias_save);
		
		std::vector<nervenet> reproduction(const nervenet& mother) const;
		
		/**
		@brief 计算一张图片的神经网络输出
		*/
		std::vector<double> calans(const mnist::pic& data) const;
		
		/**
		@brief 估价函数，估计该神经网络的正确率,并将值存入rate变量中
		*/
		double valfunc(const mnist::train_data& data);

		nervenet& operator=(const nervenet& b);
	};
}
