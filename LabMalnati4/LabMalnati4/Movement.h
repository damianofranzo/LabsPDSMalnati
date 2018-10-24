#pragma once
class Movement
{
public:
	int score;
	int op1, op2;
	char operation;

	int getScore() { return score; }


	Movement(int _op1, int _op2, char _operation, int _score) : op1(_op1), op2(_op2), operation(_operation), score(_score) {};
	~Movement();
};

