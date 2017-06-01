#pragma once
#include "stdafx.h"
#include "Consts.h"
#include "Results.h"
#include "JFactors.h"
#include "State.h"




struct point
{
	int sx;
	int sy;

	point(int x, int y);
		
	bool operator==(const point& s2) const;
};

struct inter
{
	char n1,n2; //номера плакетов, нумерация с 0
	char v1,v2; //номера вершин, нумерация с 0
	char Jtype; // 0-J1, 1-J2, 2-(J2-J1)
};

//struct res
//{
//	double factors[ResAmount]; //итоговые множители при полном сворачивании
//
//	res& operator+=(const res& tmp);
//
//	void minus();
//};

struct edge //для хранения координат ребер
{
	int x1,y1,x2,y2;

	bool operator==(const edge &e2) const;
	
	void set(int X1, int Y1, int X2, int Y2);

};


//////////////////////////////////////////////////////////////////////////
//extern double Vmatrix[SpinsInPlaquet][3][DiffStates][DiffStates];//Номер вершины в плакете, тип матрицы SP,SM или SZ,номер ряда, номер столбца
//extern double VmatrixInside[DiffStates][DiffStates];//Специально для внутренних воздействий
//extern double Energie[DiffStates]; //Энергии состояний
//extern inter curInter [N][MaxIntElem]; //массив операторов взамодействий
//extern int interAmount[N];// кол-во эл-тов в каждом операторе

//extern std::vector<State> vIn[Namount],vOut1[Namount],vOut2[Namount]; //вектора состоянй-используем по очереди один как входной другой как выходной, на следующем шаге - наоборот
//extern std::vector<State> vOutTemp1,vOutTemp2;//Промежуточные вектора
//extern std::vector<State> temp,temp2;

//////////////////////////////////////////////////////////////////////////

int minus1(int *nodeSet, int n);

void eval_cur_route(int r[][2], int OrderLength, int RouteLength, std::vector<edge> &edges, int nodeNumsOfEdges[N][2], std::vector<point> &nodes, int &RealLength);

//int find_last_group(std::vector<State> &cur, int start_n);

//Results finalvalue4(std::vector<State> &v1, std::vector<State> &v2, int n);

//осталась от начального случая - переписать или удалить
bool check_cur_operator_set(bool &Res, int OrderLength, int RealLength, int *termorder, int *op_set, std::vector<edge> edges);//проверяем можкт ли быть не 0 по данной конфигурации

void read_Route(int r[][2], std::istringstream &s);

//////////////////////////////////////////////////////////////////////////

//double getE0(int node_num);//определяет энергию уровня, к которму определяются поправки

//void returnV(inter curInter[][MaxIntElem],int interAmount[],int interN,int n1,int n2,int dx,int dy);//устанавливает оператор взаимодействия вдоль ребра

//void collect(std::vector<State> &outvec,std::vector<State> &invec);

//void act_copy(std::vector<State> &inv, std::vector<State> &outv);//просто копирует состояние, нужно для более единообразной раоты в первом порядке
//
//void act(std::vector<State> &inv,std::vector<State> &outv,double Vmatrix[SpinsInPlaquet][3][DiffStates][DiffStates],int interNumber, int node_num);
//
//void act_ground(std::vector<State> &inv,std::vector<State> &outv,double Vmatrix[SpinsInPlaquet][3][DiffStates][DiffStates],int interNumber, int node_num);
//
//void act_energy(std::vector<State> &inv,std::vector<State> &outv,double Vmatrix[SpinsInPlaquet][3][DiffStates][DiffStates],int interNumber, int node_num);
//
//void act_energy_power(std::vector<State> &inv,std::vector<State> &outv,int power,double Vmatrix[SpinsInPlaquet][3][DiffStates][DiffStates],int interNumber, int node_num);
//
//void act_inside(std::vector<State> &inv,std::vector<State> &outv,int plaquetNumber, int node_num);
//
//void act_inside_ground(std::vector<State> &inv,std::vector<State> &outv,int plaquetNumber, int node_num);
//
//void act_inside_enrgy_power(std::vector<State> &inv,std::vector<State> &outv,int power,int plaquetNumber, int node_num);

void generate_procedure_order(int *termorder,int* operatororder,int edge_amount,int num,int *Res,int *power);
