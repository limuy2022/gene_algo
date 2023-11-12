#include <cstdlib>
#include <mnist.hpp>

namespace mnist {
	size_t pic::xsize = 28;
	size_t pic::ysize = 28;
	size_t pic::sizenum = pic::xsize * pic::ysize;
	
	int ReverseInt(int i) {
		unsigned char ch1, ch2, ch3, ch4;
		ch1 = i & 255;
		ch2 = (i >> 8) & 255;
		ch3 = (i >> 16) & 255;
		ch4 = (i >> 24) & 255;
		return ((int)ch1 << 24) + ((int)ch2 << 16) + ((int)ch3 << 8) + ch4;
	}
	
	void loadlabels(fs::path anspath) {
		std::ifstream file(anspath, std::ios::binary);
		if (!file.good()) {
			std::cerr << "open " << anspath << " failed";
			exit(EXIT_FAILURE);
		}
		int magic_number = 0;
		int number_of_images = 0;
		file.read((char*)&magic_number, sizeof(magic_number));
		file.read((char*)&number_of_images, sizeof(number_of_images));
		magic_number = ReverseInt(magic_number);
		number_of_images = ReverseInt(number_of_images);
		std::cerr << "number of images = " << number_of_images << std::endl;
		std::ofstream filesave(anspath.replace_extension("data"));
		filesave << number_of_images << ' ';
		for (int i = 0; i < number_of_images; i++) {
			unsigned char label = 0;
			file.read((char*)&label, sizeof(label));
			filesave << (long long)label << ' ';
		}
		file.close();
		filesave.close();
	}
	
	void loaddata(fs::path datapath) {
		std::ifstream file(datapath, std::ios::binary);
		if (!file.good()) {
			std::cerr << "open " << datapath << " failed";
			exit(EXIT_FAILURE);
		}
		int magic_number = 0;
		int number_of_images = 0;
		int n_rows = 0;
		int n_cols = 0;
		file.read((char*)&magic_number, sizeof(magic_number));
		file.read((char*)&number_of_images, sizeof(number_of_images));
		file.read((char*)&n_rows, sizeof(n_rows));
		file.read((char*)&n_cols, sizeof(n_cols));
		magic_number = ReverseInt(magic_number);
		number_of_images = ReverseInt(number_of_images);
		n_rows = ReverseInt(n_rows);
		n_cols = ReverseInt(n_cols);
		std::ofstream filesave(datapath.replace_extension("data"), std::ios::binary);
		
		filesave << number_of_images << ' ' << \
		n_rows << ' ' <<\
		n_cols << ' ';
		std::cerr << "rows = " << n_rows << std::endl;
		std::cerr << "cols = " << n_cols << std::endl;
		
		for (int i = 0; i < number_of_images; i++) {
			for (int r = 0; r < n_rows; r++) {
				for (int c = 0; c < n_cols; c++) {
					unsigned char image = 0;
					file.read((char*)&image, sizeof(image));
					filesave << (long long)image << ' ';
				}
			}
		}
		file.close();
		filesave.close();
	}
	
	void initdata(fs::path datapath, fs::path anspath) {
		loadlabels(anspath);
		loaddata(datapath);
	}
	
	train_data load(const fs::path& datapath, const fs::path& labelpath) {
		std::ifstream datafile(datapath), ansfile(labelpath);
		if(!datafile.good()) {
			std::cerr << "load " << datapath << "failed";
			exit(EXIT_FAILURE);
		}
		if(!ansfile.good()) {
			std::cerr << "load " << labelpath << "failed";
			exit(EXIT_FAILURE);
		}
		train_data res;
		int images_number;
		datafile >> images_number;
		ansfile >> images_number;
		datafile >> mnist::pic::xsize >> mnist::pic::ysize;
		res.data.resize(images_number);
		for(auto&i:res.data){
			ansfile >> i.number;
		}
		datafile.close();
		ansfile.close();
		return res;
	}
}

