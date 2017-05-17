// Generate 1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <algorithm>
#include <sstream>
using namespace std;
const int NNN = 7;
//const int maxShift=1;//максимальное удаление от точки ноль ноль
//const int VertAmount=5;//количество вершин в маршрутах
//const int Mode=4;//Количество вершин со степенью 3
//const int duplicateMode=1; //Ребра берутся с повторениями = 2, без повторений = 1
struct point
{
	point()
	{
		x = 0;
		y = 0;
	}
	point(int x, int y)
	{
		this->x = x;
		this->y = y;
	}
	void set(int x, int y)
	{
		this->x = x;
		this->y = y;
	}
	bool operator==(const point &p2)const
	{
		if ((x == p2.x) && (y == p2.y))
			return true;
		else
			return false;
	}
	bool operator<(const point &p2)const
	{
		if (x<p2.x)
			return true;
		else if (x == p2.x)
			if (y<p2.y)
				return true;
			else
				return false;
		else
			return false;
	}
	friend const point operator+(const point &a, const point &b);
	friend const point operator-(const point &a, const point &b);

	int x, y;
};


const point operator+(const point &a, const point &b)
{
	return point(a.x + b.x, a.y + b.y);
}
const point operator-(const point &a, const point &b)
{
	return point(a.x - b.x, a.y - b.y);
}

struct edge //для хранения координат ребер
{
	int x1, y1, x2, y2;
	edge()
	{
		x1 = x2 = y1 = y2 = 0;
	}

	edge(int X1, int Y1, int X2, int Y2)
	{
		x1 = X1;
		y1 = Y1;
		x2 = X2;
		y2 = Y2;
	}
	edge(point p1, point p2)
	{
		x1 = p1.x;
		y1 = p1.y;
		x2 = p2.x;
		y2 = p2.y;
	}
	bool operator==(const edge &e2)const
	{
		if (((x1 == e2.x1) && (x2 == e2.x2) && (y1 == e2.y1) && (y2 == e2.y2)) || ((x1 == e2.x2) && (x2 == e2.x1) && (y1 == e2.y2) && (y2 == e2.y1)))
		{
			return true;
		}
		else
			return false;
	}


	bool operator<(const edge &e2)const
	{
		if (x1<e2.x1)
			return true;
		else if (x1 == e2.x1)
		{
			if (y1<e2.y1)
				return true;
			else if (y1 == e2.y1)
			{
				if (x2<e2.x2)
					return true;
				else if (x2 == e2.x2)
				{
					if (y2<e2.y2)
						return true;
					else
						return false;
				}
				else
					return false;
			}
			else
				return false;
		}
		else
			return false;
	}
	void set(int X1, int Y1, int X2, int Y2)
	{
		x1 = X1;
		y1 = Y1;
		x2 = X2;
		y2 = Y2;
	}
	void set(point p1, point p2)
	{
		x1 = p1.x;
		y1 = p1.y;
		x2 = p2.x;
		y2 = p2.y;
	}
	void norm()
	{
		point p1(x1, y1);
		point p2(x2, y2);
		if (p2<p1)
			set(p2, p1);
	}
	friend const edge operator-(const edge &e1, const point &p1);
	friend const edge operator+(const edge &e1, const point &p1);
};

const edge operator-(const edge &e1, const point &p1)
{
	return edge(e1.x1 - p1.x, e1.y1 - p1.y, e1.x2 - p1.x, e1.y2 - p1.y);
}
const edge operator+(const edge &e1, const point &p1)
{
	return edge(e1.x1 + p1.x, e1.y1 + p1.y, e1.x2 + p1.x, e1.y2 + p1.y);
}

