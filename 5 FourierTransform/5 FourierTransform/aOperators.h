#ifndef __AOPERATORS_H_INCLUDED__  
#define __AOPERATORS_H_INCLUDED__
#include "stdafx.h" 
#include "Term.h"

class bilinearFourierLadderOperators
{
public:
	std::vector<char> types_; //a,b,c or d type
	std::vector<char> names_; //plus or minus
	std::vector<std::pair<int,int>> nodes_; //to calc shift
	double coefficient_;
	int order_;
	
	

	bool operator==(const bilinearFourierLadderOperators second) const;

	bool operator<(const bilinearFourierLadderOperators second) const;
	
	void convertTerm(Term &curTerm);

	void sort

	void printAterm(std::ofstream &F, int **m, int size, bool if_print_coeff = true)const;

	void clear();

};
#endif //end of __AOPERATORS_H_INCLUDED__