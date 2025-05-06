#include <iostream>
#include <vector>
#include <fstream>
#include "../include/cache.h"

template <typename T, typename KeyT>
T slow_get_page(const int& keyT) {
	return keyT;
}

template <typename KeyT>
struct Input {
	int sz_ = 0;
	int n_ = 0;
	std::vector<KeyT> data_;
	Input(std::istream& in = std::cin);
};

template <typename KeyT>
Input<KeyT>::Input(std::istream& in) {
	long long sz; in >> sz; 
	if (sz == 0) {}
	this->sz_ = sz;
	int n; in >> n; this->n_ = n;
	if (n < 0) {
		//Error
	}
	else {
		std::vector<KeyT> data(n);
		for (int i = 0; i < n; ++i) {
			in >> data[i];
		}
		this->data_ = std::move(data);
	}
}

int main() {
	Input<int> data(std::cin);
	Cache<int, int> cache(slow_get_page<int, int>, data.sz_);
	for (int i = 0; i < data.n_; ++i) {
		cache.insert(data.data_[i]);
	}
	std::cout << Cache<int, int>::hits_ << std::endl;
}
