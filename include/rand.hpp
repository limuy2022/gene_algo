#pragma once
#include <random>

namespace rands{
	// 32位随机数
	extern std::mt19937 rand32;
	// 64位随机数
	extern std::mt19937_64 rand64;
	// 小范围变异
	extern std::uniform_real_distribution<> initrand;
	// 大范围变异
	extern std::uniform_real_distribution<> bigrand;
}

