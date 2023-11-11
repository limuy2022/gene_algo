#pragma once
#include <random>

namespace rands{
	extern std::mt19937 rand32;
	extern std::mt19937_64 rand64;	
	extern std::uniform_real_distribution<> initrand;
}

