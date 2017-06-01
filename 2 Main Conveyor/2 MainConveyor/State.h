#pragma once
#include "stdafx.h"
#include "JFactors.h"

class State
{
	double factor; //�������� ���������
	char coeff[JFactors::Npowers];  //������� ������������� J1,J2 � (J2-J1)
	std::vector<char> states;// ������ ��������� �� ��������������� ��������

public:
	
	bool operator<(const State &s) const;//������ ��� 2�� ���������, ���������� � ������ ��������

	bool operator==(const State &s) const; //��������� ����� ���� ������ ���� ��������� �����

	bool check(const State &s);

	char getPowerByNumber(unsigned int num) const;

	char getStateByNumber(unsigned int num) const;

	double getFactor() const;

	double getEnergie(const double energies[]) const;
		
	void copyPowers(const State &s);

	void copyStates(const State &s);

	void incPower(unsigned int num);

	void setStateByNumber(unsigned int num, char newState);

	void setFactor(double initFactor);

	void multiplicateFactor(double multiplier);

	void addToFactor(double addition);

	void clear();

	void addClusterState(int newStateNum);

	int getNodesNumber()const;

	State();
	State(unsigned int num);

	~State();
};

