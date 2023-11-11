#include <nervenet.hpp>

namespace nervenet {
	nervenet::nervenet(std::initializer_list<size_t> input): netsize(input) {
		auto en = input.end() - 1;
		for (auto i = input.begin(); i != en; ++i) {
			nerves.emplace_back(*i, *(i + 1));
		}
	}

	void nervenet::randinit() {
		for (auto&i : nerves) {
			i.randset();
		}
		for (auto&i : bias) {
			i = rands::initrand(rands::rand32);
		}
	}

	nervenet nervenet::reproduction(const nervenet& mother) {
		nervenet son(netsize);
		
		return son;
	}

	std::vector<double> nervenet::calans(const mnist::pic& data) {
		matrix s(1, *netsize.begin());
		for (size_t i = 1; i <= data.val.size() + 1; ++i) {
			s.data[1][i] = data.val[i - 1];
		}
		for (const auto&i : nerves) {
			s = s * i;
		}
		std::vector<double> v;
		for (int i = 1; i <= s.m; ++i) {
			v.push_back(s.data[1][i]);
		}
		return v;
	}
	
	double nervenet::valfunc(const mnist::train_data& data) {
		
	}
}