//class Graph {
//	vector<edge> gr;
//public:
//	bool operator==(const Graph &g2)const
//	{
//		if (gr.size() != (g2.gr).size())
//			return false;
//		for (int i = 0; i<gr.size(); i++)
//			if (!(gr[i] == (g2.gr)[i]))
//				return false;
//		return true;
//	}
//
//	bool operator==(const vector<edge> &gr2)const
//	{
//		if (gr.size() != gr2.size())
//			return false;
//		for (int i = 0; i < gr.size(); i++)
//			if (!(gr[i] == gr2[i]))
//				return false;
//		return true;
//	}
//
//
//
//
//	bool compare(const Graph &g2)const
//	{
//		if (gr.size() != (g2.gr).size())
//			return false;
//		vector<edge> new_vec;
//		vector<point> cur_p;
//		for (int i = 0; i<(g2.gr).size(); i++)
//			new_vec.push_back((g2.gr)[i]);
//		bool res = true;
//		for (int j = 0; j<4; j++)//4 возможных положения маршрута
//		{
//			cur_p.clear();
//			generate_points(new_vec, cur_p);
//			for (int i = 0; i<cur_p.size(); i++)//перебираем точки сдвига
//			{
//				for (int k = 0; k<new_vec.size(); k++)//совмещаем точку i с точкой (0,0)
//				{
//					new_vec[k] = new_vec[k] - cur_p[i];
//					new_vec[k].norm();
//				}
//				sort(new_vec.begin(), new_vec.end());
//				if ((*this)==new_vec)
//				{
//					res = false;
//					break;
//				}
//
//				//Возвращаем вектор обратно
//				for (int k = 0; k<new_vec.size(); k++)
//					new_vec[k] = new_vec[k] + cur_p[i];
//			}
//			if (!res)
//				break;
//			if (j == 1)
//				mirror(new_vec);
//			else
//				rotate180(new_vec);
//		}
//		return !res;
//
//	}
//};
//class Results
//{
//	vector<vector<edge>> res;
//	vector<vector<edge>> res10p;  //предок 1-1
//	vector<vector<edge>> res10pp; //предок 1-2 за 2 шага
//	vector<vector<edge>> res11pp; //предок 1-2 за 2 шага
//	vector<vector<edge>> res12pp; //предок 1-2 за 2 шага
//	vector<vector<edge>> res11p;  //предок 1-2 за 1 шаг
//	int gr_type;
//public:
//	void add(vector<edge> &cur, int type)
//	{
//		if (gr_type == type)
//		{
//			switch (gr_type)
//			{
//			case 0:
//
//			}
//		}
//	}
//
//private:
//	void read()
//};


bool isEqual(vector<edge> &v1, vector<edge> &v2)
{
	if (v1.size() != v2.size())
		return false;
	for (int i = 0; i<v1.size(); i++)
		if (!(v1[i] == v2[i]))
			return false;
	return true;
}

void generate_points(vector<edge> &v1, vector<point> &vout)
{
	point p;
	for (int i = 0; i<v1.size(); i++)
	{
		p.set(v1[i].x1, v1[i].y1);
		if (find(vout.begin(), vout.end(), p) == vout.end())
			vout.push_back(p);
		p.set(v1[i].x2, v1[i].y2);
		if (find(vout.begin(), vout.end(), p) == vout.end())
			vout.push_back(p);
	}

}

void rotate180(vector<edge> &in)
{
	for (int i = 0; i<in.size(); i++)
	{
		in[i].x1 *= -1;
		in[i].x2 *= -1;
		in[i].y1 *= -1;
		in[i].y2 *= -1;
	}
}

