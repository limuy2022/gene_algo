#include <matrix.hpp>
#include <rand.hpp>

matrix::matrix(int n, int m):n(n), m(m) {
	data.resize(n + 1);
	for(auto i:data) {
		i.resize(m + 1);
	}
}

matrix matrix::operator*(const matrix&b) {
	assert(m == b.n);
	matrix res(n, b.m);
	for(int i = 1; i <= n; ++i) {
		for(int j = 1; j <= b.m; ++j) {
			for(int k = 1; k <= m; ++k) {
				res.data[i][j] += data[i][k] * data[k][j];	
			}
		}
	}
	return res;
}

matrix matrix::operator+(const matrix&b) {
	assert(n == b.n && m == b.m);
	matrix res(n, m);
	for(int i = 1; i <= n; ++i) {
		for(int j = 1; j <= b.m; ++j) {
			res.data[i][j] = data[i][j] + b.data[i][j];
		}
	}
	return res;
}

void matrix::randset() {
	for(int i = 1; i <= n; ++i) {
		for(int j = 1; j <= m; ++j) {
			data[i][j] = rands::initrand(rands::rand32);
		}
	}
}

