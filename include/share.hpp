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
	inline const int initnum = 10;
	// 繁殖的代数
	inline const int generation = 30;
	// 交配的个体数
	inline const int delivery_son = 10;
	// 保留的个体数
	inline const int keepnum = 1;
	// 变异概率
	inline const double mutation = 0.08;
	// 小范围变异转换大范围变异概率
	inline const double bigmutation = 0.2;
	// 训练数据大小
	inline const size_t trainnum = 60000;
	// 测试数据大小
	inline const size_t testnum = 10000;
#ifdef SIMPLE
	// 每次从测试数据中取值的比例
	inline const double trainprecent = 0.1;
#else
	inlinr const double trainprecent = 0.5;
#endif
}
