#pragma once

#include <iostream>
#include <list>
#include <unordered_map> 

enum Q2_algorythm_result {
	inserted_in_fifo = 1,		//cache miss
	currently_in_fifo = 2,		//cache hit
	currently_in_lru = 3		//cache hit
};

class Q2_cache {
	private:
		size_t fifo_size;
		size_t lru_size;

		std::unordered_map <int, std::list <int>::iterator> fifo_umap;
		std::unordered_map <int, std::list <int>::iterator> lru_umap;

		std::list <int> fifo;
    		std::list <int> lru;

	public:
		Q2_cache (size_t cache_sz) {
			size_t fifo_sz = cache_sz/2;
			size_t lru_sz = cache_sz-fifo_sz;
			fifo_size = fifo_sz;
			lru_size = lru_sz;
		}

		Q2_algorythm_result insert_page(int page_number) {
			int key = page_number;
			if (fifo_umap.find(key) != fifo_umap.end()) {	//the page is currently in the fifo -> transport page to lru
				fifo.erase(fifo_umap[key]);

				if (lru.size() >= lru_size) {				//if the lru is filled -> pop the last element
					int lru_last = lru.back();
					lru_umap.erase(lru_last);
					lru.pop_back();
				}

				lru.push_front(key);
				lru_umap[key] = lru.begin();
				fifo_umap.erase(key);
				return currently_in_fifo;
			}
			if (lru_umap.find(key) != lru_umap.end()) {		//the page is currently in the lru -> replace is to the top 
				lru.erase(lru_umap[key]);
				lru.push_front(key);
				lru_umap[key] = lru.begin();
				return currently_in_lru;
			} 
			if (fifo.size() < fifo_size) {
				fifo.push_front(key);
				fifo_umap[key] = fifo.begin();
				return inserted_in_fifo;
			}

			int fifo_last = fifo.back();
			fifo.erase(fifo_umap[fifo_last]);
			fifo_umap.erase(fifo_last);
			fifo.push_front(key);
			fifo_umap[key] = fifo.begin();
			return inserted_in_fifo;

		};

		void file_dump(const char* file_name) {
			FILE* file = fopen(file_name, "w");
			fprintf(file, "%ld %ld\n%ld", fifo_size, lru_size, fifo.size());
			for (std::list <int>::iterator i = fifo.begin(); i != fifo.end(); ++i) {
				fprintf(file, " %d", *i);
			}
			fprintf(file, "\n%ld", lru.size());
			for (std::list <int>::iterator i = lru.begin(); i != lru.end(); ++i) {
				fprintf(file, " %d", *i);

			}
			fprintf(file, "\n");

			fclose(file);
		};

		void coloured_dump() {
			std::cout << "\033[0;33m" << "FIFO:\n" << "\033[0;0m";
			for (std::list <int>::iterator i = fifo.begin(); i != fifo.end(); ++i) {
				std::cout << "\033[0;34m" << "[" << "\033[0;0m" << *i << "\033[0;34m" << "]" << "\033[0;0m";
			}
			std::cout << std::endl << "\033[0;33m" << "LRU:\n" << "\033[0;0m";
			for (std::list <int>::iterator i = lru.begin(); i != lru.end(); ++i) {
				std::cout << "\033[0;34m" << "[" << "\033[0;0m" << *i << "\033[0;34m" << "]" << "\033[0;0m";
			}
			std::cout << std::endl;
		};

};