void mirror(vector<edge> &in)
{
	for (int i = 0; i<in.size(); i++)
	{
		in[i].x1 *= -1;
		in[i].x2 *= -1;
	}
}
bool if_shadow(vector<edge> &cur)
{
	vector<edge> uniqEdges;
	vector<int> amountEdges;
	bool checkE;
	for (int i = 0; i<cur.size(); i++)
	{
		cur[i].norm();
		checkE = true;
		for (int j = 0; j<uniqEdges.size(); j++)
		{
			if (uniqEdges[j] == cur[i])
			{
				amountEdges[j]++;
				checkE = false;
				break;
			}
		}
		if (checkE)
		{
			amountEdges.push_back(1);
			uniqEdges.push_back(cur[i]);
		}
	}
	bool check = false; //пусть парных ребер нет
	for (int i = 0; i<amountEdges.size(); i++)
		if (amountEdges[i]>2)
		{
			check = true;
			break;
		}
	if (!check)
		return false;


	vector<point> points;
	vector<int> amount;

	bool isMulti;
	for (int i = 0; i<uniqEdges.size(); i++) //исключаем i ребро
	{
		points.clear();
		amount.clear();
		if (amountEdges[i]>1)
		{
			isMulti = true;
			for (int j = 0; j<cur.size(); j++)
			{
				if ((uniqEdges[i] == cur[j]) && isMulti == true)
				{
					isMulti = false;//следующие такие же ребра не надо выкидывать, только 1
					continue;
				}
				bool exists;

				exists = false;
				for (int k = 0; k<points.size(); k++)
				{
					if (points[k] == point(cur[j].x1, cur[j].y1))//нашли
					{
						amount[k]++;
						exists = true;
						break;
					}
				}
				if (!exists)
				{
					points.push_back(point(cur[j].x1, cur[j].y1));
					amount.push_back(1);
				}
				//то же самое для конца ребра
				exists = false;
				for (int k = 0; k<points.size(); k++)
				{
					if (points[k] == point(cur[j].x2, cur[j].y2))//нашли
					{
						amount[k]++;
						exists = true;
						break;
					}
				}
				if (!exists)
				{
					points.push_back(point(cur[j].x2, cur[j].y2));
					amount.push_back(1);
				}
			}
			check = true;//предполагаем, что ребро необязательно
			for (int j = 0; j<amount.size(); j++)
			{
				if (amount[j] == 1)
				{
					check = false;
					break;
				}
			}
			if (check)
			{
				break;
			}
		}
	}
	return check;
}

bool if_shadow2(vector<edge> cur,int degree=0)//true- Значит уже встречалось в предыдущих порядках
{
	vector<int> duplicate_index;
	vector<int> amount;
	vector<point> points;
	for (int i = 0; i<cur.size(); i++)
	{
		cur[i].norm();
		duplicate_index.push_back(-1);
	}
	for (int i = 0; i<cur.size(); i++)
	{
		for (int j = i + 1; j<cur.size(); j++)
		{
			if (cur[i] == cur[j])
			{
				if (duplicate_index[i] == -1)
				{
					duplicate_index[i] = i;
					if (duplicate_index[j] == -1)duplicate_index[j] = i;
				}
				else
				{
					duplicate_index[j] = duplicate_index[i];
				}
			}
		}
	}
	//Начинаем откидывать ребра с duplicate_index[i]==i
	bool exists;
	int amountOf1 = 0;
	for (int i = 0; i<cur.size(); i++)
	{
		amount.clear();
		points.clear();
		if (duplicate_index[i] == i)
		{
			for (int j = 0; j<cur.size(); j++)
			{
				if (i == j)
					continue;
				////


				exists = false;
				for (int k = 0; k<points.size(); k++)
				{
					if (points[k] == point(cur[j].x1, cur[j].y1))//нашли
					{
						amount[k]++;
						exists = true;
						break;
					}
				}
				if (!exists)
				{
					points.push_back(point(cur[j].x1, cur[j].y1));
					amount.push_back(1);
				}
				//то же самое для конца ребра
				exists = false;
				for (int k = 0; k<points.size(); k++)
				{
					if (points[k] == point(cur[j].x2, cur[j].y2))//нашли
					{
						amount[k]++;
						exists = true;
						break;
					}
				}
				if (!exists)
				{
					points.push_back(point(cur[j].x2, cur[j].y2));
					amount.push_back(1);
				}
				////
			}
			if (count(amount.begin(),amount.end(),1)==degree)//при отбрасывании повторяющегося ребра не поменяла тип, значит повтор
			{
				return true;
			}
		}
	}
	return false;
}


//если true, то идентичны, если false то разные
bool compare(vector<edge> &v1, vector<edge> &v2)//на вход v1 подается отсортированный массив //
{
	if (v1.size() != v2.size())
		return false;
	vector<edge> new_vec;
	vector<point> cur_p;
	for (int i = 0; i<v2.size(); i++)
		new_vec.push_back(v2[i]);
	bool res = true;
	for (int j = 0; j<4; j++)//4 возможных положения маршрута
	{
		cur_p.clear();
		generate_points(new_vec, cur_p);
		for (int i = 0; i<cur_p.size(); i++)//перебираем точки сдвига
		{
			for (int k = 0; k<new_vec.size(); k++)//совмещаем точку i с точкой (0,0)
			{
				new_vec[k] = new_vec[k] - cur_p[i];
				new_vec[k].norm();
			}
			sort(new_vec.begin(), new_vec.end());
			if (isEqual(v1, new_vec))
			{
				res = false;
				break;
			}

			//Возвращаем вектор обратно
			for (int k = 0; k<new_vec.size(); k++)
				new_vec[k] = new_vec[k] + cur_p[i];
		}
		if (!res)
			break;
		if (j == 1)
			mirror(new_vec);
		else
			rotate180(new_vec);
	}
	return !res;
}

