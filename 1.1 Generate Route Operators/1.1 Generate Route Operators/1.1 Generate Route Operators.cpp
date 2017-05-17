// 1.6 Generate Additional Not Loop.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <algorithm>
#include <sstream>
using namespace std;
//const int Nmax=10;

const string res_dir = "Results";
const string inp_dir = "Input";
const string delim = "\\";
const string type1 = "0";
const string type2 = "1";
const string type3 = "2";
struct step
{
	step(int x=0, int y=0)
	{
		sx = x;
		sy = y;
	}
	void set(int x, int y)
	{
		sx = x;
		sy = y;
	}
	int sx;
	int sy;
	bool operator==(step s2)
	{
		if ((sx == s2.sx) && (sy == s2.sy))
		{
			return true;
		}
		else
			return false;
	}
};


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
	bool operator==(const edge &e2)
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
		if (x1 < e2.x1)
			return true;
		else if (x1 == e2.x1)
		{
			if (y1 < e2.y1)
				return true;
			else if (y1 == e2.y1)
			{
				if (x2 < e2.x2)
					return true;
				else if (x2 == e2.x2)
				{
					if (y2 < e2.y2)
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
	void set(const point& p1, const point& p2)
	{
		x1 = p1.x;
		y1 = p1.y;
		x2 = p2.x;
		y2 = p2.y;
	}
	void set(const edge& e)
	{
		x1 = e.x1;
		y1 = e.y1;
		x2 = e.x2;
		y2 = e.y2;
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

bool isEqual(vector<edge> &v1, vector<edge> &v2)
{
	if (v1.size() != v2.size())
		return false;
	for (int i = 0; i < v1.size(); i++)
	{
		if (!(v1[i] == v2[i]))
			return false;
	}
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

bool compare(vector<edge> &v1, vector<edge> &v2)//на вход v1 подается отсортированный массив
{
	if (v1.size() != v2.size())
		return false;
	vector<edge> new_vec;
	vector<point> cur_p;
	for (int i = 0; i < v2.size(); i++)
	{
		new_vec.push_back(v2[i]);
	}
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
			for (int k = 0; k < new_vec.size(); k++)
			{
				new_vec[k] = new_vec[k] + cur_p[i];
			}
		}
		if (!res)
			break;
		if (j == 1)
			mirror(new_vec);
		else
			rotate180(new_vec);
	}
	return res;
}

int size(int NNN)
{
	return 2*NNN + 1;//max(1 + ((NNN) / 2) * 2, 1 + (NNN - 2) * 2);//
}

void fillMatrix(int NNN, int **matrix)//NNN - текущий порядок
{
	int dir = 0;
	int d = size(NNN);
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

void read_route_l(int NNN, string s, vector<edge> &cur)//check!!!
{
	cur.clear();
	point p;
	char c;
	vector<point> curp;
	istringstream route;
	s = s.substr(4);
	route.str(s);
	for (int i = 0; i<NNN; i++)
	{
		route >> p.x >> c >> p.y;
		curp.push_back(p);
		route >> c >> c >> c;
	}
	for (int i = 0; i<NNN; i++)
	{
		cur.push_back(edge(curp[i].x, curp[i].y, curp[(i + 1) % NNN].x, curp[(i + 1) % NNN].y));
	}
}

void read_route_nl(int NNN, string s, vector<edge> &cur)
{
	cur.clear();
	point p;
	char c;
	vector<point> curp;
	istringstream route;
	s = s.substr(3);
	route.str(s);
	for (int i = 0; i <= NNN; i++)
	{
		route >> p.x >> c >> p.y;
		curp.push_back(p);
		route >> c >> c >> c;
	}
	for (int i = 0; i<NNN; i++)
	{
		cur.push_back(edge(curp[i].x, curp[i].y, curp[i + 1].x, curp[i + 1].y));
	}
}

//NEED TEST!!!!
void read_route_ns(int NNN, string s, vector<edge> &cur)
{
	cur.clear();
	point p1, p2;
	char c;
	istringstream route;
	//s=s.substr(3);
	route.str(s);
	for (int i = 0; i<NNN; i++)
	{
		route >> c >> c;
		route >> p1.x >> c >> p1.y;
		route >> c >> c >> c;
		route >> p2.x >> c >> p2.y;
		cur.push_back(edge(p1.x, p1.y, p2.x, p2.y));
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

void convert_to_numbers(int NNN, vector<point> &vp, int **matr, vector<int> &out)//преобразует вектор координат в номера вершин
{
	int s = size(NNN) / 2;
	for (int i = 0; i<vp.size(); i++)
	{
		out.push_back(matr[vp[i].x + s][vp[i].y + s]);
	}
}

void generate_all(int NNN, vector<edge> &curent, int **matr, ofstream &out)
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
		
		//curP = ps[j];
			
		sameShape.push_back(work);
		shift.push_back(point(0,0));
		rotate.push_back(i);
		ok.push_back(true);
			
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


	out << "n ";
	for (int i = 0; i<curent.size(); i++)
	{
		curent[i].norm();
		out << "{" << curent[i].x1 << "," << curent[i].y1 << "},{" << curent[i].x2 << "," << curent[i].y2 << "} ";
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
			convert_to_numbers(NNN, ps2, matr, curNum);
			//writeRouteToFile(sameShape[i],out);
			out << "i ";
			for (int j = 0; j<ps.size(); j++)
			{
				out << curNum[j] << " ";
			}
			out << "\n";

		}
	}

}

void generate_Route_Operators(vector<edge> vr, int OrderLength, int RouteLength, ofstream &out)
{
	//standard form: sort edge's nodes and then edges
	for (int i = 0; i<RouteLength; i++)
		vr[i].norm();//
	sort(vr.begin(), vr.end());

	//output of the route
	out << "n ";
	for (int i = 0; i<RouteLength; i++)
		out << "{" << vr[i].x1 << "," << vr[i].y1 << "},{" << vr[i].x2 << "," << vr[i].y2 << "} ";
	out << "\n";

	//Проверяем реальную длину маршрута+ строим маску - сколько раз каждое ребро должно встречаться
	vector<edge> edges;
	vector<step> nodes;
	const int N = 10;
	int num_node[N];//10- с запасом
	int num_ed[N];//число ребер
	int cur_edges[N];
	int cur_nums[N];//Текущая расстановка операторов в слагаемом
	edge cur;
	bool flag;
	int RealLength = 0;//Реальное количество различных "ГЛАВНЫХ" операторов
	int RealNodeLength = 0; //Реальное количество различных вершин
	for (int i = 0; i<N; i++)
	{
		num_ed[i] = 0;
		num_node[i] = 0;
	}
	for (int i = 0; i<RouteLength; i++)//список ребер
	{
		cur.set(vr[i]);
		flag = true;
		for (int j = 0; j<edges.size(); j++)
		{
			if (cur == edges[j])
			{
				flag = false;
				num_ed[j]++;
			}
		}
		if (flag)
		{
			edges.push_back(cur);
			num_ed[edges.size() - 1]++;
		}

	}
	RealLength = edges.size();

	int OnesAmountInit = 0;//кол-во вершин со степенью один в начальном маршруте
	step cur_node1(0, 0), cur_node2(0, 0);
	for (int i = 0; i<edges.size(); i++)
	{
		cur_node1.set(edges[i].x1, edges[i].y1);
		flag = true;
		for (int j = 0; j<nodes.size(); j++)
		{
			if (nodes[j] == cur_node1)
			{
				num_node[j]+=num_ed[i];
				flag = false;
			}
		}
		if (flag)
		{
			nodes.push_back(cur_node1);
			num_node[nodes.size() - 1]+= num_ed[i];
		}

		cur_node2.set(edges[i].x2, edges[i].y2);
		flag = true;
		for (int j = 0; j<nodes.size(); j++)
		{
			if (nodes[j] == cur_node2)
			{
				num_node[j]+= num_ed[i];
				flag = false;
			}
		}
		if (flag)
		{
			nodes.push_back(cur_node2);
			num_node[nodes.size() - 1]+= num_ed[i];
		}

	}
	RealNodeLength = nodes.size();
	for (int i = 0; i < RealNodeLength; i++)
	{
		if (num_node[i] == 1)
			OnesAmountInit++;
	}

	int amount = pow((double)(RealLength + RealNodeLength), (int)(OrderLength));//Вычисляем всевозможное количество растоновок операторов
	int factor = RealLength + RealNodeLength;//степень, на  которую делим
	int temp, j;
	out << RealLength << " " << RealNodeLength << "\n";

	for (int i = 0; i<amount; i++)
	{
		//Подготовка
		temp = i;
		for (j = 0; j<RealLength; j++)
			cur_edges[j] = 0;
		flag = true;
		//Вычисляем по номеру порядок операторов
		for (j = 0; j<OrderLength; j++)
		{
			cur_nums[j] = temp%factor;
			temp /= factor;
			if (cur_nums[j]<RealLength)
				cur_edges[cur_nums[j]]++;
		}

		//проверяем конфигурацию
		for (j = 0; j<RealLength; j++)
		{
			if (num_ed[j]>cur_edges[j])
			{
				flag = false;
				break;
			}
		}
		//проверяем, что новая конфигурация относится к тому же типу, что и изначалная, для этого считаем маску
		int cur_nodes_num[N];
		step cur_n(0,0);
		int n=0;
		int OnesAmount=0; //количество вершин со степенью один, так как по ним классифицируются типы маршрутов
		for (int i = 0; i < RealNodeLength; i++)
		{
			cur_nodes_num[i] = 0;
		}

		for (int i = 0; i < RealLength; i++)
		{
			cur_n.set(edges[i].x1, edges[i].y1);
			n=distance(nodes.begin(), find(nodes.begin(), nodes.end(), cur_n)); //ищем вершину
			cur_nodes_num[n] += cur_edges[i]; //увеличиваем степень вершины на кол-во ребер
			cur_n.set(edges[i].x2, edges[i].y2);
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
				out << cur_nums[j] << " ";
			}
			out << "\n";
		}
	}

	out.close();
}

int _tmain(int argc, _TCHAR* argv[])
{
	int **matr;
	int NNN, subOrder, routeType, format;
	string temp;
	ifstream config("!conf.txt", ios::in);
	getline(config, temp); //read of header
	config >> NNN >> subOrder >> routeType >> format;
	int d = size(NNN);

	matr = new int*[d];
	for (int i = 0; i < d; i++)
	{
		matr[i] = new int[d];
	}
	fillMatrix(NNN, matr);

	ostringstream ostr;

	string type;
	switch (routeType)
	{
	case 0: type = type1; break;
	case 1: type = type2; break;
	case 2: type = type3; break;
	}
	ostr << inp_dir << delim << subOrder << "_uniq_" << type << ".txt";
	ifstream in(ostr.str(), ios::in);
	string s;
	vector<edge> cur;
	vector<vector<edge>> Routes;
	ofstream out;
	if (subOrder == NNN)
	{
		ostr.str("");
		ostr << res_dir << delim << subOrder << "_uniq_routes_" << type << ".txt";
		out.open(ostr.str(), ios::out);
	}
	int count = 1;
	while (!in.eof())
	{
		getline(in, s);
		if (s.length()>0)
		{
			if (s[0] == 'n')
			{
				switch (format)
				{
				case 1: read_route_l(subOrder, s, cur);  break;
				case 2: read_route_nl(subOrder, s, cur); break;
				case 3: read_route_ns(subOrder, s, cur); break;
				}

				for (int i = 0; i < cur.size(); i++)
				{
					cur[i].norm();
				}
				sort(cur.begin(), cur.end());
				if (NNN == subOrder)
					generate_all(NNN, cur, matr, out);
				ostr.str("");
				ostr << res_dir << delim << NNN << "_" << subOrder << "_" << count++ << "_routeNum_" << type << ".txt";
				//Верменное отключения для генерации правильных ведущих файлов в каждом порядке
				ofstream cur_out(ostr.str(), ios::out);
				generate_Route_Operators(cur, NNN, subOrder, cur_out);
			}
		}
	}
	return 0;
}

