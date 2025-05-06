#include "../include/ideal_cache.h"
#include <iostream>
#include <cassert>

int main() {
    int npages;
    size_t sz;
    std::cin >> sz >> npages;
    assert(std::cin.good());
    std::vector<int> buf(npages);
    
    for (int i = 0; i < npages; ++i) { 
        int page;
        std::cin >> page;
        assert(std::cin.good());
        buf[i] = page;
    }
    IdealCache<int> id(buf, sz);
    std::cout << "before getting ans\n";
    std::cout << id.get_ans() << std::endl;
}


