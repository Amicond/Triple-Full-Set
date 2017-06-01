#include "stdafx.h"
#include "Results.h"


Results::Results()
{
	for (int i = 0; i < ResAmount; i++)
	{
		factors[i] = 0;
	}
}


Results::~Results()
{
}

double Results::getFactor(int num)const
{
	return factors[num];
}

void Results::setFactor(int num, double value)
{
	factors[num] = value;
}

void Results::addToFactor(int num, double value)
{
	factors[num] += value;
}

Results& Results::operator+=(const Results& tmp)
{
	for (int i = 0; i < ResAmount; i++)
	{
		factors[i] += tmp.factors[i];
	}
	return *this;
}

void Results::minus()
{
	for (int i = 0; i < ResAmount; i++)
		factors[i] *= -1;
}

void  Results::clearForCurrentOrder(int order)
{
	for (int i = 0; i < (order + 2)*(order + 1) / 2; i++)
	{
		factors[i] = 0;
	}
}

void Results::scalarProduct(WaveFunction& wf1, WaveFunction& wf2)
{
	unsigned int i1, i2;
	int tmpres[JFactors::Npowers];
	i2 = 0;// ��������� ������� ������
	i1 = 0;// ��������� ������� ������
	int last1, last2;

	while ((i2<wf2.getEigenstatesAmount()) && (i1<wf1.getEigenstatesAmount()))
	{

		if (wf2.getEigenstateByNumber(i2) == wf1.getEigenstateByNumber(i1))//���������� ������������
		{
			last1 = wf1.findLastGroup(i1);
			last2 = wf2.findLastGroup(i2);
			for (int i = i1; i <= last1; i++)
			{
				for (int j = i2; j <= last2; j++)
				{
					//���������� ������� J-����������
					for (int ttt = 0; ttt<JFactors::getAmountOfPowers(); ttt++)
					{
						tmpres[ttt] = wf1.getEigenstateByNumber(i).getPowerByNumber(ttt) + wf2.getEigenstateByNumber(j).getPowerByNumber(ttt);
					}

					//����������� ��� ������� � ����� ������ ������� � ��������� �� ������ ��������
					factors[JFactors::getNumberByPowers(tmpres)]+= wf1.getEigenstateByNumber(i).getFactor()*wf2.getEigenstateByNumber(j).getFactor();
				}
			}
			i1 = last1 + 1;
			i2 = last2 + 1;
		}
		else if (wf2.getEigenstateByNumber(i2)<wf1.getEigenstateByNumber(i1))
		{
			i2 = 1 + wf2.findLastGroup(i2);//������� � ��������� ������
		}
		else
		{
			i1 = 1 + wf1.findLastGroup(i1);
		}
	}
}