#pragma once
#include <vector>
#include <filesystem>
#include <fstream>
#include <cstdlib>
#include <iostream>

namespace fs = std::filesystem;

namespace mnist {
	class pic {
	public:
		std::vector<int> val;
		static int xsize, ysize;
		int number;
	};
	
	class train_data {
	public:
		std::vector<pic> data;
	};
	
	int ReverseInt(int i);
	
	void loadlabels(fs::path anspath);
	
	void loaddata(fs::path datapath);
	
	void initdata(fs::path datapath, fs::path anspath);
	
	train_data load(const fs::path& datapath, const fs::path& labelpath);
}

