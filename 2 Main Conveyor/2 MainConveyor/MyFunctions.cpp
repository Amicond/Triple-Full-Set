#include "stdafx.h"
#include "MyFunctions.h"

/////////////////////////////// global variables /////////////////////////////////////////
//double Vmatrix[SpinsInPlaquet][3][DiffStates][DiffStates];//Номер вершины в плакете, тип матрицы SP,SM или SZ,номер ряда, номер столбца
//double VmatrixInside[DiffStates][DiffStates];//Специально для внутренних воздействий
//double Energie[DiffStates]; //Энергии состояний
//inter curInter [N][MaxIntElem]; //массив операторов взамодействий
//int interAmount[N];// кол-во эл-тов в каждом операторе

//std::vector<State> vIn[Namount],vOut1[Namount],vOut2[Namount]; //вектора состоянй-используем по очереди один как входной другой как выходной, на следующем шаге - наоборот
//std::vector<State> vOutTemp1,vOutTemp2;//Промежуточные вектора
//std::vector<State> temp,temp2;
//////////////////////////////////////////////////////////////////////////



////////////////////  point   //////////////////////////////////////////////////////

point::point(int x, int y)
{
	sx = x;
	sy = y;
}

bool point::operator==(const point& s2) const
{
	if ((sx == s2.sx) && (sy == s2.sy))
	{
		return true;
	}
	else
		return false;
}
//////////////////////////////////////////////////////////////////////////


////////////////////  res   //////////////////////////////////////////////////////
//res& res::operator+=(const res& tmp)
//{
//	for (int i = 0; i<ResAmount; i++)
//	{
//		factors[i] += tmp.factors[i];
//	}
//	return *this;
//}
//
//void res::minus()
//{
//	for (int i = 0; i<ResAmount; i++)
//		factors[i] *= -1;
//}

//////////////////////////////////////////////////////////////////////////


////////////////////  edge   //////////////////////////////////////////////////////

bool edge::operator==(const edge &e2) const
{
	if (((x1 == e2.x1) && (x2 == e2.x2) && (y1 == e2.y1) && (y2 == e2.y2)) || ((x1 == e2.x2) && (x2 == e2.x1) && (y1 == e2.y2) && (y2 == e2.y1)))
	{
		return true;
	}
	else
		return false;
}

void edge::set(int X1, int Y1, int X2, int Y2)
{
	x1 = X1;
	y1 = Y1;
	x2 = X2;
	y2 = Y2;
}

//////////////////////////////////////////////////////////////////////////





///////////////////////////////   functions ///////////////////////////////////////////
int minus1(int *nodeSet, int n) //вычисляем знак для текущего слагаемого в рду теории возмущения
{
	if (n == 1) return 1;//для первого порядка


	int res = 1;
	//считаем кол-во нулей
	for (int i = 0; i<n - 1; i++)
	{
		if (nodeSet[i] == 0)
			res *= -1;
	}
	return res;
}

void eval_cur_route(int r[][2], int OrderLength, int RouteLength, std::vector<edge> &edges, int nodeNumsOfEdges[N][2], std::vector<point> &nodes, int &RealLength)
//заполняет для данного маршрута список ребер и их количество
{
	for (int i = 0; i<N; i++)
	{
		nodeNumsOfEdges[i][0] = 0;
		nodeNumsOfEdges[i][1] = 0;
	}
	//Проверяем реальную длину маршрута+ строим маску - сколько раз каждое ребро должно встречаться
	int numberOfEdges[10];//число ребер
	edge cur;
	bool flag;
	RealLength = 0;//Реальное количество различных "ГЛАВНЫХ" операторов
	edges.clear();
	for (int i = 0; i<10; i++)
	{
		numberOfEdges[i] = 0;
	}
	for (int i = 0; i<2 * RouteLength; i += 2)//список ребер
	{
		cur.set(r[i][0], r[i][1], r[i + 1][0], r[i + 1][1]);
		flag = true;
		for (unsigned int j = 0; j<edges.size(); j++)
		{
			if (cur == edges[j])
			{
				flag = false;
				numberOfEdges[j]++;
			}
		}
		if (flag)
		{
			edges.push_back(cur);
			numberOfEdges[edges.size() - 1]++;
		}
	}
	nodes.clear();
	//nodes.push_back(point(r[0][0],r[0][1]));
	for (int i = 0; i<2 * RouteLength; i++)//вычисляем номера вершин
	{
		if (find(nodes.begin(), nodes.end(), point(r[i][0], r[i][1])) == nodes.end())
			nodes.push_back(point(r[i][0], r[i][1]));
	}
	for (unsigned int i = 0; i<edges.size(); i++)//заполняем номера в "nodes" начала и конца для каждого ребра из "edges"
	{
		nodeNumsOfEdges[i][0] = distance(nodes.begin(), find(nodes.begin(), nodes.end(), point(edges[i].x1, edges[i].y1)));
		nodeNumsOfEdges[i][1] = distance(nodes.begin(), find(nodes.begin(), nodes.end(), point(edges[i].x2, edges[i].y2)));
	}
	RealLength = edges.size();
}

