// Generate 1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "fileNamePrinter.h"
#include "configReader.h"
#include "Route.h"

bool add(std::vector<Route> &routes, Route curRoute,int degree=0)
{
	if (curRoute.ifShadow())
		return false;

	curRoute.sortEdges();
	
	bool check = true;
	for (auto &route:routes)
	{
		if (route.compare(curRoute))
		{
			return false;
		}
	}
	
	routes.push_back(curRoute);
	return true;
}

void setDirections(std::vector<Point> &directions)
{
	directions.push_back(Point(1, 0));
	directions.push_back(Point(1, 1));
	directions.push_back(Point(0, 1));
	directions.push_back(Point(-1, 1));
	directions.push_back(Point(-1, 0));
	directions.push_back(Point(-1, -1));
	directions.push_back(Point(0, -1));
	directions.push_back(Point(1, -1));
}

void generateAllRoutes(Route &curRoute, std::vector<Route> &res0, std::vector<Route> &res1, std::vector<Route> &res2,int max_edges, std::vector<Point> &directions)
{
	int curRouteSize = curRoute.getSize();
	if (curRouteSize < max_edges)
	{
		std::vector<int> vertDeg;
		std::vector<Point> points;
		curRoute.getDegrees(vertDeg, points);
		if(count(vertDeg.begin(),vertDeg.end(),1)<=2)
		{ 
			for (int i = 0; i < points.size(); i++)
			{
				for (int j = 0; j < directions.size(); j++)
				{
					if (curRouteSize == 1)
						std::cout << "Start new step, dir=" << j << "\n";

					curRoute.addEdge(Edge(points[i], points[i] + directions[j]));
					generateAllRoutes(curRoute, res0, res1, res2, max_edges, directions);
					curRoute.removeLastEdge();
				}
			}
		}
	}
	else
	{
		std::vector<int> vertDeg;
		std::vector<Point> points;
		curRoute.getDegrees(vertDeg, points);
		switch (count(vertDeg.begin(), vertDeg.end(), 1))
		{
			case 0:	add(res0, curRoute,0); break;
			case 1:	add(res1, curRoute,1); break;
			case 2:	add(res2, curRoute,2); break;
			default:
				break;
		}
	}
}

//service function
void string_to_graph(std::string s, std::vector<Edge> &res)
{
	std::istringstream in;
	char c;
	Point p1, p2;
	int x, y;
	int amount = (count(s.begin(), s.end(), ','))/3;
	in.str(s);
	for (int i = 0; i < amount; i++)
	{
			in >> c;
			in >> x;
			in >> c;
			in >> y;
			p1.set(x, y);
			in >> c;
			in >> c;
			in >> c;
			in >> x;
			in >> c;
			in >> y;
			in >> c;
			p2.set(x, y);
			res.push_back(Edge(p1, p2));
	}
}



int main()
{
	std::vector<Point> directions;

	int type;

	setDirections(directions);

	const int DiffDirections = 3;
	const int DiffTypes=3;
	std::vector<Route> res[DiffTypes];
	std::ofstream out_files[DiffTypes];
	
	Route curRoute;
	int curOrder = 1;

	configReader myConfigReader;
	myConfigReader.openConfigFile("input.txt");
	curOrder=myConfigReader.readIntWithHeader();
	myConfigReader.closeConfig();
	
	std::ostringstream fname;
	for (int curType = 0; curType < DiffTypes; curType++)
	{
		std::string fileName = fileNamePrinter::getPathToBasicGeneralRoutesInfo(curOrder, std::to_string(curType));
		(out_files[curType]).open(fileName, std::ios::out);
	}
	
	for (int i = 0; i < DiffDirections; i++)
	{
		curRoute.clear();
		curRoute.addEdge(Edge(Point(0, 0), directions[i]));
		generateAllRoutes(curRoute, res[0], res[1], res[2], curOrder, directions);
	}


	for (int i = 0; i < DiffTypes; i++)
	{
		for (int j = 0; j < (res[i]).size(); j++)
		{
			res[i][j].writeRouteToFile(out_files[i]);
		}
	}
	return 0;
}

