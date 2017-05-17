#pragma once
#include "stdafx.h"
#include <string>
class matrixNodes
{
	int **matrix;
	int size;
public:
	matrixNodes();
	~matrixNodes();

	void matrixNodes::fillMatrix(int maxPlaquetDistance);
	void matrixNodes::printMatrix();
	void matrixNodes::printMatrixToFile(std::string fname);
	int getElem(int i, int j);
	int getSize();
};

