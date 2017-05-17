//Modifications:
// 13.05.2017 

//global include
#include "stdafx.h"
#include "globalConsts.h"
#include "configReader.h"
#include "LadderOperator.h"
#include "Term.h"
#include "fileNamePrinter.h"

//local include
//#include "localConsts.h"
#include "Summator.h"

class OutputFilePreparator
{
	int ladderOpLength_;

public:
	OutputFilePreparator(int _ladderOpLength)
	{
		ladderOpLength_ = _ladderOpLength;
	}


	void  openFile(std::ofstream &outResFile, int order, std::string point)
	{
		if (outResFile)
			outResFile.close();
		outResFile.open(fileNamePrinter::getPathToSumOfTermsNameByOrderAndPoint(point,order,ladderOpLength_), std::ios::out);
		outResFile.setf(std::ios::fixed);
		outResFile << std::setprecision(10);
	}

};

void sumTerms(int order, int ordernums[][RouteTypeAmount], std::string curPoint, int opLength,const std::string inputRoutesPath)
{
	std::ostringstream str;
	std::string s;
	std::string curResFileName;
	std::ifstream termsAndNodes;
	std::ifstream resFile;
	
	int curRouteNum;
	int subOrder;

	Term curTerm;
	Summator summator(inputRoutesPath, opLength);

	for (int type = 0; type < RouteTypeAmount; type++)
	{
		curRouteNum = 1;
		subOrder = minSubOrder[type];
		
		//for type zero fisrt suborder exists only in 1st order
		if (type == 0 && order > 1 && subOrder == 1)
			subOrder++;

		while (subOrder <= order)
		{
			
			termsAndNodes.open(fileNamePrinter::getPathToGeneralRoutesInfo(subOrder, TypeStr[type]), std::ios::in);
			
			while (!termsAndNodes.eof())
			{
				getline(termsAndNodes, s);
				if (s.length() > 0)
				{
					if (s[0] == 'n')//new route
					{
						summator.parseNewRoute(s);
						//str.str("");
						//str << inputRoutesPath << curPoint << Delimiter << "res" << TypeStr[type] << "$" << order << "$" << subOrder << "$" << curRouteNum++ << "_" << curPoint << ".txt";
						curResFileName = fileNamePrinter::getPathToResOfSystemSolving(curPoint,order,subOrder,curRouteNum, TypeStr[type]);
						curRouteNum++;
					}
					else //new set of nodes for current route
					{
						summator.parseNodes(s,true);
						s = "";
						resFile.open(curResFileName, std::ios::in);
						while (!resFile.eof())
						{
							getline(resFile, s);
							if (s.length() > 0)
								summator.addTerm(s);
						}
						resFile.close();
					}
				}
			}
			subOrder++;
		}
	}

	std::ofstream out;
	OutputFilePreparator ofp(opLength);
	ofp.openFile(out, order, curPoint);
	summator.print(out);
	out.close();

}



int _tmain(int argc, _TCHAR* argv[])
{	
	int pointsAmount, order;
	int orderNums[MaxOrder][RouteTypeAmount];
	std::vector<std::string> points;
	
	std::string inputPathToResFiles;

	int aOperators;

	configReader myConfigReader;

	myConfigReader.openConfigFile("config.txt");
	
	inputPathToResFiles =myConfigReader.readStrFromConfig();//path to input files with solutions of system
	order = myConfigReader.readIntWithHeader();//max order of current run
	aOperators= myConfigReader.readIntWithHeader();//max of ladder operators in output term
	
	myConfigReader.readRouteAmounts(orderNums,1,order);
	
	myConfigReader.closeConfig();
		
	pointsAmount=myConfigReader.readPoints("points.txt",points);

	for (int point_num = 0; point_num<pointsAmount; point_num++)
	{
		for (int curOrder = 1; curOrder <= order;curOrder++)
			sumTerms(curOrder, orderNums, points[point_num], aOperators, inputPathToResFiles);
	}
	return 0;
}

