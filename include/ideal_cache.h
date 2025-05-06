#pragma once 

#include <iostream>
#include <unordered_map>
#include <set>
#include <list>
#include <vector>
#include <cstddef>

template <typename KeyT, typename Compare = std::less<KeyT>>
class IdealCache {
	using PairT = std::pair<KeyT, size_t>;
	using ListT = std::list<size_t>;
private:
	size_t size_;
	struct cmp_for_set;
	std::set<PairT, cmp_for_set> set_;
	std::unordered_map<KeyT, size_t> umap_;
	std::unordered_map<KeyT, ListT> data_;
	std::vector<KeyT> keys_;
	Compare cmp_;
	size_t ans_ = 0;

	struct cmp_for_set {
		bool operator() (const PairT& a, const PairT& b) const{
			return a.second < b.second;
		}
	};

private:
	inline void map_keys(const std::vector<KeyT>& keys);
	inline bool insert_page(const KeyT& key, size_t next_hit);
    inline void set_delete_by_key(const KeyT& key);
    inline void cache_add(const KeyT& key, size_t next_hit);
public:
	explicit IdealCache(const std::vector<KeyT>& keys, size_t size = 0, const Compare& cmp = Compare()) 
		: size_(size), cmp_(cmp), keys_(keys) { map_keys(keys); std::cout << "ctor ended\n"; }
	inline size_t get_ans();
};

template <typename KeyT, typename Compare>
void IdealCache<KeyT, Compare>::map_keys(const std::vector<KeyT>& keys) {
    printf("in mapping keys\n");
	for (size_t i = 1, ie = keys.size()+1; i < ie; ++i) {
        printf("here\n");
		data_[keys[i-1]].push_front(i);
	}
	for (auto& i : this->data_) { i.second.push_front(0); }
}

template <typename KeyT, typename Compare>
void IdealCache<KeyT, Compare>::set_delete_by_key(const KeyT& key) {
    auto i = set_.begin();
    while (!cmp_(i->first, key)) { ++i; }
    set_.erase(i); 
}

template <typename KeyT, typename Compare>
void IdealCache<KeyT, Compare>::cache_add(const KeyT& key, size_t next_hit) {
    set_.insert({key, next_hit});
    umap_[key] = next_hit;
}

template <typename KeyT, typename Compare>
bool IdealCache<KeyT, Compare>::insert_page(const KeyT& key, size_t next_hit) { 
    printf("inserting page\n");
    auto it = umap_.find(key); 
    if (next_hit == 0 && it != umap_.end()) {
		set_delete_by_key(key);
        umap_.erase(key);
		return true;
    }
    if (next_hit == 0) { return false; }
    
    if (it != umap_.end()) {
        set_delete_by_key(key);
        cache_add(key, next_hit);
        return true;
    }
    if (set_.size() < size_) {
		cache_add(key, next_hit);
		return false;
	}
    auto r_it = set_.rbegin();
	if (next_hit > r_it->second) { return false; }
    umap_.erase(r_it->first);
    set_delete_by_key(r_it->first);
    cache_add(key, next_hit);
    return false;
}

template <typename KeyT, typename Compare>
size_t IdealCache<KeyT, Compare>::get_ans() {
    printf("before cycle\n");
    for (auto i: keys_) {
        printf("here\n");
		if (data_[i].size() > 0) {
			data_[i].pop_back();
		}
		ans_ += insert_page(i, data_[i].back());
	}
	return ans_;
}

