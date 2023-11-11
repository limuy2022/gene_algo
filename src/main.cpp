#include <cassert>
#include <vector>
#include <cstring>
#include "matrix.hpp"
#include "mnist.hpp"
#include "nervenet.hpp"
#include "share.hpp"

std::vector<nervenet::nervenet> sons;
std::initializer_list<size_t> networksize = {784, 20, 20, 10};

// 初始化物种个数
void init_sons() {
	for(int i = 1; i <= share::initnum; ++i) {
		sons.emplace_back(networksize);
	}
}

// 繁殖
void reproduction() {
	for(int i = 1; i <= share::generation; ++i) {
		
	}
}

int main(int argc, char** argv) {
	if(argc == 2 && !strcmp(argv[1], "mnist")) {
		mnist::initdata("train-images.idx3-ubyte", "train-labels.idx1-ubyte");
		mnist::initdata("t10k-images.idx3-ubyte", "t10k-labels.idx1-ubyte");
	}
	// 读取数据
	share::traindata = mnist::load("train-images.data", "train-labels.data");
	share::testdata = mnist::load("t10k-images.data", "t10k-labels.data");
	init_sons();
	reproduction();
	return 0;
}

