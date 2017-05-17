#pragma once
#include "stdafx.h"
#include <sstream>
#include "globalConsts.h"
#include "fileNamePrinter.h"


std::string fileNamePrinter::getPathToGeneralRoutesInfo(int currentOrder, std::string typeName)
{
	std::ostringstream ostr;
	ostr << DirForResultsFromStage11 << currentOrder << "_uniq_routes_" << typeName << ".txt";
	return ostr.str();
}


//after srtage 2
std::string fileNamePrinter::getPathToResMatrixFiles(std::string routeType,int currentOrder, int currenSubOrder, int routeNum)
{
	std::ostringstream ostr;
	ostr << DirForResultsFromStage2 << "reults_" << routeType << Delimiter << currentOrder << "." << currenSubOrder << Delimiter << currentOrder << "_" << currenSubOrder << "_" << routeNum << "_res_" << routeType << ".txt";
	return ostr.str();
}

std::string fileNamePrinter::getPathToResOfSystemSolving(std::string curJ2Point, int curOrder,int curSuborder,int curRouteNum, std::string typeName)
{
	std::ostringstream ostr;
	//str << inputRoutesPath << curPoint << Delimiter << "res" << TypeStr[type] << "$" << order << "$" << subOrder << "$" << curRouteNum++ << "_" << curPoint << ".txt";
	ostr << DirForResultsFromStage35 << curJ2Point << Delimiter << "res" << typeName << "$" << curOrder << "$" << curSuborder << "$" << curRouteNum << "_" << curJ2Point << ".txt";
	return ostr.str();
}

//output for stage 4, input for stage 5
std::string fileNamePrinter::getPathToSumOfTermsNameByOrderAndPoint(std::string point, int order, int ladderOpAmount)
{
	std::ostringstream ostr;
	ostr << DirForResultsFromStage4 << point << Delimiter << order << "_results_" << point << "_" << ladderOpAmount << ".txt";
	return ostr.str();
}

//output for stage 5, final
std::string fileNamePrinter::getPathToFourierTransformOfTermsNameByPoint(std::string point,  int ladderOpAmount)
{
	std::ostringstream ostr;
	ostr << DirForResultsFromStage5 << point << Delimiter << "FourierSeries_" << point << "_" << ladderOpAmount << "operator(s).txt";
	return ostr.str();
}

