// 1.6 Generate Additional Not Loop.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "globalConsts.h"
#include "configReader.h"
#include "fileNamePrinter.h"
#include "matrixNodes.h"
#include "Route.h"



void readRouteFromFile(int currentOrder, std::string inpStr, Route &curRoute)
{
	curRoute.clear();
	char c;
	int x1, y1,x2,y2;
	std::istringstream strRoute;
	strRoute.str(inpStr);
	for (int i = 0; i<currentOrder; i++)
	{
		strRoute >> c >> c;
		strRoute >> x1 >> c >> y1;
		strRoute >> c >> c >> c;
		strRoute >> x2 >> c >> y2;
		curRoute.addEdge(Edge(x1,y1,x2,y2));
	}
}

void transformPoints(int transformationType, std::vector<Point> &inpPoints, Point shift, std::vector<Point> &outPoints)
//n задает прееобразование при котором полученный веткор совмещаетс€ с исходным, 
//l задает точку, которую надо  сдвинуть в точку (0,0)
//дл€ получени€ номера соответствующей  точки надо выполнить преобразовани€ координат в обратном пор€дке
{
	switch (transformationType)
	{
	case 0:
		for (int i = 0; i < inpPoints.size(); i++) //повернуть обратно на 180
		{
			outPoints.push_back(inpPoints[i]);
			outPoints[i] = outPoints[i] - shift;
		}
		break;
	case 1:
		for (int i = 0; i < inpPoints.size(); i++) //повернуть обратно на 180
		{
			outPoints.push_back(inpPoints[i]);
			outPoints[i].transformRotate();
			outPoints[i] = outPoints[i] - shift;
		}
		break;
	case 2://поворот+отражение
		for (int i = 0; i < inpPoints.size(); i++)
		{
			outPoints.push_back(inpPoints[i]);
			outPoints[i].transformMirrorY();
			outPoints[i] = outPoints[i] - shift;
		}
		break;
	case 3://поворот+отражение+поворот
		for (int i = 0; i<inpPoints.size(); i++)
		{
			outPoints.push_back(inpPoints[i]);
			outPoints[i].transformMirrorX();
			outPoints[i] = outPoints[i] - shift;
		}
	}
}

void nodesToNumbers(int NNN, std::vector<Point> &curPoints, matrixNodes &matr, std::vector<int> &outputNumbers)
//transfor coordinates to Numbers
{
	int s = matr.getSize() / 2;

	for (auto &curPoint: curPoints)
	{
		outputNumbers.push_back(matr.getElem(curPoint.getX() + s,curPoint.getY() + s));
	}
}

void generateAllVariants(int curOrder, Route &curentRoute, matrixNodes &matr, std::ofstream &out)
{
	std::vector<Route> sameShape;
	Route work;
	std::vector<std::vector<int>>  Nodes;
	std::vector<Point> intitialPoints, transformedPoints;//точки начального маршрутов
	std::vector<Point> shift;//преобразовани€ сдвига дл€ каждого маршрута
	std::vector<int> rotate;//преобразование поворота
	std::vector<int> degrees;//temp vector
	std::vector<bool> ok;


	Point curP;
	for (int i = 0; i<4; i++)
	{
		work.copy(curentRoute);
		
		switch (i)
		{
		case 1: work.rotate180(); break;
		case 2: work.rotate180(); work.mirrorVerticalAxis(); break;
		case 3: work.rotate180(); work.mirrorVerticalAxis(); work.rotate180(); break;
		}

		intitialPoints.clear();
		degrees.clear();
		work.getDegrees(degrees, intitialPoints);
			
		sameShape.push_back(work);
		shift.push_back(Point(0,0));
		rotate.push_back(i);
		ok.push_back(true);
			
	}
	for (int i = 0; i<sameShape.size(); i++)
		sameShape[i].sortEdges();

	for (int i = 0; i<sameShape.size() - 1; i++)
		for (int j = i + 1; j<sameShape.size(); j++)
		{
			if (ok[i] && ok[j])
				if (sameShape[i]== sameShape[j])
				{
					ok[j] = false;
				}
		}

	intitialPoints.clear();
	degrees.clear();
	curentRoute.getDegrees(degrees, intitialPoints);
	
	curentRoute.writeRouteToFile(out);

	std::vector<int> curNum;
	for (int i = 0; i<sameShape.size(); i++)
	{
		curNum.clear();
		transformedPoints.clear();
		if (ok[i])
		{
			transformPoints(rotate[i], intitialPoints, shift[i], transformedPoints);
			nodesToNumbers(curOrder, transformedPoints, matr, curNum);
			
			out << "i ";
			for (int j = 0; j<curNum.size(); j++)
				out << curNum[j] << " ";
			out << "\n";
		}
	}

}