bool check_cur_operator_set(bool &Res, int OrderLength, int RealLength, int *termorder, int *operatorSet, std::vector<edge> edges)//проверяем можкт ли быть не 0 по данной конфигурации
{
	int start = 0;
	int end;
	int mask[10];
	Res = true;
	bool last = false;
	bool ifFind;
	std::vector<point> nodes;
	point currentNode(0, 0);
	for (int i = 0; i<OrderLength; i++)
	{
		if (i == OrderLength - 1)
		{
			end = i;
			last = true;
		}
		if ((termorder[i] == 0) || (last == true))
		{
			end = i;//нашли группу
			for (int j = 0; j<10; j++)
				mask[j] = 0;
			for (int j = start; j <= end; j++)
			{
				if ((unsigned int)operatorSet[j]<edges.size())//нашли ребро
				{
					currentNode.sx = edges[operatorSet[j]].x1; //выбираем его первую вершину
					currentNode.sy = edges[operatorSet[j]].y1;
					ifFind = false;
					for (unsigned int k = 0; k<nodes.size(); k++)//ищем ее
					{
						if (nodes[k] == currentNode)//если нашли увеличиваем ее кол-во
						{
							mask[k]++;
							ifFind = true;
						}
					}
					if (!ifFind)//если не нашли добавляем
					{
						nodes.push_back(currentNode);
						mask[nodes.size() - 1] = 1;
					}

					currentNode.sx = edges[operatorSet[j]].x2;//выбираем его 2ую вершину
					currentNode.sy = edges[operatorSet[j]].y2;
					ifFind = false;
					for (unsigned int k = 0; k<nodes.size(); k++)//ищем ее
					{
						if (nodes[k] == currentNode)//если нашли увеличиваем ее кол-во
						{
							mask[k]++;
							ifFind = true;
						}
					}
					if (!ifFind)//если не нашли добавляем
					{
						nodes.push_back(currentNode);
						mask[nodes.size() - 1] = 1;
					}
				}
			}

			for (int j = 0; j<OrderLength; j++)
			{
				if (mask[j] == 1)
				{
					Res = false;
					break;
				}
			}

			if (!Res)
				break;
			start = i + 1;
		}
		if (!Res)
			break;
	}
	return Res;
}

void read_Route(int r[][2], std::istringstream &s)
{
	char c = ' ';
	while (c != 'n')
	{
		s >> c;
	}
	s >> c;//пропускаем 1 символ
	int num = 0;
	while (N * 2 - 1 >= num)
	{

		s >> r[num][0];
		s >> c;
		s >> r[num][1];
		num++;

		s >> c;
		s >> c;
		s >> c;//пропускаем 3 символа
		s >> r[num][0];
		s >> c;
		s >> r[num][1];
		num++;
		s >> c;
		s >> c;
	}

}

void generate_procedure_order(int *termorder, int* operatororder, int edge_amount, int num, int *Res, int *power)
{
	//первый порядок отдельно
	if (num == 1)
	{
		power[0] = 0;
		if (edge_amount == 1)
			Res[0] = 2;
		else
			Res[0] = 7;
		return;
	}
	//общий случай для остальных порядков
	for (int i = 0; i<num; i++)
	{
		power[i] = 0;
	}
	for (int i = 0; i<(num + 1) / 2; i++)
	{
		if (termorder[i] == 0)//ground группа
		{
			if (operatororder[i]<edge_amount)//реберный оператор
				Res[i] = 2;
			else
				Res[i] = 7;
		}
		else if (termorder[i] == 1)//знаменатель в 1ой степени
		{
			if (operatororder[i] < edge_amount)
			{
				Res[i] = 3;
				power[i] = 1;
			}
			else
			{
				Res[i] = 6;
				power[i] = 1;
			}

		}
		else
		{
			if (operatororder[i]<edge_amount)
				Res[i] = 4;
			else
				Res[i] = 6;
			power[i] = termorder[i];
		}
	}

	//обратный ход
	for (int i = 0; i<(num / 2); i++)
	{
		if (termorder[num - i - 2] == 0)//ground группа
		{
			if (operatororder[num - i - 1]<edge_amount)//реберный оператор
				Res[num - i - 1] = 2;
			else
				Res[num - i - 1] = 7;
		}
		else if (termorder[num - i - 2] == 1)//знаменатель в 1ой степени
		{
			if (operatororder[num - i - 1] < edge_amount)
			{
				Res[num - i - 1] = 3;
				power[num - i - 1] = 1;
			}
			else
			{
				Res[num - i - 1] = 6;
				power[num - i - 1] = 1;
			}

		}
		else
		{
			if (operatororder[num - i - 1]<edge_amount)
				Res[num - i - 1] = 4;
			else
				Res[num - i - 1] = 6;
			power[num - i - 1] = termorder[num - i - 2];
		}
	}
	//самый последний элемент обратного хода без знаменателя
	if ((Res[num - num / 2] == 3) || (Res[num - num / 2] == 4))
		Res[num - num / 2] = 1;
	if (Res[num - num / 2] == 6)
		Res[num - num / 2] = 5;

}