int add(vector<vector<edge>> &routes, vector<edge> cur,int degree=0)
{
	if (if_shadow2(cur,degree))
		return -2;
	for (int i = 0; i<cur.size(); i++)
		cur[i].norm();
	sort(cur.begin(), cur.end());

	bool check = true;
	for (int i = 0; i<routes.size(); i++)
	{
		if (compare(routes[i], cur))
		{
			check = false;
			return i;
			break;
		}
	}
	if (check)
	{
		routes.push_back(cur);
		return -1;
	}
	return -10;
}

bool next_combination(vector<int> &a, int n) //n-количество объектов и которых выбираем
{
	int k = (int)a.size();
	for (int i = k - 1; i >= 0; --i)
		if (a[i] < n - k + i + 1) {
			++a[i];
			for (int j = i + 1; j<k; ++j)
				a[j] = a[j - 1] + 1;
			return true;
		}
	return false;
}

void fillMatrix(int **matrix)
{
	int dir = 0;
	int d = 1 + (NNN / 2) * 2;
	int s = d / 2;
	int cx, cy, ct, cb, cl, cr;
	dir = 0;//0- направо, 1-вверх, 2- налево, 3- вниз
	ct = cb = cl = cr = cx = cy = s;
	for (int curn = 0; curn<d*d; curn++)
	{
		matrix[cy][cx] = curn;
		switch (dir)
		{
		case 0:
			if (cx == cr)
			{
				cr++;
				cl--;
				cx++;
				dir = 1;
			}
			else
			{
				cx++;
			}
			break;
		case 1:
			if (cy == ct)
			{
				ct--;
				cb++;
				cy--;
				dir = 2;
			}
			else
			{
				cy--;
			}
			break;
		case 2:
		{
			cx--;
			if (cx == cl)
			{
				dir = 3;
			}
		}
		break;
		case 3:
		{
			cy++;
			if (cy == cb)
			{
				dir = 0;
			}
		}
		break;
		}
	}
	//!------------Matrix output--------------------
	for (int i = 0; i<d; i++)
	{
		for (int j = 0; j<d; j++)
		{
			cout << setw(3) << matrix[i][j] << " ";
		}
		cout << "\n";
	}
	//!-----------Output end
	ofstream nodenums("nodenums.txt", ios::out);
	nodenums << "{";
	for (int i = 0; i<d; i++)
	{
		nodenums << "{";
		for (int j = 0; j<d; j++)
		{
			if (j == d - 1)
				nodenums << matrix[i][j] << "}";
			else
				nodenums << matrix[i][j] << ",";
		}
		if (i<d - 1)
			nodenums << ",";
	}
	nodenums << "}";
	nodenums.close();


}

void shift(point p, vector<edge> in, vector<edge> &out)
{
	out.clear();
	for (int i = 0; i<in.size(); i++)
	{
		out.push_back(in[i] - p);
	}
}

void transform_points(vector<point> &vp, int n, point l, vector<point> &out)
//n задает прееобразование при котором полученный веткор совмещается с исходным, 
//l задает точку, которую надо  сдвинуть в точку (0,0)
//для получения номера соответствующей  точки надо выполнить преобразования координат в обратном порядке

{
	for (int i = 0; i<vp.size(); i++)
	{
		out.push_back(vp[i]);
		if (n == 1)//повернуть обратно на 180
		{
			out[i].x = -out[i].x;
			out[i].y = -out[i].y;
		}
		if (n == 2)//поворот+отражение
		{
			out[i].y = -out[i].y;
		}
		if (n == 3)//поворот+отражение+поворот
		{
			out[i].x = -out[i].x;
		}
		out[i] = out[i] - l;
	}
}

void convert_to_numbers(vector<point> &vp, int **matr, vector<int> &out)//преобразует вектор координат в номера вершин
{
	int s = (1 + (NNN / 2) * 2) / 2;
	for (int i = 0; i<vp.size(); i++)
	{
		out.push_back(matr[vp[i].x + s][vp[i].y + s]);
	}
}

