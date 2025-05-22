#include <iostream>
#include <unordered_map>
#include <list>
#include <cstdlib>
#include <cstdio>
#include <string>
#include <sys/stat.h> 

class Q2_cache_test {
public:
    size_t fifo_size;
    size_t lru_size;

    std::unordered_map<int, std::list<int>::iterator> fifo_umap;
    std::unordered_map<int, std::list<int>::iterator> lru_umap;
    std::list<int> fifo;
    std::list<int> lru;

    Q2_cache_test(size_t fifo_sz, size_t lru_sz)
        : fifo_size(fifo_sz), lru_size(lru_sz) {
        if (fifo_size == 0) fifo_size = 1;
        if (lru_size == 0) lru_size = 1;
    }

    void insert_page(int page_number) {
        int key = page_number;
        
        if (fifo_umap.find(key) != fifo_umap.end()) {
            fifo.erase(fifo_umap[key]);
            
            if (lru.size() >= lru_size) {
                lru_umap.erase(lru.back());
                lru.pop_back();
            }
            
            lru.push_front(key);
            lru_umap[key] = lru.begin();
            fifo_umap.erase(key);
            return;
        }
        
        if (lru_umap.find(key) != lru_umap.end()) {
            lru.erase(lru_umap[key]);
            lru.push_front(key);
            lru_umap[key] = lru.begin();
            return;
        }
        
        if (fifo.size() < fifo_size) {
            fifo.push_front(key);
            fifo_umap[key] = fifo.begin();
            return;
        }
        
        if (!fifo.empty()) {
            int fifo_last = fifo.back();
            fifo.erase(fifo_umap[fifo_last]);
            fifo_umap.erase(fifo_last);
            fifo.push_front(key);
            fifo_umap[key] = fifo.begin();
        }
    }
};

void ensure_directory_exists(const std::string& path) {
    struct stat st;
    if (stat(path.c_str(), &st)) {
        mkdir(path.c_str(), 0777);
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <number_of_tests>\n";
        return 1;
    }
    
    int ntests = atoi(argv[1]);
    if (ntests <= 0) {
        std::cerr << "Number of tests must be positive\n";
        return 1;
    }

    ensure_directory_exists("../build/Tests");
    
    FILE* tests_src = fopen("../build/Tests/tests.txt", "w");
    if (!tests_src) {
        std::cerr << "Failed to open tests.txt for writing\n";
        return 1;
    }

    srand(time(nullptr)); 

    for (int j = 0; j < ntests; j++) {
        int npages = rand() % 100000 + 1;    
        size_t cache_sz = rand() % 100 + 1;  
        
        size_t fifo_sz = std::max(1ul, cache_sz/2);
        size_t lru_sz = std::max(1ul, cache_sz - fifo_sz);
        
        fprintf(tests_src, "%ld %d", cache_sz, npages);
        
        Q2_cache_test cache(fifo_sz, lru_sz);
        
        for (int i = 0; i < npages; ++i) {
            int page = rand() % 100 + 1;  
            fprintf(tests_src, " %d", page);
            cache.insert_page(page);
        }

        fprintf(tests_src, "\n%ld", cache.fifo.size());
        for (int page : cache.fifo) {
            fprintf(tests_src, " %d", page);
        }

        fprintf(tests_src, "\n%ld", cache.lru.size());
        for (int page : cache.lru) {
            fprintf(tests_src, " %d", page);
        }

        fprintf(tests_src, "\n");
    }
    
    fclose(tests_src);
    return 0;
}