//03.02.2014
//int find_last_group(std::vector<State> &cur, int startNumber)//возвращает номер последнего элемента равного заданному, работает для отсортированных массивов
//														 ///Можно улучшить с помощью бинарного поиска
//{
//	int last = startNumber;
//	for (int i = startNumber + 1; i<cur.size(); i++)
//	{
//		if (cur[startNumber] == cur[i])
//		{
//			last = i;
//		}
//		else
//		{
//			break;
//		}
//	}
//
//	return last;
//
//}

//Results finalvalue4(std::vector<State> &v1, std::vector<State> &v2, int n)
//{
//	Results fv;
//	fv.clearForCurrentOrder(n);
//
//	unsigned int i1, i2;
//	int tmpres[JFactors::Npowers];
//	i2 = 0;// указывает текущую группу
//	i1 = 0;// указывает текущую группу
//	int last1, last2;
//
//	while ((i2<v2.size()) && (i1<v1.size()))
//	{
//
//		if (v2[i2] == v1[i1])//организуем суммирование
//		{
//			last1 = find_last_group(v1, i1);
//			last2 = find_last_group(v2, i2);
//			for (int i = i1; i <= last1; i++)
//			{
//				for (int j = i2; j <= last2; j++)
//				{
//					//складываем степени J-множителей
//					for (int ttt = 0; ttt<JFactors::getAmountOfPowers(); ttt++)
//					{
//						tmpres[ttt] = v1[i].getPowerByNumber(ttt) + v2[j].getPowerByNumber(ttt);
//					}
//
//					//преобразуем три степени в номер ячейки массива и заполняем ее нужным фактором
//					fv.addToFactor(JFactors::getNumberByPowers(tmpres),v1[i].getFactor()*v2[j].getFactor());
//
//				}
//			}
//			i1 = last1 + 1;
//			i2 = last2 + 1;
//		}
//		else if (v2[i2]<v1[i1])
//		{
//			i2 = 1 + find_last_group(v2, i2);//перешли к следующей группе
//		}
//		else
//		{
//			i1 = 1 + find_last_group(v1, i1);
//		}
//	}
//	return fv;
//}

//////////////////////////////////////////////////////////////////////////

