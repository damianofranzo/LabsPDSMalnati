// LabMalntati3.cpp
//

#include "stdafx.h"
#include <vector>
#include <list>
#include <iostream>
#include<fstream>
#include <functional>
#include <algorithm>    
#include "ArrayAndScore.h"
#include <map>
#include <set>
#include <unordered_map>
#include <string>
#include "utility.h"
#include <boost/functional/hash.hpp>

#define K 7
using namespace std;


template <typename Container> // we can make this generic for any container [1]
struct container_hash {
	std::size_t operator()(Container const& c) const {
		return boost::hash_range(c.begin(), c.end());
	}
};



void proneList(list<ArrayAndScore>& goodarrays);
void proneAndUpdate(ArrayAndScore &aas);
void proneAndUpdate_gt2(ArrayAndScore &aas);
void getArrayAndScore_n2(ArrayAndScore &aas);

static unordered_map<vector<int>, list<Movement>, container_hash<vector<int>>> cache_movements{};
static set<vector<int>> invalid_vectors;
//static unordered_map<string, list<Movement>> cache_movements;



int main()
{
	vector<int> numbers{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

	list<ArrayAndScore> goodarrays;
	getCombinations(numbers, numbers.size(), K, goodarrays);
	/*
	//DEBUG
	goodarrays.push_back(ArrayAndScore{vector<int>{2,5,7}});
	//pruning on goodarrays
	*/
	proneList(goodarrays);

	ofstream fout;
	fout.open("output.txt");
	if (goodarrays.size() >= 1000) {
		cout << "List too big, the threeshold has to be littler" << endl;
		return -1;
	}
	for (list<ArrayAndScore>::iterator iter = goodarrays.begin(); iter != goodarrays.end(); iter++) {
		for (const auto num : iter->numbers)
			fout << num << ' ';
		fout << iter->score << endl;

		list<Movement> movements = iter->getMovements();
		for (list<Movement>::iterator i = movements.begin(); i != movements.end(); i++)
		{
			fout << i->op1 << " " << i->operation << " " << i->op2 << " single score: " << i->score << endl;
		}
		fout << endl << endl;
	}
	system("pause");
	return 0;
}

void proneList(list<ArrayAndScore>& goodarrays) {
	for (auto &i : goodarrays) {
		proneAndUpdate(i);
	}
}


void proneAndUpdate(ArrayAndScore& aas) {

	//first we check if the user is looking for solutions with small k
	switch (aas.getSize())
	{
	case 1:
		cout << "not yet implemented" << endl;
		break;
	case 2:
		getArrayAndScore_n2(aas);
		break;
	default:
		proneAndUpdate_gt2(aas);
		break;
	}
}


void proneAndUpdate_gt2(ArrayAndScore &aas) {
	if (aas.getSize() == 2) {
		//chose best solution
		getArrayAndScore_n2(aas);
	}
	else {
		vector<int> localnumbers = aas.getVector();
		list<pair<int, int>> pairs = generatePairs(localnumbers.size());
		list<ArrayAndScore> candidates = list<ArrayAndScore>();
		for (pair<int, int> singlepair : pairs) {
			//get the two values
			int i1 = singlepair.first;
			int i2 = singlepair.second;

			int a = localnumbers[i1];
			int b = localnumbers[i2];
			int c1 = a + b;
			int c2 = b - a;

			auto eraseAB = [&](vector<int> &v, int i1, int i2) {
				v.erase(v.begin() + i1);
				if (i1 > i2)
					v.erase(v.begin() + i2);
				else
					v.erase(v.begin() + i2 - 1);
			};


			//This vector does not contain nor a or b.
			vector<int> localnumbersWoutAB(localnumbers);
			eraseAB(localnumbersWoutAB, i1, i2);

			//SUM, ALWAYS POSSIBLE
			{
				vector<int> localnumbers1(localnumbersWoutAB);
				localnumbers1.push_back(c1);
				if (invalid_vectors.find(localnumbers1) == invalid_vectors.end()) {
					ArrayAndScore aas1(localnumbers1);
					aas1.sortNumbers();
					aas1.insertMovement(a, b, '+', c1 * 2);

					//check cache
					list<Movement> movements1;
					if ((cache_movements.find(localnumbers1)) != cache_movements.end()) {
						movements1 = cache_movements.find(localnumbers1)->second;
						aas1.addMovements(movements1);
						candidates.insert(candidates.end(), aas1);
					}
					else {
						proneAndUpdate_gt2(aas1);
						if (aas1.isValid())
							candidates.insert(candidates.end(), aas1);
						else
							invalid_vectors.insert(localnumbers1);
					}
				}
			}

			//DIFFERENCE
			if (c2 >= 0) {
				vector<int> localnumbers2(localnumbersWoutAB);
				localnumbers2.push_back(c2);
				if (invalid_vectors.find(localnumbers2) == invalid_vectors.end()) {

					ArrayAndScore aas2(localnumbers2);
					aas2.sortNumbers();
					aas2.insertMovement(b, a, '-', c2 * 1);

					//check cache
					list<Movement> movements2;
					if ((cache_movements.find(localnumbers2)) != cache_movements.end()) {
						movements2 = cache_movements.find(localnumbers2)->second;
						aas2.addMovements(movements2);
						candidates.insert(candidates.end(), aas2);
					}
					else {
						proneAndUpdate_gt2(aas2);
						if (aas2.isValid())
							candidates.insert(candidates.end(), aas2);
						else
							invalid_vectors.insert(localnumbers2);
					}
				}
			}

			//MULTIPLICATION
			if (!(a == 0 || b == 0)) {
				int c3 = a * b;
				vector<int> localnumbers3(localnumbersWoutAB);
				localnumbers3.push_back(c3);
				if (invalid_vectors.find(localnumbers3) == invalid_vectors.end()) {

					ArrayAndScore aas3(localnumbers3);
					aas3.sortNumbers();
					aas3.insertMovement(b, a, '*', c3 * 4);

					//check cache
					list<Movement> movements3;
					if ((cache_movements.find(localnumbers3)) != cache_movements.end()) {
						movements3 = cache_movements.find(localnumbers3)->second;
						aas3.addMovements(movements3);
						candidates.insert(candidates.end(), aas3);
					}
					else {
						proneAndUpdate_gt2(aas3);
						if (aas3.isValid())
							candidates.insert(candidates.end(), aas3);
						else
							invalid_vectors.insert(localnumbers3);
					}
				}
			}

			//DIVISION
			if (a != 0) {
				if (b % a == 0) {
					int c4 = b / a;
					vector<int> localnumbers4(localnumbersWoutAB);
					localnumbers4.push_back(c4);
					if (invalid_vectors.find(localnumbers4) == invalid_vectors.end()) {
						ArrayAndScore aas4(localnumbers4);
						aas4.sortNumbers();
						aas4.insertMovement(b, a, '/', c4 * 3);

						//check cache
						list<Movement> movements4;
						if ((cache_movements.find(localnumbers4)) != cache_movements.end()) {
							movements4 = cache_movements.find(localnumbers4)->second;
							aas4.addMovements(movements4);
							candidates.insert(candidates.end(), aas4);
						}
						else {
							proneAndUpdate_gt2(aas4);
							if (aas4.isValid())
								candidates.insert(candidates.end(), aas4);
							else
								invalid_vectors.insert(localnumbers4);
						}
					}
				}
			}




		}
		//if there is at least candidate, i chose the best
		if (candidates.size() > 0) {
			aas.setValidity(true);
			int maxscore = -2;
			ArrayAndScore winner;
			for (list<ArrayAndScore>::iterator i = candidates.begin(); i != candidates.end(); i++) {
				if (i->getScore() > maxscore) {
					maxscore = i->getScore();
					winner = *i;
				}
			}

			aas.addMovements(winner);
			if (cache_movements.find(aas.getVector()) == cache_movements.end()) {
				cache_movements[aas.getVector()] = aas.getMovements();
			}

			if (aas.getSize() == K) {
				int score = winner.getScore();
				cout << "BEST CANDIDATE SCORE: " << score << endl;
				cout << "array: ";
				aas.printVector();
				aas.printMovements();
				cout << endl << endl;
			}
		}

	}
}


void getArrayAndScore_n2(ArrayAndScore &aas) {
	vector<int> localnumbers = aas.getVector();
	int a0 = localnumbers[0];
	int a1 = localnumbers[1];

	if (a0 == a1) {
		aas.setValidity(true);
		aas.insertMovement(a1, a0, '-', 0);
	}
	else {
		aas.setValidity(false);
	}
}
