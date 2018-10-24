#pragma once
#include "Movement.h"
#include "ArrayAndScore.h"

#include <unordered_map>
#include <vector>
#include <boost/functional/hash.hpp>

template <typename Container> // we can make this generic for any container [1]
struct container_hash {
	std::size_t operator()(Container const& c) const {
		return boost::hash_range(c.begin(), c.end());
	}
};

template<typename Problem, typename Solution>
class Cache {
public:
	bool get(const Problem& p, Solution& s); //ritorna false in caso di MISS
	void put(const Problem& p, const Solution& s);
	Cache();
	~Cache();
};

template<>
class Cache<vector<int>,list<Movement>> {
public:
	unordered_map<vector<int>, list<Movement>, container_hash<vector<int>>> cache_movements{};
	mutex m;

	bool get(const vector<int>& p, list<Movement>& s); //ritorna false in caso di MISS
	void put(const vector<int>& p, const list<Movement>& s);
	Cache();
	~Cache();

};
