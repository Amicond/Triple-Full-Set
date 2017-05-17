#pragma once
#include "stdafx.h"
#include "Term.h"

class Summator
{
	std::unordered_map<Term, double> results_;
	std::vector<int> curRouteNodes_;
	std::string inputRoutesPath_;
	int nodesInRoute_;
	int curRouteNumber_;
	int curTermLength_;
	static const double Treshold_;
	static const int AllLengthes_;
public:

	Summator(const std::string _inputRoutesPath, int _curTermLength = AllLengthes_);
	
	void parseNewRoute(std::string inp);
	
	void parseNodes(std::string inp, bool isUncut = false);
	
	void addTerm(std::string inp);
	
	void print(std::ofstream &out);
	
	void clear();
	
};

