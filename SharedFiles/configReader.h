//iClass to read params from config files
#pragma once
#include <fstream>
#include <sstream>
#include "globalConsts.h"

class configReader
{
private:
	std::ifstream config;
	std::ifstream points;
	std::istringstream inp;
public:
	bool openConfigFile(std::string);
	void closeConfig();
	bool openPointsFile(std::string);
	void closePointsFile();
	int readIntWithHeader();
	int readNewlineInt();
	int readNextInt();
	
	std::string readStrFromConfig();
	
	int readPoints(std::vector<std::string> &pointsVec);
	int readPoints(std::string fname, std::vector<std::string> &pointsVec);

	void readRouteAmounts(int orderAmounts[][RouteTypeAmount],int minOrder=1,int maxOrder=1);
};