void writeRouteToFile(vector<edge> &edges, ofstream &out)
{
	//записываем результат в файл
	out << "n ";
	for (int i = 0; i<edges.size(); i++)
	{
		out << "{" << edges[i].x1 << "," << edges[i].y1 << "},{" << edges[i].x2 << "," << edges[i].y2 << "} ";
	}
	out << "\n";
}

void generate_all(vector<edge> &curent, int **matr, ofstream &out)
{
	vector<vector<edge>> sameShape;
	vector<edge> work;//
	vector<vector<int>>  Nodes;
	vector<point> ps, ps2;//точки начального маршрутов
	vector<point> shift;//преобразования сдвига для каждого маршрута
	vector<int> rotate;//преобразование поворота
	vector<bool> ok;


	point curP;
	for (int i = 0; i<4; i++)
	{
		work.clear();
		for (int j = 0; j<curent.size(); j++)
		{
			work.push_back(curent[j]);
		}
		switch (i)
		{
		case 1: rotate180(work); break;
		case 2: rotate180(work); mirror(work); break;
		case 3: rotate180(work); mirror(work); rotate180(work); break;
		}
		ps.clear();
		generate_points(work, ps);
		for (int j = 0; j<ps.size(); j++)
		{
			curP = ps[j];
			for (int k = 0; k<work.size(); k++)
			{
				work[k] = work[k] - curP;
			}
			sameShape.push_back(work);
			shift.push_back(curP);
			rotate.push_back(i);
			ok.push_back(true);
			for (int k = 0; k<work.size(); k++)
			{
				work[k] = work[k] + curP;
			}
		}
	}
	for (int i = 0; i<sameShape.size(); i++)
	{
		for (int j = 0; j<sameShape[i].size(); j++)
		{
			sameShape[i][j].norm();
		}
		sort(sameShape[i].begin(), sameShape[i].end());
	}
	for (int i = 0; i<sameShape.size() - 1; i++)
		for (int j = i + 1; j<sameShape.size(); j++)
		{
			if (ok[i] && ok[j])
				if (isEqual(sameShape[i], sameShape[j]))
				{
					ok[j] = false;
				}
		}


	ps.clear();
	generate_points(curent, ps);
	for (int j = 0; j<ps.size(); j++)
	{
		out << "{" << ps[j].x << "," << ps[j].y << "} ";
	}
	out << "\n";

	vector<int> curNum;
	for (int i = 0; i<sameShape.size(); i++)
	{
		curNum.clear();
		ps2.clear();
		if (ok[i])
		{
			transform_points(ps, rotate[i], shift[i], ps2);
			convert_to_numbers(ps2, matr, curNum);
			//writeRouteToFile(sameShape[i],out);
			for (int j = 0; j<ps.size(); j++)
			{
				out << curNum[j] << " ";
			}
			out << "\n";

		}
	}

}


