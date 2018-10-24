#include "stdafx.h"
#include "Cache.h"
#include <mutex>


Cache<vector<int>, list<Movement>>::Cache()
{
	this->cache_movements = unordered_map<vector<int>, list<Movement>, container_hash<vector<int>>>{};
}


Cache<vector<int>, list<Movement>>::~Cache() {
}

bool Cache<vector<int>, list<Movement>>::get(const vector<int>& p, list<Movement>& s) {
	lock_guard<mutex> l(m);
	if (this->cache_movements.find(p) != this->cache_movements.end()){
		s = cache_movements.find(p)->second;
		return true;
	}
	return false;

}
void Cache<vector<int>, list<Movement>>::put(const vector<int>& p, const list<Movement>& s){
	lock_guard<mutex> l(m);
	if (cache_movements.find(p) == cache_movements.end()) {
		cache_movements[p] = s;
	}
}