#include "2Q_cache.h"
#include <iostream>

static bool is_number(std::string& str) {
	return !str.empty() && (str.find_first_not_of("-0123456789") == str.npos);
}

int main() {
	std::string cache_sz_buf;
	std::cin >> cache_sz_buf;

	if (!is_number(cache_sz_buf)) {
		std::cout << "Cache size must be an integer!" << std::endl;
		return 1;
	}
	int try_cache_sz = std::atoi(cache_sz_buf.c_str());

	if (try_cache_sz <= 0) {
		std::cout << "Cache size must be > 0!" << std::endl;
		return 1;
	}

	size_t cache_sz = try_cache_sz;

	std::string npages_buf;
	std::cin >> npages_buf;

	if (!is_number(npages_buf)) {
    	std::cout << "Number of requests must be an integer!" << std::endl;
        return 1; 
  	}   
    int try_npages = std::atoi(npages_buf.c_str());

    if (try_npages < 0) {
   		std::cout << "Number of requests must be >= 0!" << std::endl;
        return 1;
  	}

    size_t npages = try_npages;


	Q2_cache cache (cache_sz);

	int ticks = 0;
	int page = 0;
	int input_res = 0;
	std::string page_buf;

	for (int i = 0; i < npages; i++) {
		std::cin >> page_buf;
		if (!is_number(page_buf)) {
			std::cout << "Request number must be an integer\n" << std::endl;
			return 1;
		}
		page = std::atoi(page_buf.c_str());	
		input_res = cache.insert_page(page);
		if (input_res == 2 || input_res == 3)
			ticks++;
		cache.coloured_dump();
	}
	std::cout << ticks << std::endl;

	return 0;
}