void set_directions(vector<point> &directions)
{
	directions.push_back(point(1, 0));
	directions.push_back(point(1, 1));
	directions.push_back(point(0, 1));
	directions.push_back(point(-1, 1));
	directions.push_back(point(-1, 0));
	directions.push_back(point(-1, -1));
	directions.push_back(point(0, -1));
	directions.push_back(point(1, -1));
}
void next_step_loops(vector<point> &cur, int NNN, vector<point> &directions, vector<vector<edge>> &results)//length - веутрення переменная, здесь что бы не выделять под нее память
{
	if (cur.size() == 2)
		cout << cur[1].x << " " << cur[1].y << "\n";
	if (cur.size()<NNN + 1)
	{
		for (int i = 0; i<directions.size(); i++)
		{
			if (cur.size()>0)
				cur.push_back(cur[cur.size() - 1] + directions[i]); //если точки уже есть, то новая это сдвиг относительно старой
			if ((abs(cur[cur.size() - 1].x) <= NNN / 2) && (abs(cur[cur.size() - 1].y) <= NNN / 2))//нельзя уходить слишком далеко
				next_step_loops(cur, NNN, directions, results);
			cur.pop_back();
		}
	}
	else if (cur.size() == NNN + 1)//набрали весь маршрут
	{
		if ((cur[cur.size() - 1].x == 0) && (cur[cur.size() - 1].y == 0))//вернулись в начало
		{
			vector<edge> curEdges;
			edge curE;
			for (int i = 0; i<NNN; i++)
			{
				curE.set(cur[i], cur[i + 1]);
				curEdges.push_back(curE);
			}
			add(results, curEdges);
		}
	}
}
void next_step_not_loops(vector<point> &cur, int NNN, vector<point> &directions, vector<vector<edge>> &results)//length - веутрення переменная, здесь что бы не выделять под нее память
{
	//вывод для наглядности
	if (cur.size() == 2)
		cout << cur[1].x << " " << cur[1].y << "\n";
	//конец вывода
	if (cur.size()<NNN + 1)
	{
		for (int i = 0; i<directions.size(); i++)
		{
			if (cur.size()>0)
				cur.push_back(cur[cur.size() - 1] + directions[i]); //если точки уже есть, то новая это сдвиг относительно старой
			if ((abs(cur[cur.size() - 1].x) <= (NNN - 3)) && (abs(cur[cur.size() - 1].y) <= (NNN - 3)))//нельзя уходить слишком далеко
				next_step_not_loops(cur, NNN, directions, results);
			cur.pop_back();
		}
	}
	else if (cur.size() == NNN + 1)//набрали весь маршрут
	{

		if ((cur[NNN].x != 0) || (cur[NNN].y != 0))
		{
			bool not_loop = false;
			bool double_start = false;
			for (int i = 1; i<NNN; i++)//проверяем что последняя вершина уже встречалась ранее, и что есть второе прохождение через (0,0)
			{
				if ((cur[NNN].x == cur[i].x) && (cur[NNN].y == cur[i].y))
					not_loop = true;
				if ((cur[i].x == 0) && (cur[i].y == 0))
					double_start = true;
			}
			if (not_loop&&double_start)//вернулись в начало
			{
				vector<edge> curEdges;
				edge curE;
				for (int i = 0; i<NNN; i++)
				{
					curE.set(cur[i], cur[i + 1]);
					curEdges.push_back(curE);
				}
				add(results, curEdges);
				//results.push_back(curEdges);
			}
		}
	}
}

//temp
void check_old()
{
	vector<vector<edge>> res;
	vector<vector<edge>> badres;
	vector<int> badcodes;// -2 -shadow, "-1" -ок, Positive - identical from res;
	vector<edge> cur;
	edge e;
	ostringstream os;
	istringstream is;
	os << NNN << "_uniq_routes_l.txt";
	ifstream in(os.str(), ios::in);
	string s;
	char c;
	int m;
	int num = 0, code;

	int global_old_number = 1;
	ofstream global_good("glod_good.txt", ios::out);
	ofstream global_bad("glod_bad.txt", ios::out);
	while (!in.eof())
	{
		getline(in, s);
		if (s.length()>0)
		{
			if (s[0] == 'n')
			{
				num++;
				s = s.substr(2);
				is.str(s);
				cur.clear();
				for (int i = 0; i<NNN; i++)
				{
					is >> c >> e.x1 >> c >> e.y1 >> c >> c >> c >> e.x2 >> c >> e.y2 >> c;
					cur.push_back(e);
				}
				m = res.size();
				code = add(res, cur);
				if (res.size() - m == 0)
				{
					badres.push_back(cur);
					badcodes.push_back(code);
					global_bad << global_old_number << "\n";
				}
				else
				{
					global_good << global_old_number << "\n";
				}
				global_old_number++;
			}
		}
	}
	ofstream out("test.txt", ios::out);
	out << res.size() << "\n";
	for (int i = 0; i<res.size(); i++)
	{
		out << "i=" << i << "  ";
		for (int j = 0; j<NNN; j++)
		{
			out << "{" << res[i][j].x1 << "," << res[i][j].y1 << "},{" << res[i][j].x2 << "," << res[i][j].y2 << "} ";
		}
		out << "\n";
	}
	ofstream out2("test_bad.txt", ios::out);
	out2 << badres.size() << "\n";
	for (int i = 0; i<badres.size(); i++)
	{
		out2 << "i=" << i << "  " << badcodes[i] << " ";
		for (int j = 0; j<NNN; j++)
		{
			out2 << "{" << badres[i][j].x1 << "," << badres[i][j].y1 << "},{" << badres[i][j].x2 << "," << badres[i][j].y2 << "} ";
		}
		out2 << "\n";
	}
}


