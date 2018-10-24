#pragma once
#include <vector>
#include "Movement.h"
#include <list>
#include <algorithm>
#include <iostream>

using namespace std;

class ArrayAndScore
{
public:
	vector<int> numbers;
	int score;
	list<Movement> movements;
	bool valid;

	ArrayAndScore() {};
	ArrayAndScore(vector<int> numbers);

	void setScore(int score);
	 int getScore() const;

	void insertMovement(int _op1, int _op2, char _operation, int _score);
	void insertMovement(Movement m);
	void addMovements(ArrayAndScore winner);
	void addMovements(list<Movement> _movements);

	list<Movement> getMovements();
	void printMovements();


	void sortNumbers();
	vector<int> getVector();
	int getSize();


	~ArrayAndScore();

	void setValidity(bool val) {
		this->valid = val;
	}
	bool isValid() {
		return this->valid;
	}

	void printVector();


};

