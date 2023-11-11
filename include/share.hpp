#pragma once
#include <mnist.hpp>

namespace share {
	extern mnist::train_data traindata;
	extern mnist::train_data testdata;
	inline const int initnum = 50;
	inline const int generation = 100;
	inline const int delivery_son = 50;
}

