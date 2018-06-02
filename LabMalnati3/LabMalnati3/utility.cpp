#include "stdafx.h"
#include "utility.h"




void getCombinations(const vector<int> allNumber, int n, int k, list<ArrayAndScore>& goodarrays) {
	int pos = 0;
	//position of simple combination alghorithm
	vector<int> knumbers(k, -1);
	comb_sempl(pos, allNumber, knumbers, n, k, 0, goodarrays);
}

void comb_sempl(int pos, const vector<int> allNumber, vector<int> &knumbers, int n, int k, int start, list<ArrayAndScore>& goodarrays) {
	//if my vector is complete, i push it into the list
	if (pos >= k) {
		goodarrays.push_back(ArrayAndScore{ knumbers });
		return;
	}
	for (int i = start; i < n; i++) {
		knumbers[pos] = allNumber[i];
		comb_sempl(pos + 1, allNumber, knumbers, n, k, i + 1, goodarrays);
	}
}



list<pair<int, int>> generatePairs(int n) {
	list<pair<int, int>> result;
	for (int i = 0; i < n; i++) {
		for (int j = i+1; j < n; j++) {
			if (i != j) {
				result.push_back(pair<int, int>(i, j));
			}
		}
	}
	return result;
}

bool aasSort(ArrayAndScore a1, ArrayAndScore a2) {
	return a1.getScore() < a2.getScore();
}


string hash_vector(vector<int> vect) {
	string hash;
	int n = vect.size();
	for (int i = 0; i < n; i++)
	{
	hash.append(to_string(vect[i]) + "_");
	}
	return hash;
}