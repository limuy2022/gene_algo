#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <vector>
#include <cstring>
#include <matrix.hpp>
#include <mnist.hpp>
#include <nervenet.hpp>
#include <share.hpp>
#include <random>
#include <rand.hpp>
#include <algorithm>
#include <filesystem>

namespace fs = std::filesystem;


std::vector<nervenet::nervenet> sons;
std::initializer_list<size_t> networksize = {784, 20, 20, 10};

// 初始化物种个数
void init_sons() {
	for(int i = 1; i <= share::initnum; ++i) {
		sons.emplace_back(networksize);
		sons.back().randinit();
		sons.back().valfunc(share::traindata);
	}
}

auto ratecmp = [](const auto&a, const auto&b) {
	return a.rate < b.rate;
};

// 繁殖
// 保留最强的keepnum个个体不被筛选掉
void reproduction() {
	for(int i = 1; i <= share::generation; ++i) {
		decltype(sons) temp, nextans;
		// 先排序选出前keepnum个
		std::sort(sons.begin(), sons.end(), ratecmp);
		for(int j = 0; j < share::keepnum; ++j) {
			nextans.push_back(sons[j]);
		}
		// 将他们随机打乱分组交配
		std::shuffle(sons.begin(), sons.end(), rands::rand32);
		// 交配,结果储存在temp中
		for(int j = 1; j <= share::delivery_son; j += 2) {
			const auto& tmp = sons[j].reproduction(sons[j + 1]);
			temp.insert(temp.end(), tmp.begin(), tmp.end());
		}
		// 遍历进行估价
		for(auto&j:temp) {
			j.valfunc(share::traindata);
		}
		std::sort(temp.begin(), temp.end(), ratecmp);
		for(int j = 0; j < share::initnum - share::keepnum; ++j) {
			nextans.push_back(temp[j]);
		}
		sons = nextans;
	}
}

void save_data(const nervenet::nervenet& net, const fs::path&res) {
	std::ofstream file(res);
	for(int j = 0; j < net.nerves.size(); ++j) {
		for(int k = 1; k <= net.nerves[j].n; ++k) {
			for(int l = 1; l <= net.nerves[j].m; ++l) {
				
			}
		}
	}
	file.close();
}

int main(int argc, char** argv) {
	if(argc == 2) {
		if(!strcmp(argv[1], "mnist")) {
			mnist::initdata(datapath("train-images-idx3-ubyte"), datapath("train-labels-idx1-ubyte"));
			mnist::initdata(datapath("t10k-images-idx3-ubyte"), datapath("t10k-labels-idx1-ubyte"));
		} else if(!strcmp(argv[1], "test")) {
			
		} else {
			std::cerr << "unrecognized option";
			exit(EXIT_FAILURE);
		}
	}
	// 读取数据
	share::traindata = mnist::load(datapath("train-images-idx3-ubyte-data"), datapath("train-labels-idx1-ubyte-data"));
	share::testdata = mnist::load(datapath("t10k-images-idx3-ubyte-data"), datapath("t10k-labels-idx1-ubyte-data"));
	init_sons();
	reproduction();
	std::sort(sons.begin(), sons.end(), ratecmp);
	
	return 0;
}
