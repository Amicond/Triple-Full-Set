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
//n ������ ��������������� ��� ������� ���������� ������ ����������� � ��������, 
//l ������ �����, ������� ����  �������� � ����� (0,0)
//��� ��������� ������ ���������������  ����� ���� ��������� �������������� ��������� � �������� �������
{
	switch (transformationType)
	{
	case 0:
		for (int i = 0; i < inpPoints.size(); i++) //��������� ������� �� 180
		{
			outPoints.push_back(inpPoints[i]);
			outPoints[i] = outPoints[i] - shift;
		}
		break;
	case 1:
		for (int i = 0; i < inpPoints.size(); i++) //��������� ������� �� 180
		{
			outPoints.push_back(inpPoints[i]);
			outPoints[i].transformRotate();
			outPoints[i] = outPoints[i] - shift;
		}
		break;
	case 2://�������+���������
		for (int i = 0; i < inpPoints.size(); i++)
		{
			outPoints.push_back(inpPoints[i]);
			outPoints[i].transformMirrorY();
			outPoints[i] = outPoints[i] - shift;
		}
		break;
	case 3://�������+���������+�������
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
	std::vector<Point> intitialPoints, transformedPoints;//����� ���������� ���������
	std::vector<Point> shift;//�������������� ������ ��� ������� ��������
	std::vector<int> rotate;//�������������� ��������
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

	//��������� �������� ����� ��������+ ������ ����� - ������� ��� ������ ����� ������ �����������
	std::vector<Edge> uniqEdges;
	std::vector<int> num_ed;//����� �����

	std::vector<Point> nodes;
	std::vector<int> amountOfNodes;
	
	const int N = 10;
	
	
	int cur_edges[N];
	int cur_nums[N];//������� ����������� ���������� � ���������
	Edge cur;
	bool flag;
	int RealLength = 0;//�������� ���������� ��������� "�������" ����������
	int RealNodeLength = 0; //�������� ���������� ��������� ������
	

	curRoute.getEdges(uniqEdges, num_ed);
	RealLength = uniqEdges.size();

	int OnesAmountInit = curRoute.getDegrees(amountOfNodes, nodes);
	RealNodeLength = nodes.size();

	
	int amount = pow((int)(RealLength + RealNodeLength), (int)(OrderLength));//��������� ������������ ���������� ���������� ����������
	int factor = RealLength + RealNodeLength;//�������, ��  ������� �����
	int temp, j;
	outFile << RealLength << " " << RealNodeLength << "\n";

	for (int i = 0; i<amount; i++)
	{
		//����������
		temp = i;
		for (j = 0; j<RealLength; j++)
			cur_edges[j] = 0;
		flag = true;
		//��������� �� ������ ������� ����������
		for (j = 0; j<OrderLength; j++)
		{
			cur_nums[j] = temp%factor;
			temp /= factor;
			if (cur_nums[j]<RealLength)
				cur_edges[cur_nums[j]]++;
		}

		//��������� ������������
		for (j = 0; j<RealLength; j++)
		{
			if (num_ed[j]>cur_edges[j])
			{
				flag = false;
				break;
			}
		}
		//���������, ��� ����� ������������ ��������� � ���� �� ����, ��� � ����������, ��� ����� ������� �����
		int cur_nodes_num[N];
		Point cur_n(0,0);
		int n=0;
		int OnesAmount=0; //���������� ������ �� �������� ����, ��� ��� �� ��� ���������������� ���� ���������
		for (int i = 0; i < RealNodeLength; i++)
		{
			cur_nodes_num[i] = 0;
		}

		for (int i = 0; i < RealLength; i++)
		{
			cur_n.set(uniqEdges[i].getX1(), uniqEdges[i].getY1());
			n=distance(nodes.begin(), find(nodes.begin(), nodes.end(), cur_n)); //���� �������
			cur_nodes_num[n] += cur_edges[i]; //����������� ������� ������� �� ���-�� �����
			cur_n.set(uniqEdges[i].getX2(), uniqEdges[i].getY2());
			n = distance(nodes.begin(), find(nodes.begin(), nodes.end(), cur_n)); //���� �������
			cur_nodes_num[n] += cur_edges[i]; //����������� ������� ������� �� ���-�� �����
		}
		for (int i = 0; i < RealNodeLength; i++)
		{
			if (cur_nodes_num[i] == 1)
				OnesAmount++;
		}

		if (OnesAmount != OnesAmountInit)
			flag = false;

		//����, �� � ����
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