void generate11(vector<point> &cur, int NNN, vector<point> &directions, vector<vector<edge>> &results)
{
	//
}


int getDegrees(vector<edge> &inp, vector<int> &vertexDegree, vector<point> &points)
{
	point cur;
	bool check;
	for (int i = 0; i < inp.size(); i++)
	{
		cur.set(inp[i].x1, inp[i].y1);

		vector<point>::iterator it=find(points.begin(), points.end(), cur);
		if(it== points.end())
		{
			points.push_back(cur);
			vertexDegree.push_back(1);
		}
		else
		{
			vertexDegree[distance(points.begin(), it)]++;
		}
		cur.set(inp[i].x2, inp[i].y2);

		it = find(points.begin(), points.end(), cur); //ищем текущую точку среди уже найденных
		if (it == points.end())
		{
			points.push_back(cur); //добавляем новую точку
			vertexDegree.push_back(1);
		}
		else
			vertexDegree[distance(points.begin(), it)]++; //увеличиваем степень существующей вершины
	}
	return count(vertexDegree.begin(), vertexDegree.end(), 1);//считаем кол-во вершин со степенью 1
}

void addEdge(vector<edge> &parent, vector<vector<edge>> &results, int type,vector<point> &directions)//type определяет сколько вершин со степенью один нужно на выходе
{
	vector<point> points;
	vector<int> degrees;
	vector<edge> cur;
	getDegrees(parent, degrees, points);
	if (type == 1)//хотим получить маршрут типа 1-1 на выходе
	{
		int ones = count(degrees.begin(), degrees.end(), 1);
		if (ones > 1) return; //не получаем 1-1 из предыдущего 1-2
		if (ones == 1)//добавляем в саму вершину
		{
			int num = 0;
			for (int i = 0; i < degrees.size();i++)
				if (degrees[i] == 1) { num = i; break; }//нашли вершину со степенью 1
			point end;
			edge e;
			for (int i = 0; i < directions.size(); i++)
			{
				end = points[num] + directions[i];
				if (find(points.begin(), points.end(), end) == points.end())//если точка не найдена, то после добавления это будет маршрут типа 1-1
				{
					cur.clear();
					cur.resize(parent.size());
					copy(parent.begin(), parent.end(), cur.begin());
					cur.push_back(edge(points[num], end));
					add(results, cur);
				}
			}	
		}
		if (ones == 0) 
		{
			point end;
			for (int i = 0; i< points.size(); i++)
			{
				for (int j = 0; j < directions.size(); j++)
				{
					end = points[i] + directions[j];
					if (find(points.begin(), points.end(), end) == points.end())//если точка не найдена, то после добавления это будет маршрут типа 1-1
					{
						cur.clear();
						cur.resize(parent.size());
						copy(parent.begin(), parent.end(), cur.begin());
						cur.push_back(edge(points[i], end));
						add(results, cur);
					}
				}
			}
		}

	}
	else //получаем маршрут типа 1-2
	{

	}
}

void read_routes(string fname, vector<vector<edge>> &parents,int size)//size- количество ребер в предке
{
	ifstream inp(fname, ios::in);
	istringstream in;
	char c;
	string s;
	point p1, p2;
	vector<edge> curGr;
	while (!inp.eof())
	{
		curGr.clear();
		getline(inp, s);
		if (s[0] != 'n') return;
		in.str(s);
		in >> c;
		for (int i = 0; i < size; i++)
		{
			in >> c;
			in >> p1.x;
			in >> c;
			in >> p1.y;
			in >> c;
			in >> c;
			in >> c;
			in >> p2.x;
			in >> c;
			in >> p2.y;
			in >> c;
			curGr.push_back(edge(p1, p2));
		}
		parents.push_back(curGr);
	}
}



