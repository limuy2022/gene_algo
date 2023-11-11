#pragma once
#include <mnist.hpp>

// 数据路径
#define datapath(x) "./data/" x

namespace share {
	// 训练数据
	extern mnist::train_data traindata;
	// 测试数据
	extern mnist::train_data testdata;
	// 初始化个体数
	inline const int initnum = 50;
	// 繁殖的代数
	inline const int generation = 100;
	// 交配的个体数
	inline const int delivery_son = 50;
	// 保留的个体数
	inline const int keepnum = 10;
	// 变异概率
	inline const double mutation = 0.03;
	// 小范围变异转换大范围变异概率
	inline const double bigmutation = 0.2;
}