void generateRouteOperators(Route curRoute, int OrderLength, int RouteLength, std::ofstream &outFile)
{
	//standard form: sort edge's nodes and then edges
	curRoute.sortEdges();

	//output of the route
	curRoute.writeRouteToFile(outFile);

	//ѕровер€ем реальную длину маршрута+ строим маску - сколько раз каждое ребро должно встречатьс€
	std::vector<Edge> uniqEdges;
	std::vector<int> num_ed;//число ребер

	std::vector<Point> nodes;
	std::vector<int> amountOfNodes;
	
	const int N = 10;
	
	
	int cur_edges[N];
	int cur_nums[N];//“екуща€ расстановка операторов в слагаемом
	Edge cur;
	bool flag;
	int RealLength = 0;//–еальное количество различных "√Ћј¬Ќџ’" операторов
	int RealNodeLength = 0; //–еальное количество различных вершин
	

	curRoute.getEdges(uniqEdges, num_ed);
	RealLength = uniqEdges.size();

	int OnesAmountInit = curRoute.getDegrees(amountOfNodes, nodes);
	RealNodeLength = nodes.size();

	
	int amount = pow((int)(RealLength + RealNodeLength), (int)(OrderLength));//¬ычисл€ем всевозможное количество растоновок операторов
	int factor = RealLength + RealNodeLength;//степень, на  которую делим
	int temp, j;
	outFile << RealLength << " " << RealNodeLength << "\n";

	for (int i = 0; i<amount; i++)
	{
		//ѕодготовка
		temp = i;
		for (j = 0; j<RealLength; j++)
			cur_edges[j] = 0;
		flag = true;
		//¬ычисл€ем по номеру пор€док операторов
		for (j = 0; j<OrderLength; j++)
		{
			cur_nums[j] = temp%factor;
			temp /= factor;
			if (cur_nums[j]<RealLength)
				cur_edges[cur_nums[j]]++;
		}

		//провер€ем конфигурацию
		for (j = 0; j<RealLength; j++)
		{
			if (num_ed[j]>cur_edges[j])
			{
				flag = false;
				break;
			}
		}
		//провер€ем, что нова€ конфигураци€ относитс€ к тому же типу, что и изначална€, дл€ этого считаем маску
		int cur_nodes_num[N];
		Point cur_n(0,0);
		int n=0;
		int OnesAmount=0; //количество вершин со степенью один, так как по ним классифицируютс€ типы маршрутов
		for (int i = 0; i < RealNodeLength; i++)
		{
			cur_nodes_num[i] = 0;
		}

		for (int i = 0; i < RealLength; i++)
		{
			cur_n.set(uniqEdges[i].getX1(), uniqEdges[i].getY1());
			n=distance(nodes.begin(), find(nodes.begin(), nodes.end(), cur_n)); //ищем вершину
			cur_nodes_num[n] += cur_edges[i]; //увеличиваем степень вершины на кол-во ребер
			cur_n.set(uniqEdges[i].getX2(), uniqEdges[i].getY2());
			n = distance(nodes.begin(), find(nodes.begin(), nodes.end(), cur_n)); //ищем вершину
			cur_nodes_num[n] += cur_edges[i]; //увеличиваем степень вершины на кол-во ребер
		}
		for (int i = 0; i < RealNodeLength; i++)
		{
			if (cur_nodes_num[i] == 1)
				OnesAmount++;
		}

		if (OnesAmount != OnesAmountInit)
			flag = false;

		//если, ок в файл
		if (flag)
		{
			for (j = 0; j < OrderLength; j++)
			{
				outFile << cur_nums[j] << " ";
			}
			outFile << "\n";
		}
	}

	outFile.close();
}

int _tmain(int argc, _TCHAR* argv[])
{
	int curOrder, subOrder, routeType;
	std::string temp;
	configReader myConfigReader;
	myConfigReader.openConfigFile("conf.txt");
	curOrder = myConfigReader.readIntWithHeader();
	subOrder = myConfigReader.readNextInt();
	routeType = myConfigReader.readNextInt();
	myConfigReader.closeConfig();
	 
	std::string type = TypeStr[routeType];
	
	
	std::ifstream in(fileNamePrinter::getPathToBasicGeneralRoutesInfo(subOrder,type), std::ios::in);
	std::string s;
	Route curRoute;
	
	
	int count = 1;

	matrixNodes myMatrix;
	myMatrix.fillMatrix(curOrder);
	
	std::ofstream out;
	if (curOrder == subOrder)
	{
		out.open(fileNamePrinter::getPathToGeneralRoutesInfo(subOrder, type), std::ios::out);
	}
	
	while (!in.eof())
	{
		getline(in, s);
		if ((s.length()>0) && (s[0] == 'n'))
		{
			readRouteFromFile(subOrder, s, curRoute);
			curRoute.sortEdges();

			if (curOrder == subOrder)
			{
				generateAllVariants(curOrder, curRoute, myMatrix, out);
			}
				
			std::ofstream cur_out(fileNamePrinter::getPathToRouteFile(curOrder,subOrder,count++,type), std::ios::out);
			generateRouteOperators(curRoute, curOrder, subOrder, cur_out);
			cur_out.close();
		}
	}
	out.close();
	return 0;
}

