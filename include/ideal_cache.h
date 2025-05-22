#pragma once

#include <iostream>
#include <unordered_map>
#include <list>
#include <set>

enum Ideal_cache_input_res {
	tick = 1,
	miss = 0
};

struct Page{
	int page_number;
	int next_hit;
};

struct cmp {
	bool operator() (struct Page a1, struct Page a2) const {
		return a1.next_hit < a2.next_hit;
	}
};

class Ideal_cache {
	private: 
		size_t cache_size;
		std::set <struct Page, cmp> p_set;
		
		std::unordered_map <int, int> p_umap;
	public: 
		Ideal_cache (size_t cache_sz) : cache_size(cache_sz) {} 
		Ideal_cache_input_res insert_page (int page_number, int next_hit) {
			struct Page cur_page = {page_number, next_hit};
			if (next_hit == -1) {		
				if (p_umap.find(page_number) != p_umap.end()) {
					for (auto i = p_set.begin(), ie = p_set.end(); i != ie; ++i) {
						if (i->page_number == cur_page.page_number) {
							p_set.erase(i);
							break;
						}
					}
					p_umap.erase(page_number);
					return tick;
				} 
				return miss;
			}
			if (p_umap.find(page_number) != p_umap.end()) {
				for (auto i = p_set.begin(), ie = p_set.end(); i!= ie; ++i) {
					if (i->page_number == page_number) {
						p_set.erase(i);
						break;
					}
				}
				p_set.insert(cur_page);
				p_umap[page_number] = next_hit;
				return tick;
			} 
			if (p_set.size() < cache_size) {
				p_set.insert(cur_page);
				p_umap[page_number] = next_hit;
				
				return miss;
			} 
			if (next_hit > (*p_set.rbegin()).next_hit) 
				return miss;

			p_umap.erase((*p_set.rbegin()).page_number);
			
			Page pg_max = *p_set.rbegin();

			for (auto i = p_set.begin(), ie = p_set.end(); i != ie; ++i) {
				if (i->page_number == pg_max.page_number) {
					p_set.erase(i);
					break;
				}
			}

			p_set.insert(cur_page);
			p_umap[page_number] = next_hit;

			return miss;
		}
			
		void coloured_dump() {
              std::cout << "\033[0;33m" << "IDEAL_CACHE:\n" << "\033[0;0m";
              for (std::set <struct Page, cmp>::iterator i = p_set.begin(), ie = p_set.end(); i != ie; ++i) 
                  std::cout << "\033[0;34m" << "[" << "\033[0;0m" << i->page_number << "(" << i->next_hit << ")" << "\033[0;34m" << "]" << "\033[0;0m";
              std::cout << std::endl;
        };
};
