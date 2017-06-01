#pragma once
#include "stdafx.h"

const int N = 5;
const std::string Delim = "\\";
const std::string InputDirFinalData = "final_data";
const std::string InputPrefixForMatrixes = "matrixes" + Delim + "b3_";
const std::string InputPrefixForRroutes = "input_routes" + Delim;
const std::string OutputPrefixResults = "results_";
const std::string FileType = ".txt";

const std::string StrType[] = { "0","1","2" };

const int SpinsInPlaquet = 4;
const int OrtsAmount = 3;

const int indexOfInsideJOperator = 2;

const int DiffStates = 16; //количкество различных собственных состояний, равно размеру матриц
const int MaxIntElem = 4; // количество слагаемых в операторе взаимодействия
const int Namount = 576;   //количество различных ground-состояний 6
const int ResAmount = 45; //кол-во различных J факторов в 8 порядке


