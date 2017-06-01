//Edit 05.02.2017 First changes from singlet version
//                Change order of basis order
//				  getE0()added
//				  output files names are changed


//edit 08.02.2017 change N->node_num in all "act_*" functions

//edit 08.02.2017  change vIn: read proper file for each route

//edit 09.02.2017 struct states: change array to vector, all act procedures received init for "tempst"

// MainConveyor.cpp : Defines the entry point for the console application.
//

#pragma once
#include "stdafx.h"
#include "JFactors.h"
#include "MatrixOfResults.h"
#include "Results.h"
#include "MyFunctions.h"
#include "WFTransformer.h"
#include "fileNamePrinter.h"

std::ofstream logfile("log.txt", std::ios::out);

std::vector<edge> edges;//ребра текущего маршрута
std::vector<point> nodes;//вершины текущего маршрута

WaveFunction *ref1,*ref2;
int nodeNumsOfEdges[N][2];


int main(int argc, char* argv[])
{
	//запуск нескольких маршрутов
	int startRouteNum,finRouteNum;
	std::ifstream inproutesNums("input.txt", std::ios::in);
	inproutesNums>>startRouteNum>>finRouteNum;
	inproutesNums.close();

	///ѕроверка на допустимость размеров
	if(3*(N)*(int)pow((double)2,N-1)>Namount)
	{
		logfile<<"Change Namount";
        logfile.close();
		return 1;
	}

	std::stringstream fname;
	std::string fileNameString;
	std::stringstream sscanner;
	
	std::ifstream config("config.txt", std::ios::in);//хранит текущий пор€док и кол-во маршрутов всех длин, начина€ с 2
	std::ifstream terms;//слагаемые р€да теории возмущений
	std::ifstream operatorsset;//ненулевые наборы операторов
	std::ofstream matrixRes;  //выходные файлы

	int inpOrder; //temp var for file reading
	int type, Order, subOrder; //“ип маршрутов
	int routesAmount [3][N+1];
	std::vector<int*> nodeSets;
	int *termOrder;
	int edgeNum;//число ребер в маршруте
	int nodeNum;//число вершин в маршруте
	int r[20][2]; //2*10 - на 10 ребер с запасом

	std::string outString;
	std::string strType;
	std::string s; //reading from file

	//std::vector<int> goodNotLoopNums;


	getline(config,outString);//—читываем заголовок - он не важен
	config>>type>>Order>>subOrder;
	if(type>=0&&type<3)
        strType=StrType[type];
	else
	{
		logfile << "Wrong type!";
		logfile.close();
		return 2;
	}
    	
	getline(config,outString);//—читываем заголовок - он не важен
	getline(config,outString);//—читываем заголовок - он не важен

	for(int i=1;i<=N;i++)
	{
        config>>inpOrder>>routesAmount[0][i]>>routesAmount[1][i]>>routesAmount[2][i];
	}
	


	//Test input data

	if(Order>N)
	{
		logfile<<"\nWrong Const or Config file\n";
		logfile.close();
		int tmpVar;
		std::cout<<"!!!!!!!!!!!!!!!!!!";
		std::cin>>tmpVar;
		return 3;
	}

	//End Test input data
	//////////////////////////////////////////////////////////////////////////
	

	JFactors::setOrder(Order);
	WFTransformer mainTransformer;


	

	//считываем все слагаемые дл€ данного пор€дка
	//отдельно дл€ первого пор€дка
	if (Order == 1)
	{
		termOrder = new int[1];
		termOrder[0] = -1;
		nodeSets.push_back(termOrder);
	}
	else//дл€ всех остальных
	{
		sscanner.str("");
		sscanner << InputDirFinalData << Delim << Order << "order.txt\0";
		fileNameString = sscanner.str();

		terms.open(fileNameString.c_str(), std::ios::in);
		while (!terms.eof())
		{
			getline(terms, s);
			sscanner.str(s);
			if (s.length() > 0)
			{
				termOrder = new int[Order - 1];
				for (int i = 0; i < Order - 1; i++)
				{
					sscanner >> termOrder[i];
				}
				nodeSets.push_back(termOrder);
			}
		}
	}
	int *curOperatorSet;

////„тение ћатриц
//	//чтение матриц из файлов
//	std::ifstream in;
//	s="";
//	for( int i=0;i<SpinsInPlaquet;i++)
//	{
//		std::ostringstream tmp;
//		tmp<<(i+1);
//		s=InputPrefixForMatrixes+"noutp_s"+tmp.str()+".txt"; //Sp
//		in.open(s.c_str(), std::ios::in);
//		for(int j=0;j<DiffStates;j++)
//		{
//			for(int k=0;k<DiffStates;k++)
//			{
//				in>>Vmatrix[i][0][j][k];
//			}
//		}
//		in.close();
//
//		s=InputPrefixForMatrixes+"noutm_s"+tmp.str()+".txt"; //Sm
//		in.open(s.c_str(), std::ios::in);
//		for(int j=0;j<DiffStates;j++)
//		{
//			for(int k=0;k<DiffStates;k++)
//			{
//				in>>Vmatrix[i][1][j][k];
//			}
//		}
//		in.close();
//
//		s=InputPrefixForMatrixes+"noutz_s"+tmp.str()+".txt"; //z=> второй индекс 2
//		in.open(s.c_str(), std::ios::in);
//		for(int j=0;j<DiffStates;j++)
//		{
//			for(int k=0;k<DiffStates;k++)
//			{
//				in>>Vmatrix[i][2][j][k];
//			}
//		}
//		in.close();
//	}
//
//	in.open((InputPrefixForMatrixes+"inside_matr.txt").c_str(), std::ios::in);
//	for(int i=0;i<DiffStates;i++)
//		for(int j=0;j<DiffStates;j++)
//		{
//			in>>VmatrixInside[i][j];
//		}
//	in.close();
//
//	in.open((InputPrefixForMatrixes+"energy.txt").c_str(), std::ios::in);
//	for(int i=0;i<DiffStates;i++)
//	{
//		in>>Energie[i];
//	}
//// онец „тени€ ћатриц

	mainTransformer.vOperator.readMatrixAndEnergie();

	State init;//в будущем дл€ четни€
	//Initializing input and output arrays of WF
	WaveFunction *wfIn,*wfOut1,*wfOut2;
	WaveFunction wfTemp1, wfTemp2;
	int vecAmount = 3 * (Order + 1)*(int)pow((double)2, Order);
	wfIn = new WaveFunction[vecAmount];
	wfOut1 = new WaveFunction[vecAmount];
	wfOut2 = new WaveFunction[vecAmount];


	
	//»нициализируем матрицу результатов
	MatrixOfResults fullMatrix;
	fullMatrix.clearAndSetSize(vecAmount);
	// онец инициализации;


	curOperatorSet=new int[Order];
	int *procedureOrder=new int[Order];
	int *powerOrder=new int[Order];
	int realSize;

	for(int i=subOrder;i<=subOrder;i++)//ѕеребираем все возможные длины маршрутов
	{
		for(int j=1;j<=routesAmount[type][subOrder];j++)//перебираем все доступные маршруты при данной длине
		{
			//Ѕлок управлени€ различными копи€ми, позвол€ет запускать только часть маршрутов
			if(j>finRouteNum)//если все вычислили, то выходим.
				exit(0);
			if(j<startRouteNum)
				continue;//пропускаем если не подходит по заданным номерам
			// онец Ѕлока управлени€ различными копи€ми

			sscanner.str("");
			sscanner<<InputPrefixForRroutes<<Order<<"_"<<strType<<Delim<<Order<<"_"<<i<<"_"<<j<<"_routeNum_"<<strType<<".txt";
			s=sscanner.str();
			operatorsset.open(s.c_str(), std::ios::in);
			getline(operatorsset,s);
			std::istringstream route;
			route.str(s);
			
			operatorsset >> edgeNum >> nodeNum;
			//если это не случай внутреннего оператора, то считываем ребра
			if (edgeNum > 0)
			{
				read_Route(r, route);
				eval_cur_route(r, Order, i, edges, nodeNumsOfEdges, nodes, edgeNum);

				//ѕроверка на ошибку ребер
				if (edges.size() != edgeNum)
				{
					logfile << "Edges wrong. n=" << Order << " i=" << i << " j=" << j << " " << strType;
					logfile.close();
					return 4;
				}
				// онец проверки на ошибку в ребрах

				mainTransformer.setInteractions(nodeNumsOfEdges, edges);
				/*for (int ll = 0; ll < edgeNum; ll++)
				{
					returnV(curInter, interAmount, ll, nodeNumsOfEdges[ll][0], nodeNumsOfEdges[ll][1], edges[ll].x2 - edges[ll].x1, edges[ll].y2 - edges[ll].y1);
				}*/
			}

			//заполн€ем спины каждый раз
			sscanner.str("");
			sscanner << "spins" << Delim << nodeNum << "spins_order.txt";
			std::ifstream inStates((sscanner.str()).c_str(), std::ios::in);

			
			
			int curState;

			for (int i = 0; i<vecAmount; i++)
			{
				wfIn[i].clear();
				init.clear();
				for (int j = 0; j<nodeNum; j++) 
				{
					inStates >> curState; 
					init.addClusterState(curState);
				}
				wfIn[i].addEigenState(init);
			}
			inStates.close();
			//конец заполнени€ спинов

			

			getline(operatorsset,s);
			realSize=3*nodeNum*(int)pow((double)2,nodeNum-1);
			
			fullMatrix.clearAndSetSize(realSize);
			 
			int zz=0;		
			while(!operatorsset.eof())
			{
				getline(operatorsset,s);

				if(s.length()>0)
				{

					std::cout<<Order<<" "<<i<<" "<<j<<" zz="<<zz<<"\n";

					zz++;

					sscanner.str(s);
					for(int k=0;k<Order;k++)
					{
						sscanner>>curOperatorSet[k];//считываем n операторов вдоль маршрута
					}

					for(unsigned int k=0;k<nodeSets.size();k++)
					{

						for(int ll=0;ll<realSize;ll++)//очищаем выходные данные
						{
							wfOut1[ll].clear();
							wfOut2[ll].clear();
						}
						
						generate_procedure_order(nodeSets[k],curOperatorSet,edgeNum,Order,procedureOrder,powerOrder);

						for(int ll=0;ll<realSize;ll++)//вычисл€ем хвосты. начало
						{
							ref1=&wfIn[ll];
							ref2=&wfTemp1;
							for(int mm=0;mm<(Order+1)/2;mm++)
							{
								if(mm==(Order+1)/2-1)//если осталс€ последний шаг
									ref2=&wfOut1[ll];
								if(procedureOrder[mm]<=4)//выбираем процедуру
								{
									mainTransformer.act(*ref1, *ref2, curOperatorSet[mm], procedureOrder[mm], powerOrder[mm]);
								}
								else
								{
									mainTransformer.actInside(*ref1, *ref2, curOperatorSet[mm] - edgeNum, procedureOrder[mm], powerOrder[mm]);
								}
								if(ref2==&wfTemp1)
								{
									ref2=&wfTemp2;
									ref1=&wfTemp1;
									wfTemp2.clear();
								}
								else
								if(ref2==&wfTemp2)
								{
									ref2=&wfTemp1;
									ref1=&wfTemp2;
									wfTemp1.clear();
								}
							}
						}

						///¬ычисл€ем замыкающие хвосты

						for(int ll=0;ll<realSize;ll++)
						{
							ref1=&wfIn[ll];
							ref2=&wfTemp1;
							//дл€ случа€ первого пор€дка просто копируем
							if (Order == 1)
							{
								mainTransformer.actCopy(wfIn[ll], wfOut2[ll]);
							}
							//дл€ всех остальных пор€дков
							for(int mm=Order-1;mm>Order-1-Order/2;mm--)
							{
								if(mm==Order-Order/2)//если осталс€ последний шаг
									ref2=&wfOut2[ll];
								
								if (procedureOrder[mm] <= 4)
								{
									mainTransformer.act(*ref1, *ref2, curOperatorSet[mm], procedureOrder[mm], powerOrder[mm]);
								}
								else
								{
									mainTransformer.actInside(*ref1, *ref2, curOperatorSet[mm] - edgeNum, procedureOrder[mm], powerOrder[mm]);
								}
								

								if(ref2==&wfTemp1)
								{
									ref2=&wfTemp2;
									ref1=&wfTemp1;
									wfTemp2.clear();
								}else
								if(ref2==&wfTemp2)
								{
									ref2=&wfTemp1;
									ref1=&wfTemp2;
									wfTemp1.clear();
								}
							}
						}

						for(int x=0;x<realSize;x++)
						{
							for(int y=0; y<realSize;y++)
							{
								Results tmpres;
									
								tmpres.scalarProduct(wfOut1[x], wfOut2[y]);

								if(minus1(nodeSets[k],Order)==-1)
									tmpres.minus();
								fullMatrix.add(x, y, tmpres);
							}
						}
					}
				}
			}
						
			//печать матрицы
			fullMatrix.printMatrix(fileNamePrinter::getPathToResMatrixFiles(strType, Order, i, j),Order);
			operatorsset.close();
		}
	}

	delete[] curOperatorSet;

	logfile.close();

	return 0;
}

