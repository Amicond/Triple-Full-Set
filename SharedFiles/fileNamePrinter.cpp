#include "stdafx.h"
#include <sstream>
#include "globalConsts.h"
#include "fileNamePrinter.h"



//input data section
std::string fileNamePrinter::getPathToConfigFile()
{
	std::ostringstream ostr;
	ostr << ConfigFilePath;
	return ostr.str();
}

std::string fileNamePrinter::gePathToMatrixes(std::string matrixPrefix,int vertexNum)
{
	std::ostringstream ostr;
	ostr << DirForMatrixInput << MatrixVersionPrefix << matrixPrefix << vertexNum<< ".txt";
	return ostr.str();
}
std::string fileNamePrinter::gePathToInsideMatrix()
{
	std::ostringstream ostr;
	ostr << DirForMatrixInput << MatrixVersionPrefix << "inside_matr.txt";
	return ostr.str();
}

std::string fileNamePrinter::gePathToEnergies()
{
	std::ostringstream ostr;
	ostr << DirForMatrixInput << MatrixVersionPrefix << "energy.txt";
	return ostr.str();
}

std::string fileNamePrinter::getPathToPerturbationTerms(int order)
{
	std::ostringstream ostr;
	ostr << DirForPerturbationSeries << order << "order.txt";
	return ostr.str();
}

//stage 0 output
//stage 2 input
std::string fileNamePrinter::getPathToSpinsOrder(int order)
{
	std::ostringstream ostr;
	ostr << DirForResultsFromStage0 << order << "spins_order" << ".txt";
	return ostr.str();
}

//output for stage 1
//input for stage 1.1
std::string fileNamePrinter::getPathToBasicGeneralRoutesInfo(int currentOrder, std::string typeName)
{
	std::ostringstream ostr;
	ostr << DirForResultsFromStage1 << currentOrder << "_uniq_" << typeName << ".txt";
	return ostr.str();
}
//output for stage 1.1
//input for stage 2
std::string fileNamePrinter::getPathToRouteFile(int order, int subOrder, int routeNum, std::string routeType)
{
	std::ostringstream ostr;
	ostr << DirForResultsFromStage11 << order << "_" << routeType << Delimiter<< order<<"_"<<subOrder<<"_"<<routeNum<< "_routeNum_" << routeType << ".txt";
	return ostr.str();
}

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
	ostr << DirForResultsFromStage2 << "results_" << routeType << Delimiter << currentOrder << "." << currenSubOrder << Delimiter << currentOrder << "_" << currenSubOrder << "_" << routeNum << "_res_" << routeType << ".txt";
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

