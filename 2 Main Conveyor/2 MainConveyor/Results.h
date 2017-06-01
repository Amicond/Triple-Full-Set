#pragma once
#include "Consts.h"
#include "waveFunction.h"
class Results
{
	double factors[ResAmount]; //�������� ��������� ��� ������ ������������
public:

	double getFactor(int num)const;
	void setFactor(int num, double value);
	void addToFactor(int num, double value);

	void clearForCurrentOrder(int order);
	Results& operator+=(const Results& tmp);

	void minus();

	void scalarProduct(WaveFunction& wf1, WaveFunction& wf2);
	Results();
	~Results();
};

