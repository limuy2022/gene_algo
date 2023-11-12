#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <ostream>
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
#ifdef SIMPLE
std::vector<size_t> networksize = {784, 16, 16, 10};
#else
std::vector<size_t> networksize = {784, 16, 16, 10};
#endif

// 初始化物种个数
void init_sons() {
	for(int i = 1; i <= share::initnum; ++i) {
		std::cout << "Initing " << i <<" ...\n" << std::flush;
		sons.emplace_back(networksize);
		sons.back().randinit();
		sons.back().valfunc(share::traindata);
		std::cout << "Finish initing " << i << '\n' << std::flush;
	}
}

auto ratecmp = [](const auto&a, const auto&b) {
	return a.rate < b.rate;
};

// 繁殖
// 保留最强的keepnum个个体不被筛选掉
void reproduction() {
	assert(share::delivery_son % 2 == 0);
	for(int i = 1; i <= share::generation; ++i) {
		std::cout << "generation " << i << " is training\n" << std::flush;
		decltype(sons) temp, nextans;
		// 先排序选出前keepnum个
		std::sort(sons.begin(), sons.end(), ratecmp);
		for(int j = 0; j < share::keepnum; ++j) {
			nextans.push_back(sons[j]);
		}
		// 将他们随机打乱分组交配
		std::shuffle(sons.begin(), sons.end(), rands::rand32);
		// 交配,结果储存在temp中
		for(int j = 0; j < share::initnum - 1; j += 2) {
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
		std::cout << "generation " << i << " has been trained\n" << std::flush;
	}
}

void save_data(const nervenet::nervenet& net, const fs::path&res) {
	std::ofstream file(res);
	for(size_t j = 0; j < net.nerves.size(); ++j) {
		for(int k = 1; k <= net.nerves[j].n; ++k) {
			for(int l = 1; l <= net.nerves[j].m; ++l) {
				file << net.nerves[j].data[k][l] << ' ';
			}
		}
	}
	for(auto i:net.bias) {
		for(int j = 1; j <= i.m; ++j) {
			file << i.data[1][j] << ' ';
		}
	}
	file.close();
}

nervenet::nervenet loadnet(const fs::path& model_file) {
	nervenet::nervenet ret(networksize);
	std::ifstream file(model_file);
	return ret;
}

#define LOADTEST share::testdata = mnist::load(datapath("t10k-images-idx3-ubyte.data"), datapath("t10k-labels-idx1-ubyte.data"))
#define LOADTRAIN share::traindata = mnist::load(datapath("train-images-idx3-ubyte.data"), datapath("train-labels-idx1-ubyte.data"))

int main(int argc, char** argv) {
	if(argc != 1) {
		if(!strcmp(argv[1], "mnist")) {
			mnist::initdata(datapath("train-images-idx3-ubyte"), datapath("train-labels-idx1-ubyte"));
			mnist::initdata(datapath("t10k-images-idx3-ubyte"), datapath("t10k-labels-idx1-ubyte"));
		} else if(!strcmp(argv[1], "test")) {
			std::cout << "test mode\n";
			LOADTEST;
			const auto&net = loadnet(argv[2]);
			int right_num = 0, total = share::testdata.data.size();
			for(const auto&i:share::testdata.data) {
				const auto& ret = net.calans(i);
				double maxval = std::numeric_limits<double>::lowest();
				int refnum = std::numeric_limits<int>::max();
				assert(ret.size() == 10);
				for(size_t j = 0; j < ret.size(); ++j) {
					if(ret[j] > maxval) {
						maxval = ret[j];
						refnum = j;
					}
				}
				std::cerr << "Output:" << refnum << ".Expect:" << i.number <<'\n';
				if(refnum == i.number) {
					right_num++;
				}
			}
			printf("Your net's rate is %.8lf.Total %d,Right %d\n", right_num * 1.0 / total, total, right_num);
			exit(0);
		} else {
			std::cerr << "unrecognized option";
			exit(EXIT_FAILURE);
		}
	}
	// 读取数据
	std::cout << "start reading file data....\n" << std::flush;
	LOADTRAIN;
	LOADTEST;
	std::cout << "finished reading\n" << std::flush;
	if(share::traindata.data.size() != share::trainnum) {
		std::cerr << "train data's pictures num isn't right.Except " << share::trainnum << ".Actually " << share::traindata.data.size();
		exit(EXIT_FAILURE);
	}
	std::cout << "Initing sons....\n" << std::flush;
	init_sons();
	std::cout << "Init sons finish\nstart reproduction\n" << std::flush;
	reproduction();
	std::cout << "finish training\n" << std::flush;
	std::sort(sons.begin(), sons.end(), ratecmp);
	std::cout << "Saving data....\n" << std::flush;
	save_data(sons.front(), "nervenet_data_save");
	std::cout << "END\n" << std::flush;
	return 0;
}
