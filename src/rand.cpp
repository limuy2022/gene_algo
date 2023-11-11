#include <random>

namespace rands{
	std::random_device rddev;
	std::mt19937 rand32(rddev());
	std::mt19937_64 rand64(rddev());	
	std::uniform_real_distribution<> initrand(-3.0, 3.0);
}