//направления 0- (0,0); далее против часовой стрелки
//void returnV(inter curInter[][MaxIntElem],int interAmount[],int interN,int n1,int n2,int dx,int dy) //n1,n2 - порядковые номера соответствующих функций, dir -направление от первого плакета ко второму
//{
//
//	/*switch(dir)
//	{
//	case 0: dx=1;   dy=0;  break;
//	case 1: dx=1;   dy=1;  break;
//	case 2: dx=0;   dy=1;  break;
//	case 3: dx=-1;  dy=1;  break;
//	case 4: dx=-1;  dy=0;  break;
//	case 5: dx=-1;  dy=-1; break;
//	case 6: dx=0;   dy=-1; break;
//	case 7: dx=1;   dy=-1; break;
//	}*/
//	int q=-1;
//	if((dx==1)&&(dy==0))// по горизонтали направо
//	{
//		q++;
//		curInter[interN][q].Jtype=0;
//		curInter[interN][q].n1=n1; curInter[interN][q].n2=n2;
//		curInter[interN][q].v1=1;  curInter[interN][q].v2=0;
//
//		q++;
//		curInter[interN][q].Jtype=0;
//		curInter[interN][q].n1=n1; curInter[interN][q].n2=n2;
//		curInter[interN][q].v1=2;  curInter[interN][q].v2=3;
//
//		q++;
//		curInter[interN][q].Jtype=1;
//		curInter[interN][q].n1=n1; curInter[interN][q].n2=n2;
//		curInter[interN][q].v1=1;  curInter[interN][q].v2=3;
//
//		q++;
//		curInter[interN][q].Jtype=1;
//		curInter[interN][q].n1=n1; curInter[interN][q].n2=n2;
//		curInter[interN][q].v1=2;  curInter[interN][q].v2=0;
//
//	}
//	if((dx==1)&&(dy==1)) //по диагонали вправо вверх
//	{
//		q++;
//		curInter[interN][q].Jtype=1;
//		curInter[interN][q].n1=n1; curInter[interN][q].n2=n2;
//		curInter[interN][q].v1=1;  curInter[interN][q].v2=3;
//	}
//
//	if((dx==0)&&(dy==1))//строго вверх
//	{
//		q++;
//		curInter[interN][q].Jtype=0;
//		curInter[interN][q].n1=n1; curInter[interN][q].n2=n2;
//		curInter[interN][q].v1=0;  curInter[interN][q].v2=3;
//
//		q++;
//		curInter[interN][q].Jtype=0;
//		curInter[interN][q].n1=n1; curInter[interN][q].n2=n2;
//		curInter[interN][q].v1=1;  curInter[interN][q].v2=2;
//
//		q++;
//		curInter[interN][q].Jtype=1;
//		curInter[interN][q].n1=n1; curInter[interN][q].n2=n2;
//		curInter[interN][q].v1=0;  curInter[interN][q].v2=2;
//
//		q++;
//		curInter[interN][q].Jtype=1;
//		curInter[interN][q].n1=n1; curInter[interN][q].n2=n2;
//		curInter[interN][q].v1=1;  curInter[interN][q].v2=3;
//	}
//	if((dx==-1)&&(dy==1)) //по диагонали влево вверх
//	{
//		q++;
//		curInter[interN][q].Jtype=1;
//		curInter[interN][q].n1=n1; curInter[interN][q].n2=n2;
//		curInter[interN][q].v1=0;  curInter[interN][q].v2=2;
//	}//ок
//	if((dx==-1)&&(dy==0))//строго влево
//	{
//		q++;
//		curInter[interN][q].Jtype=0;
//		curInter[interN][q].n1=n1; curInter[interN][q].n2=n2;
//		curInter[interN][q].v1=0;  curInter[interN][q].v2=1;
//
//		q++;
//		curInter[interN][q].Jtype=0;
//		curInter[interN][q].n1=n1; curInter[interN][q].n2=n2;
//		curInter[interN][q].v1=3;  curInter[interN][q].v2=2;
//
//		q++;
//		curInter[interN][q].Jtype=1;
//		curInter[interN][q].n1=n1; curInter[interN][q].n2=n2;
//		curInter[interN][q].v1=0;  curInter[interN][q].v2=2;
//
//		q++;
//		curInter[interN][q].Jtype=1;
//		curInter[interN][q].n1=n1; curInter[interN][q].n2=n2;
//		curInter[interN][q].v1=3;  curInter[interN][q].v2=1;
//	}//ок
//	if((dx==-1)&&(dy==-1)) //по диагонали влево вниз
//	{
//		q++;
//		curInter[interN][q].Jtype=1;
//		curInter[interN][q].n1=n1; curInter[interN][q].n2=n2;
//		curInter[interN][q].v1=3;  curInter[interN][q].v2=1;
//	}//ок
//	if((dx==0)&&(dy==-1))//строго вниз
//	{
//		q++;
//		curInter[interN][q].Jtype=0;
//		curInter[interN][q].n1=n1; curInter[interN][q].n2=n2;
//		curInter[interN][q].v1=2;  curInter[interN][q].v2=1;
//
//		q++;
//		curInter[interN][q].Jtype=0;
//		curInter[interN][q].n1=n1; curInter[interN][q].n2=n2;
//		curInter[interN][q].v1=3;  curInter[interN][q].v2=0;
//
//		q++;
//		curInter[interN][q].Jtype=1;
//		curInter[interN][q].n1=n1; curInter[interN][q].n2=n2;
//		curInter[interN][q].v1=2;  curInter[interN][q].v2=0;
//
//		q++;
//		curInter[interN][q].Jtype=1;
//		curInter[interN][q].n1=n1; curInter[interN][q].n2=n2;
//		curInter[interN][q].v1=3;  curInter[interN][q].v2=1;
//	}//ок
//
//	if((dx==1)&&(dy==-1)) //по диагонали нараво вниз
//	{
//		q++;
//		curInter[interN][q].Jtype=1;
//		curInter[interN][q].n1=n1; curInter[interN][q].n2=n2;
//		curInter[interN][q].v1=2;  curInter[interN][q].v2=0;
//	}//ок
//	interAmount[interN]=q+1;
//
//}

