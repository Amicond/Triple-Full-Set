//set of functions to return standard filenames for future use of output files
#pragma once
#include <string>

class fileNamePrinter
{
public:
	//output name on stage 0
	//output name on stage 1
	//output name on stage 1.1
	static std::string getPathToGeneralRoutesInfo(int currentOrder, std::string typeName);
	//output name on stage 2
	static std::string getPathToResMatrixFiles(std::string routeType, int currentOrder, int currenSubOrder, int routeNum);
	
	//output name on stage 3.5, after Mathematica

	//input for stage 4
	static std::string getPathToResOfSystemSolving(std::string curJ2Point, int curOrder, int curSuborder, int curRouteNum, std::string typeName);

	//output name on stage 4
	static std::string getPathToSumOfTermsNameByOrderAndPoint(std::string point, int order,int ladderOpAmount);
	
	//output name on stage 5
	static std::string getPathToFourierTransformOfTermsNameByPoint(std::string point, int ladderOpAmount);

};