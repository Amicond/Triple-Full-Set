#include "stdafx.h"
#include "Operators.h"

//names of different operators. length should be equal to the OrtsNumber const;
const std::string Operators::matrixNamePrefixes[] = { "noutp_s", "noutm_s", "noutz_s" };

Operators::Operators()
{
}

Operators::~Operators()
{
}

void Operators::readMatrixAndEnergie()
{
	std::ifstream in;
	std::string s = "";
	for (int prefixNum = 0; prefixNum < OrtsAmount;prefixNum++)
	{
		for (int i = 0; i < SpinsInPlaquet; i++)
		{
			std::ostringstream tmp;
			tmp << (i + 1);
			s = InputPrefixForMatrixes + matrixNamePrefixes[prefixNum] + tmp.str() + FileType;
			in.open(s.c_str(), std::ios::in);
			for (int j = 0; j < DiffStates; j++)
			{
				for (int k = 0; k < DiffStates; k++)
				{
					in >> vMatrix[i][prefixNum][j][k];
					if (vMatrix[i][prefixNum][j][k] != 0)
						vMatrixNonZero[i][prefixNum][j].push_back(std::pair<int, double>(k, vMatrix[i][prefixNum][j][k]));
				}
			}
			in.close();
		}
	}

	in.open((InputPrefixForMatrixes + "inside_matr.txt").c_str(), std::ios::in);
	for (int i = 0; i<DiffStates; i++)
		for (int j = 0; j<DiffStates; j++)
		{
			in >> vMatrixInside[i][j];
			if (vMatrixInside[i][j] != 0)
				vMatrixInsideNonZero[i].push_back(std::pair<int, double>(j, vMatrixInside[i][j]));
		}
	in.close();

	in.open((InputPrefixForMatrixes + "energy.txt").c_str(), std::ios::in);
	for (int i = 0; i<DiffStates; i++)
	{
		in >> Energie[i];
	}


}

//special Function for triplet case
double Operators::getE0(int node_num) {
	return Energie[0] * (node_num - 1) + Energie[2];//
}

double Operators::getMatrixElements(int spinNumber, int ort, int inState, int outState)const
{
	return vMatrix[spinNumber][ort][inState][outState];
}

double Operators::getInsideMatrixElements(int inState, int outState)const
{
	return vMatrixInside[inState][outState];
}

const std::vector<std::pair<int, double>>& Operators::getNonZeroElements(int spinNumber, int ort, int inState)const
{
	return vMatrixNonZero[spinNumber][ort][inState];
}

const std::vector<std::pair<int, double>>& Operators::getNonZeroElementsInsideOperator(int inState)const
{
	return vMatrixInsideNonZero[inState];
}

const double* Operators::getEnergiesOfStates()const
{
	return Energie;
}