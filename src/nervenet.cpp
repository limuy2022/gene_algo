#include "matrix.hpp"
#include <nervenet.hpp>
#include <share.hpp>
#include <vector>
#include <rand.hpp>

namespace nervenet {
	nervenet::nervenet(std::initializer_list<size_t> input): netsize(input) {
		auto en = input.end() - 1;
		for (auto i = input.begin(); i != en; ++i) {
			nerves.emplace_back(*i, *(i + 1));
		}
		for(auto i : input) {
			nervenum += i;
			bias.push_back(matrix(1, i));
		}
		layer = input.size();
	}

	void nervenet::randinit() {
		for (auto&i : nerves) {
			i.randset();
		}
		for (auto&i : bias) {
			i.randset();
		}
	}

	std::vector<nervenet> nervenet::reproduction(const nervenet& mother) {
		std::vector<nervenet> res;
		// 随机产生后代，后代每一个参数从亲代等概率选择
		for(int i = 1; i <= share::delivery_son; ++i) {
			nervenet son(netsize);
			// 随机bias
			auto iter = netsize.begin();
			for(int j = 0; j < layer; ++j, ++iter) {
				for(int k = 1; k <= *iter; ++k) {
					if(rands::rand32() % 1000 < share::mutation * 1000) {
					// 变异
						if(rands::rand32() % 1000 < share::bigmutation * 1000) {
							// 大范围变异
							son.bias[j].data[1][k] = rands::bigrand(rands::rand32);
						} else {
							// 小范围变异
							son.bias[j].data[1][k] = rands::initrand(rands::rand32);
						}
					} else {
						// 不变异,随机继承
						if(rands::rand32() % 2 == 1) {
							son.bias[j].data[1][k] = mother.bias[j].data[1][k];	
						} else {
							son.bias[j].data[1][k] = bias[j].data[1][k];
						}
					}
				}
			}
			// 随机边权重
			for(auto& matri:son.nerves) {
				for(int j = 1; j <= son.nervenum; ++j) {
					for(int k = 1; k <= matri.n; ++k) {
						for(int l = 1; l <= matri.m; ++l) {
							if(rands::rand32() % 1000 < share::mutation * 1000) {
								// 变异
								if(rands::rand32() % 1000 < share::bigmutation * 1000) {
									// 大范围变异
									son.nerves[j].data[k][l] = rands::bigrand(rands::rand32);
								} else {
									// 小范围变异
									son.nerves[j].data[k][l] = rands::initrand(rands::rand32);
								}
							} else {
								// 不变异,随机继承
								if(rands::rand32() % 2 == 1) {
									son.nerves[j].data[k][l] = mother.nerves[j].data[k][l];	
								} else {
									son.nerves[j].data[k][l] = matri.data[k][l];
								}
							}
						}
					}
				}
			}
			res.push_back(son);
		}
		return res;
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
		rate = 0;
		for(const auto&i:data.data) {
			std::vector<double> ans = calans(i);
			// 计算正确率
			for(auto j:ans) {
				if(j == i.number) {
					rate += (1 - j) * (1 - j);
				} else {
					rate += (0 - j) * (0 - j);
				}
			}
		}
		rate /= data.data.size();
		return rate;
	}
}

