#include "stdafx.h"
#include "ArrayAndScore.h"

ArrayAndScore::ArrayAndScore(vector<int> numbers)
{
	this->numbers = vector<int>{numbers};
	this->score = 0;
	this->setValidity(false);
	this->movements = list<Movement>{};
	//it has to be set then
}


ArrayAndScore::~ArrayAndScore()
{
	this->numbers.clear();
}

void ArrayAndScore::setScore(int score) {
	this->score = score;
}


int ArrayAndScore::getScore () const{
	return this->score;
}

vector<int> ArrayAndScore::getVector() {
	return this->numbers;
}

int ArrayAndScore::getSize() {
	return this->numbers.size();
}

void ArrayAndScore::sortNumbers() {
	std::sort(this->numbers.begin(), this->numbers.end());
}

void ArrayAndScore::insertMovement(int _op1, int _op2, char _operation, int _score) {
	Movement i = {_op1,_op2,_operation,_score};
	this->movements.insert(this->movements.end(), i);
	this->score+=_score;
}

void ArrayAndScore::insertMovement(Movement m) {
	this->movements.insert(this->movements.end(), m);
	this->score += m.getScore();
}

void ArrayAndScore::printVector() {
	for (int i : this->numbers) {
		cout << i << " ";
	}
	cout << endl;
}

void ArrayAndScore::addMovements(ArrayAndScore winner) {
	list<Movement> win_movements = winner.getMovements();

	for (list<Movement>::iterator i = win_movements.begin(); i != win_movements.end() ; i++)
	{
		(this->movements).insert((this->movements).end(), *i);
		this->score += i->getScore();

	}
}

void ArrayAndScore::addMovements(list<Movement> _movements) {
	for (list<Movement>::iterator i = _movements.begin(); i != _movements.end(); i++)
	{
		(this->movements).insert((this->movements).end(), *i);
		this->score += i->getScore();
	}
}


list<Movement> ArrayAndScore::getMovements() {
	return this->movements;
}

void ArrayAndScore::printMovements()
{
	for (list<Movement>::iterator i = this->movements.begin(); i != this->movements.end(); i++)
	{
		cout << i->op1 << " " << i->operation << " " << i->op2 << " single score: " << i->score << endl;
	}
}