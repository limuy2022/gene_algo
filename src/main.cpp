#include <algorithm>
#include <cassert>
#include <vector>
#include <cstring>
#include <matrix.hpp>
#include <mnist.hpp>
#include <nervenet.hpp>
#include <share.hpp>
#include <random>
#include <rand.hpp>
#include <algorithm>

std::vector<nervenet::nervenet> sons;
std::initializer_list<size_t> networksize = {784, 20, 20, 10};

// 初始化物种个数
void init_sons() {
	for(int i = 1; i <= share::initnum; ++i) {
		sons.emplace_back(networksize);
		sons.back().randinit();
	}
}

// 繁殖
// 保留最强的keepnum个个体不被筛选掉
void reproduction() {
	for(int i = 1; i <= share::generation; ++i) {
		// 将他们随机打乱分组交配
		std::shuffle(sons.begin(), sons.end(), rands::rand32);
		// 交配,结果储存在temp中
		decltype(sons) temp;
		for(int j = 1; j <= share::delivery_son; j += 2) {
			const auto& tmp = sons[j].reproduction(sons[j + 1]);
			temp.insert(temp.end(), tmp.begin(), tmp.end());
		}
		// 遍历进行估价
		for(auto&j:temp) {
			j.valfunc(share::traindata);
		}
		std::sort(temp.begin(), temp.end(), [](const auto&a, const auto&b) {
			return a.rate < b.rate;
		});
	}
}

int main(int argc, char** argv) {
	if(argc == 2 && !strcmp(argv[1], "mnist")) {
		mnist::initdata(datapath("train-images.idx3-ubyte"), datapath("train-labels.idx1-ubyte"));
		mnist::initdata(datapath("t10k-images.idx3-ubyte"), datapath("t10k-labels.idx1-ubyte"));
	}
	// 读取数据
	share::traindata = mnist::load(datapath("train-images.data"), datapath("train-labels.data"));
	share::testdata = mnist::load(datapath("t10k-images.data"), datapath("t10k-labels.data"));
	init_sons();
	reproduction();
	return 0;
}
