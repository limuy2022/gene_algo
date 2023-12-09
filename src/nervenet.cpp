#include "matrix.hpp"
#include <cassert>
#include <cstdlib>
#include <nervenet.hpp>
#include <share.hpp>
#include <vector>
#include <rand.hpp>

namespace nervenet {
	nervenet::nervenet(const std::vector<size_t>& input): netsize(input) {
		auto en = input.end() - 1;
		for (auto i = input.begin(); i != en; ++i) {
			nerves.emplace_back(*i, *(i + 1));
			require_num += *i * *(i + 1);
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

	std::vector<nervenet> nervenet::reproduction(const nervenet& mother) const {
		if(netsize.size() != mother.netsize.size()) {
			std::cerr << "net size isn't same " << netsize.size() << ' ' << mother.netsize.size() << '\n';
			exit(EXIT_FAILURE);
		} else {
			for(size_t i = 0; i < netsize.size(); ++i) {
				if(netsize[i] != mother.netsize[i]) {
					std::cerr << "net work's nerves number is not the same in the " \
					<< i + 1 << " item.One is " << netsize[i] <<' '<<".Another is " << mother.netsize[i];
					exit(EXIT_FAILURE);
				}
			}
		}
		std::vector<nervenet> res;
		// 随机产生后代，后代每一个参数从亲代等概率选择
		for(int i = 1; i <= share::delivery_son; ++i) {
			nervenet son(netsize);
			// 随机bias
			for(int j = 0; j < layer; ++j) {
				for(int k = 1; k <= son.bias[j].m; ++k) {
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
							son.bias[j].data[1][k] = \
							mother.bias[j].data[1][k];	
						} else {
							son.bias[j].data[1][k] = \
							bias[j].data[1][k];
						}
					}
				}
			}
			// 随机边权重
			for(size_t j = 0; j < nerves.size(); ++j) {
				for(int k = 1; k <= nerves[j].n; ++k) {
					for(int l = 1; l <= nerves[j].m; ++l) {
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
								son.nerves[j].data[k][l] = nerves[j].data[k][l];
							}
						}
					}
				}
			}
			res.push_back(son);
		}
		return res;
	}

	std::vector<double> nervenet::calans(const mnist::pic& data)const {
		assert(data.sizenum == netsize.front());
		matrix s(1, netsize.front());
		for (size_t i = 1; i <= data.val.size(); ++i) {
			s.data[1][i] = data.val[i - 1];
		}
		s = s + bias[0];
		for(int i = 0; i < layer - 1; ++i) {
			s = s * nerves[i];
			s = s + bias[i + 1];
			s.sigmoid();
		}
		std::vector<double> v;
		for (int i = 1; i <= s.m; ++i) {
			v.push_back(s.data[1][i]);
		}
		return v;
	}
	
	double nervenet::valfunc(const mnist::train_data& data) {
		rate = 0;
		size_t total = 0;
		for(const auto&i:data.data) {
			if(rands::rand32() % 100 >= share::trainprecent * 100) {
				// 这里概率选取，不然数据太大跑不完
				continue;
			}
			total++;
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
		rate /= total;
		return rate;
	}

	nervenet& nervenet::operator=(const nervenet&b) {
		nerves = b.nerves;
		bias = b.bias;
		nervenum = b.nervenum;
		layer = b.layer;
		rate = b.rate;
		return *this;
	}

	void nervenet::load(const std::vector<double>& weights, const std::vector<double>& bias_save) {
		assert(weights.size() == require_num);
		assert(bias_save.size() == nervenum);
		size_t cnt = 0;
		for(auto&i:nerves) {
			for(int j = 1; j <= i.n; ++j) {
				for(int k = 1; k <= i.m; ++k) {
					i.data[j][k] = weights[cnt];
					cnt++;
				}
			}
		}
		assert(cnt == require_num);
		cnt = 0;
		for(auto&i:bias) {
			for(int j = 1; j <= i.m; ++j) {
				i.data[1][j] = bias_save[cnt];
				cnt++;
			}
		}
		assert(cnt == nervenum);
	}
}