//void collect(std::vector<State> &outvec, std::vector<State> &invec)
//{
//	int index=0;
//	sort(invec.begin(),invec.end());
//	outvec.push_back(invec[0]);
//	for(unsigned int i=1;i<invec.size();i++)
//	{
//		if(invec[i].check(outvec[index]))
//		{
//			outvec[index].addToFactor(invec[i].getFactor());
//		}
//		else
//		{
//			if(abs(outvec[index].getFactor())<=0.000000000000001)
//			{
//				outvec.pop_back();
//				--index;
//			}
//			++index;
//			outvec.push_back(invec[i]);
//		}
//	}
//}

///* #0 */void act_copy(std::vector<State> &inv, std::vector<State> &outv)//просто копирует состояние, нужно для более единообразной раоты в первом порядке
//{
//	for (auto a : inv)
//	{
//		outv.push_back(a);
//	}
//}
//
///* #1 */void act(std::vector<State> &inv, std::vector<State> &outv,double Vmatrix[SpinsInPlaquet][3][DiffStates][DiffStates], int interNumber, int node_num)
//{
//	State tempst(node_num);
//	int second_ort;
//	double curE,E0;
//
//	//initialisation
//	E0=getE0(node_num);
//	temp2.clear();
//	
//
//	//computation
//	for(unsigned int inSt=0;inSt<inv.size();inSt++)
//	{
//		for(int i=0;i<interAmount[interNumber];i++) //перебираем все эл-ты взаимодействия
//		{
//			//temp2.clear();
//			//х-матрицы
//			for(int ort=0;ort<3;ort++)
//			{
//				temp.clear();//очищаем временный массив состояний, важно для y и z компонент
//				for(int j=0;j<DiffStates;j++)//вычисляем результат воздействия 1ой сигма-
//				{
//					if(Vmatrix[curInter[interNumber][i].v1][ort][inv[inSt].getStateByNumber(curInter[interNumber][i].n1)][j]!=0)
//					{
//						//При 1ом проходе это верно, дальше надо умножать
//
//						tempst.setFactor(inv[inSt].getFactor()*Vmatrix[curInter[interNumber][i].v1][ort][inv[inSt].getStateByNumber(curInter[interNumber][i].n1)][j]);
//						if(ort==1||ort==0)//у-сигма матрица, то надо умножить на -1, только 1 раз
//							tempst.multiplicateFactor(0.5);
//						
//						//Копируем старые состояния, так как не знаем какое будет меняться
//						
//						tempst.copyStates(inv[inSt]);
//
//						//Меняем состояние
//						tempst.setStateByNumber(curInter[interNumber][i].n1, j);
//						
//						tempst.copyPowers(inv[inSt]);
//						//Меняем при действии только первой матрицей!!!
//						tempst.incPower(curInter[interNumber][i].Jtype);
//
//						temp.push_back(tempst);
//					}
//				}
//				switch(ort)
//				{
//				case 0: second_ort=1; break;
//				case 1: second_ort=0; break;
//				case 2: second_ort=2; break;
//				}
//				for(int k=0;k<temp.size();k++) //действуем на полученные состояния 2ой матрицей
//				{
//					for(int j=0;j<DiffStates;j++)
//					{
//						if(Vmatrix[curInter[interNumber][i].v2][second_ort][temp[k].getStateByNumber(curInter[interNumber][i].n2)][j]!=0)
//						{
//							
//							tempst.copyStates(temp[k]);
//							tempst.setStateByNumber(curInter[interNumber][i].n2,j);
//
//							curE = tempst.getEnergie(Energie);
//
//							if(curE!=E0)
//							{
//								tempst.setFactor(temp[k].getFactor()*Vmatrix[curInter[interNumber][i].v2][second_ort][temp[k].getStateByNumber(curInter[interNumber][i].n2)][j]);
//								
//								tempst.copyPowers(temp[k]);
//								
//								temp2.push_back(tempst);  //записываем в выходной вектор
//							}
//						}
//					}
//				}
//			}
//		}
//	}
//	//Сортируем и собираем выходной вектор
//	outv.clear();
//	if(temp2.size())
//		collect(outv,temp2);
//
//}
//
///* #2 */void act_ground(std::vector<State> &inv, std::vector<State> &outv,double Vmatrix[SpinsInPlaquet][3][DiffStates][DiffStates], int interNumber, int node_num)
//{
//	State tempst(node_num);
//	int second_ort;
//	double curE, E0;
//
//	//initialisation
//	E0 = getE0(node_num);
//	temp2.clear();
//	
//	//computation
//	for(int inSt=0;inSt<inv.size();inSt++)
//	{
//		for(int i=0;i<interAmount[interNumber];i++) //перебираем все эл-ты взаимодействия
//		{
//			//temp2.clear();
//			//х-матрицы
//			for(int ort=0;ort<3;ort++)
//			{
//				temp.clear();//очищаем временный массив состояний, важно для y и z компонент
//				for(int j=0;j<DiffStates;j++)//вычисляем результат воздействия 1ой сигма-
//				{
//					if(Vmatrix[curInter[interNumber][i].v1][ort][inv[inSt].getStateByNumber(curInter[interNumber][i].n1)][j]!=0)
//					{
//						//При 1ом проходе это верно, дальше надо умножать
//
//						tempst.setFactor(inv[inSt].getFactor()*Vmatrix[curInter[interNumber][i].v1][ort][inv[inSt].getStateByNumber(curInter[interNumber][i].n1)][j]);
//						if (ort == 1 || ort == 0)//у-сигма матрица, то надо умножить на -1, только 1 раз
//							tempst.multiplicateFactor(0.5);
//						
//						//Копируем старые состояния, так как не знаем какое будет меняться
//						
//						tempst.copyStates(inv[inSt]);
//						//Меняем состояние
//						tempst.setStateByNumber(curInter[interNumber][i].n1,j);
//
//						tempst.copyPowers(inv[inSt]);
//						//Меняем при действии только первой матрицей!!!
//						tempst.incPower(curInter[interNumber][i].Jtype);
//							
//
//						temp.push_back(tempst);
//					}
//				}
//				//if(sti==1)testout<<"temp size="<<temp.size()<<"\n";
//				switch(ort)
//				{
//				case 0: second_ort=1; break;
//				case 1: second_ort=0; break;
//				case 2: second_ort=2; break;
//				}
//				for(int k=0;k<temp.size();k++) //действуем на полученные состояния 2ой матрицей
//				{
//					for(int j=0;j<DiffStates;j++)
//					{
//						if(Vmatrix[curInter[interNumber][i].v2][second_ort][temp[k].getStateByNumber(curInter[interNumber][i].n2)][j]!=0)
//						{
//							
//							tempst.copyStates(temp[k]);
//
//							tempst.setStateByNumber(curInter[interNumber][i].n2, j);
//							
//							curE=tempst.getEnergie(Energie);
//							
//							if(curE==E0)
//							{
//								tempst.setFactor(temp[k].getFactor()*Vmatrix[curInter[interNumber][i].v2][second_ort][temp[k].getStateByNumber(curInter[interNumber][i].n2)][j]);
//
//								tempst.copyPowers(temp[k]);
//
//								temp2.push_back(tempst);  //записываем в выходной вектор
//							}
//						}
//					}
//				}
//			}
//		}
//	}
//	//Сортируем и собираем выходной вектор
//	outv.clear();
//	if(temp2.size())
//		collect(outv,temp2);
//}
//
///* #3 */void act_energy(std::vector<State> &inv, std::vector<State> &outv,double Vmatrix[SpinsInPlaquet][3][DiffStates][DiffStates],int interNumber, int node_num)
//{
//	State tempst(node_num);
//	int second_ort;
//	double curE, E0;
//
//	//initialisation
//	E0 = getE0(node_num);
//	temp2.clear();
//	
//	if (inv.size()>50)
//		temp2.reserve(30000);
//											 
//											 
//	//computation
//	for(int inSt=0;inSt<inv.size();inSt++)
//	{
//
//
//		for(int i=0;i<interAmount[interNumber];i++) //перебираем все эл-ты взаимодействия
//		{
//			//temp2.clear();
//			//х-матрицы
//			for(int ort=0;ort<3;ort++)
//			{
//				temp.clear();//очищаем временный массив состояний, важно для y и z компонент
//				for(int j=0;j<DiffStates;j++)//вычисляем результат воздействия 1ой сигма-
//				{
//					if(Vmatrix[curInter[interNumber][i].v1][ort][inv[inSt].getStateByNumber(curInter[interNumber][i].n1)][j]!=0)
//					{
//						//При 1ом проходе это верно, дальше надо умножать
//						tempst.setFactor(inv[inSt].getFactor()*Vmatrix[curInter[interNumber][i].v1][ort][inv[inSt].getStateByNumber(curInter[interNumber][i].n1)][j]);
//						if (ort == 1 || ort == 0)//у-сигма матрица, то надо умножить на -1, только 1 раз
//							tempst.multiplicateFactor(0.5);
//						
//						//Копируем старые состояния, так как не знаем какое будет меняться
//						tempst.copyStates(inv[inSt]);
//
//						//Меняем состояние
//						tempst.setStateByNumber(curInter[interNumber][i].n1,j);
//
//						tempst.copyPowers(inv[inSt]);
//						//Меняем при действии только первой матрицей!!!
//						tempst.incPower(curInter[interNumber][i].Jtype);
//
//						temp.push_back(tempst);
//					}
//				}
//				switch(ort)
//				{
//				case 0: second_ort=1; break;
//				case 1: second_ort=0; break;
//				case 2: second_ort=2; break;
//				}
//				for(int k=0;k<temp.size();k++) //действуем на полученные состояния 2ой матрицей
//				{
//					for(int j=0;j<DiffStates;j++)
//					{
//						if(Vmatrix[curInter[interNumber][i].v2][second_ort][temp[k].getStateByNumber(curInter[interNumber][i].n2)][j]!=0)
//						{
//							tempst.copyStates(temp[k]);
//							tempst.setStateByNumber(curInter[interNumber][i].n2,j);
//
//							curE = tempst.getEnergie(Energie);
//
//							if(curE!=E0)
//							{
//								tempst.setFactor(temp[k].getFactor()*Vmatrix[curInter[interNumber][i].v2][second_ort][temp[k].getStateByNumber(curInter[interNumber][i].n2)][j]/(E0-curE));
//
//								tempst.copyPowers(temp[k]);
//
//								temp2.push_back(tempst);  //записываем в выходной вектор
//							}
//
//						}
//					}
//				}
//			}
//		}
//	}
//
//	//Сортируем и собираем выходной вектор
//	outv.clear();
//	if(temp2.size())
//		collect(outv,temp2);
//
//}
//
///* #4 */void act_energy_power(std::vector<State> &inv, std::vector<State> &outv,int power,double Vmatrix[SpinsInPlaquet][3][DiffStates][DiffStates],int interNumber, int node_num)
//{
//	State tempst(node_num);
//	int second_ort;
//	double curE,E0;
//
//	//initialisation
//	E0 = getE0(node_num);
//	temp2.clear();
//	
//	if (inv.size()>50)
//		temp2.reserve(30000);
//
//	for(int inSt=0;inSt<inv.size();inSt++)
//	{
//		for(int i=0;i<interAmount[interNumber];i++) //перебираем все эл-ты взаимодействия
//		{
//			//temp2.clear();
//			//х-матрицы
//			for(int ort=0;ort<3;ort++)
//			{
//				temp.clear();//очищаем временный массив состояний, важно для y и z компонент
//				for(int j=0;j<DiffStates;j++)//вычисляем результат воздействия 1ой сигма-
//				{
//					if(Vmatrix[curInter[interNumber][i].v1][ort][inv[inSt].getStateByNumber(curInter[interNumber][i].n1)][j]!=0)
//					{
//						//При 1ом проходе это верно, дальше надо умножать
//						tempst.setFactor(inv[inSt].getFactor()*Vmatrix[curInter[interNumber][i].v1][ort][inv[inSt].getStateByNumber(curInter[interNumber][i].n1)][j]);
//						if (ort == 1 || ort == 0)//у-сигма матрица, то надо умножить на -1, только 1 раз
//							tempst.multiplicateFactor(0.5);
//						
//						//Копируем старые состояния, так как не знаем какое будет меняться
//						
//						tempst.copyStates(inv[inSt]);
//
//						//Меняем состояние
//						tempst.setStateByNumber(curInter[interNumber][i].n1,j);
//
//						tempst.copyPowers(inv[inSt]);
//						//Меняем при действии только первой матрицей!!!
//						tempst.incPower(curInter[interNumber][i].Jtype);
//
//						temp.push_back(tempst);
//					}
//				}
//				switch(ort)
//				{
//				case 0: second_ort=1; break;
//				case 1: second_ort=0; break;
//				case 2: second_ort=2; break;
//				}
//				for(int k=0;k<temp.size();k++) //действуем на полученные состояния 2ой матрицей
//				{
//					for(int j=0;j<DiffStates;j++)
//					{
//						if(Vmatrix[curInter[interNumber][i].v2][second_ort][temp[k].getStateByNumber(curInter[interNumber][i].n2)][j]!=0)
//						{
//							tempst.setFactor(temp[k].getFactor()*Vmatrix[curInter[interNumber][i].v2][second_ort][temp[k].getStateByNumber(curInter[interNumber][i].n2)][j]);
//
//							tempst.copyStates(temp[k]);
//
//							tempst.setStateByNumber(curInter[interNumber][i].n2,j);
//
//							curE=tempst.getEnergie(Energie);
//
//							if(curE!=E0)
//							{
//								tempst.copyPowers(temp[k]);
//								
//								tempst.setFactor(tempst.getFactor()/pow((E0-curE),power));
//								
//								temp2.push_back(tempst);  //записываем в выходной вектор
//							}
//
//						}
//					}
//				}
//			}
//		}
//	}
//	//Сортируем и собираем выходной вектор
//	outv.clear();
//	if(temp2.size())
//		collect(outv,temp2);
//
//}
//
///* #5 */void act_inside(std::vector<State> &inv, std::vector<State> &outv,int plaquetNumber, int node_num)
//{
//	State tempst(node_num);
//	double curE,E0;
//	
//	//initialisation
//	E0 = getE0(node_num);
//	temp.clear();
//	
//	for(int inSt=0;inSt<inv.size();inSt++)
//	{
//		for(int j=0;j<DiffStates;j++)
//		{
//			if(VmatrixInside[inv[inSt].getStateByNumber(plaquetNumber)][j]!=0)
//			{
//
//				tempst.copyStates(inv[inSt]);
//				
//				tempst.setStateByNumber(plaquetNumber,j);
//
//				curE = tempst.getEnergie(Energie);
//				
//				if(E0!=curE)
//				{
//
//					tempst.setFactor(inv[inSt].getFactor()*VmatrixInside[inv[inSt].getStateByNumber(plaquetNumber)][j]);
//
//					tempst.copyPowers(inv[inSt]);
//
//					tempst.incPower(2);
//
//					temp.push_back(tempst);
//				}
//
//			}
//		}
//	}
//	outv.clear();
//	if(temp.size())
//		collect(outv,temp);
//
//}
//
///* #6 */void act_inside_enrgy_power(std::vector<State> &inv, std::vector<State> &outv,int power,int plaquetNumber, int node_num)
//{
//	State tempst(node_num);
//	double curE,E0;
//
//	//initialisation
//	E0 = getE0(node_num);
//	temp.clear();
//	
//	for(int inSt=0;inSt<inv.size();inSt++)
//	{
//		for(int j=0;j<DiffStates;j++)
//		{
//			if(VmatrixInside[inv[inSt].getStateByNumber(plaquetNumber)][j]!=0)
//			{
//				
//				tempst.copyStates(inv[inSt]);
//				tempst.setStateByNumber(plaquetNumber,j);
//
//				curE = tempst.getEnergie(Energie);
//
//				if(E0!=curE)
//				{
//
//					tempst.setFactor(inv[inSt].getFactor()*VmatrixInside[inv[inSt].getStateByNumber(plaquetNumber)][j]/pow(double(E0-curE),power));
//
//					tempst.copyPowers(inv[inSt]);
//
//					tempst.incPower(2);
//
//					temp.push_back(tempst);
//				}
//
//			}
//		}
//	}
//	outv.clear();
//	if(temp.size())
//		collect(outv,temp);
//
//}
//
///* #7 */void act_inside_ground(std::vector<State> &inv, std::vector<State> &outv,int plaquetNumber, int node_num)
//{
//	State tempst(node_num);
//	double curE,E0;
//
//	//initialisation
//	E0 = getE0(node_num);
//	temp.clear();
//
//	
//	for(int inSt=0;inSt<inv.size();inSt++)
//	{
//		for(int j=0;j<DiffStates;j++)//перебираем все элементы текущей строки матрицы перехода
//		{
//			if(VmatrixInside[inv[inSt].getStateByNumber(plaquetNumber)][j]!=0)
//			{
//
//				tempst.copyStates(inv[inSt]);
//				tempst.setStateByNumber(plaquetNumber,j);
//
//				curE = tempst.getEnergie(Energie);
//
//				if(E0==curE)
//				{
//
//					tempst.setFactor(inv[inSt].getFactor()*VmatrixInside[inv[inSt].getStateByNumber(plaquetNumber)][j]);
//
//					tempst.copyPowers(inv[inSt]);
//
//					tempst.incPower(2);
//
//					temp.push_back(tempst);
//				}
//
//			}
//		}
//	}
//	outv.clear();
//	if(temp.size())
//		collect(outv,temp);
//
//}


