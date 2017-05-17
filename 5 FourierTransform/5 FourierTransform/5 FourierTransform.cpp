// 9 Collect Third Terms.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"



//global
#include "globalConsts.h"
#include "configReader.h" 
#include "matrixNodes.h"
#include "fileNamePrinter.h"
#include "Term.h"

//local
#include "localConsts.h"
#include "aOperators.h"
#include "cos.h"

int** matrix;

int main(int argc, char * argv[])
{
	//config  vars
	int minOrder, maxOrder;
	std::vector<std::string> points;
	int ladderOpAmount;

	//config init
	configReader myConfReader;
	myConfReader.openConfigFile("config.txt");

	//read parameters
	minOrder=myConfReader.readIntWithHeader();
	maxOrder = myConfReader.readNextInt();
	ladderOpAmount = myConfReader.readIntWithHeader();

	//close config
	myConfReader.closeConfig();

	//read points
	myConfReader.readPoints("points.txt",points);
	

	//input file name
	std::string fName;
	//input string with term info
	std::string inputTerm;

	std::istringstream opParser;
	Term currentTerm;
	double value;

	for (auto &elem : points)
	{
		for (int curOrder = minOrder; curOrder <= maxOrder; curOrder++)
		{
			std::cout << elem<<" "<< curOrder << "\n";
			fName = fileNamePrinter::getPathToSumOfTermsNameByOrderAndPoint(elem, curOrder, ladderOpAmount);
			std::ifstream inputTermsFile(fName, std::ios::in);
			while (!inputTermsFile.eof())
			{
				inputTermsFile >> inputTerm;
				currentTerm.parseTerm(inputTerm);
				inputTermsFile >> value;
			}
		}
	}
	int i;
	
	

	

	return 0;
}

