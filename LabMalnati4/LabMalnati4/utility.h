#pragma once
#include "ArrayAndScore.h"
#include <string>


void getCombinations(const vector<int> allNumber, int n, int k, list<ArrayAndScore>& goodarrays);
void comb_sempl(int pos, const vector<int> allNumber, vector<int>& knumbers, int n, int k, int start, list<ArrayAndScore>& goodarrays);
list<pair<int, int>> generatePairs(int n);
bool aasSort(ArrayAndScore a1, ArrayAndScore a2);
string hash_vector(vector<int> vect);