void generate_routes(vector<edge> &cur,vector<vector<edge>> &res0, vector<vector<edge>> &res1, vector<vector<edge>> &res2,int max_edges,vector<point> &directions)
{
	
	if (cur.size() < max_edges)
	{
		vector<int> vertDeg;
		vector<point> points;
		getDegrees(cur, vertDeg, points);
		if(count(vertDeg.begin(),vertDeg.end(),1)<=2)
		{ 
			for (int i = 0; i < points.size(); i++)
			{
				for (int j = 0; j < directions.size(); j++)
				{
					if (cur.size() == 1)
						cout << "Start new step, dir=" << j << "\n";

					cur.push_back(edge(points[i], points[i] + directions[j]));
					generate_routes(cur, res0, res1, res2, max_edges, directions);
					cur.pop_back();
				}
			}
		}
	}
	else
	{
		vector<int> vertDeg;
		vector<point> points;
		getDegrees(cur, vertDeg, points);
		switch (count(vertDeg.begin(), vertDeg.end(), 1))
		{
			case 0:	add(res0, cur,0); break;
			case 1:	add(res1, cur,1); break;
			case 2:	add(res2, cur,2); break;
			default:
				break;
		}
	}
}



//service function
void string_to_graph(string s, vector<edge> &res)
{
	istringstream in;
	char c;
	point p1, p2;
	int amount = (count(s.begin(), s.end(), ','))/3;
	in.str(s);
	for (int i = 0; i < amount; i++)
	{
			in >> c;
			in >> p1.x;
			in >> c;
			in >> p1.y;
			in >> c;
			in >> c;
			in >> c;
			in >> p2.x;
			in >> c;
			in >> p2.y;
			in >> c;
			res.push_back(edge(p1, p2));
	}
}



int main()
{
	vector<point> cur;
	vector<point> directions;
	vector<vector<edge>> results;
	vector<vector<edge>> parents;

	int type;

	set_directions(directions);
	cur.push_back(point(0, 0));

	//test
	vector<edge> v1;
	string_to_graph("{0,-2},{0,-1} {0,-1},{0,0} {0,0},{0,1} {0,0},{0,1} {0,1},{0,2}", v1);
	//end test

	/*read_routes("data\\2_uniq_l.txt", parents,2);
	for (int i = 0; i < parents.size(); i++)
	{
		addEdge(parents[i], results, 1, directions);
	}
	ofstream out_t("out_test.txt",ios::out);
	for (int i = 0; i < results.size(); i++)
	{
		writeRouteToFile(results[i], out_t);
	}
	out_t.close();*/


	//////////////////////////////////////////////////////////////////////////////
	const int diff_direct = 3;
	const int diff_types=3;
	vector<vector<edge>> res[diff_types];
	ofstream out_files[diff_types];
	
	vector<edge> cur_r;
	int cur_len = 1;
	ostringstream fname;
	for (int i = 0; i < diff_types; i++)
	{
		fname.str("");
		fname << "data\\" << cur_len << "_uniq_" << i << ".txt";
		(out_files[i]).open(fname.str(), ios::out);
	}
	
	for (int i = 0; i < diff_direct; i++)
	{
		cur_r.clear();
		cur_r.push_back(edge(point(0, 0), directions[i]));
		generate_routes(cur_r, res[0], res[1], res[2], cur_len, directions);
	}


	for (int i = 0; i < diff_types; i++)
	{
		for (int j = 0; j < (res[i]).size(); j++)
		{
			writeRouteToFile(res[i][j], out_files[i]);
		}
	}
	return 0;
	//////////////////////////////////////////////////////////////////////////////
	

	ifstream inp("input.txt", ios::in);
	inp >> type;
	if (type == 1)
		next_step_loops(cur, NNN, directions, results);
	if (type == 2)
		next_step_not_loops(cur, NNN, directions, results);


	ostringstream ostr;
	if (type == 1)
		ostr << NNN << "_uniq_l.txt";
	if (type == 2)
		ostr << NNN << "_uniq_nl.txt";
	ofstream out(ostr.str(), ios::out);
	for (int i = 0; i<results.size(); i++)
	{
		out << "n ";
		for (int j = 0; j<NNN; j++)
		{
			out << "{" << results[i][j].x1 << "," << results[i][j].y1 << "},{" << results[i][j].x2 << "," << results[i][j].y2 << "} ";
		}
		out << "i=" << i + 1 << "  ";
		out << "\n";
	}
	out.close();
	return 0;
}

