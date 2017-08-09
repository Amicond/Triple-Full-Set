// Define Systems.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "fileNamePrinter.h"
#include "configReader.h"
#include "globalConsts.h"

std::string getSystemNumber(int size)
{
	switch (size)
	{
	case 3:	  return "1";
	case 12:  return "2";
	case 36:  return "3";
	case 96:  return "4";
	case 240: return "5";
	case 576: return "6";
	}
}

void define_term(std::ofstream &math_out, std::string &point, int type, int order, int subOrder, int &total, std::ofstream &out, int term_amount[][3], int step)
{
	std::ostringstream fname, tmpStr;
	std::ifstream cur_f;
	std::string MatrixStr, routeType;
	int size;

	for (int j = 1; j <= term_amount[subOrder][type]; j++)
	{

		total++;
		if (total%step == 0)
		{
			out.close();
			//добавл€ем в авто файл
			math_out << "nb1= NotebookOpen[StringJoin[{NotebookDirectory[], \"" << fileNamePrinter::getFileNameOfMathematicaFile(subOrder, point, total / step) << "\"}]];\n";
			math_out << "NotebookEvaluate[nb1];\n";
			math_out << "NotebookClose[nb1];\n";
			//конец добавки
			out.open(fname.str(), std::ios::out);
		}
		
		
		routeType = std::to_string(type);
		
		//fname.str("");
		//fname << root_path << "results_" << temp << Delimiter << order << "." << subOrder << Delimiter << order << "_" << subOrder << "_" << j << "_res_" << temp << ".txt";
		cur_f.open(fileNamePrinter::getPathToResMatrixFiles(routeType,order,subOrder,j), std::ios::in);
		
		cur_f >> size;
		tmpStr.str("");
		tmpStr << "res" << routeType << "$" << order << "$" << subOrder << "$" << j;
		out << tmpStr.str() << "=";
		while (!cur_f.eof())
		{
			getline(cur_f, MatrixStr);
			if (MatrixStr.length()>0)
				out << MatrixStr;
		}
		out << ";\n";
		out << "Eff" << tmpStr.str() << "=Expand[Chop[Solve[Sys";
		
		out << getSystemNumber(size) << "==" << tmpStr.str() << "/.{J1->"<<J1<<",J2->" << point << "},Var" << getSystemNumber(size) << "]]];\n";
		out << "tt = OpenWrite[\""<<  fileNamePrinter::getPathToMathematicaSolutionsFiles(point,routeType,order,subOrder,j) << "\"];\n";
		out << "If[Length[Eff" << tmpStr.str() << "]==0,Write[tt,\"" << order << " " << subOrder << " " << j << "\"];];\n";
		out << "For[i = 1, i <= Length[Eff" << tmpStr.str() << "[[1]]], i++,\n";
		out << "	 WriteString[tt,";
		out << "ScientificForm[Eff" << tmpStr.str() << "[[1, i]], ";
		out << "ExponentFunction -> (If[-100 < # < 10, Null, #] &)], \"\\n\"]";
		out << "];\n";
		out << "Close[tt];\n\n\n";
		cur_f.close();
	}
}

int _tmain(int argc, _TCHAR* argv[])
{

	//std::ifstream conf("config.txt", std::ios::in);


	//std::string root_path;
	const int J1 = 1;
	std::vector<std::string> my_points;

	std::string temp;

	std::ofstream out;
	std::ofstream math_out;

	std::ifstream cur_f;
	std::ostringstream fname, tmpStr;
	std::string MatrixStr;

	int order, subOrder, cur_amount, cur_amount_nl, size, mode;
	int amountOfPoints, term_amount[10][3];//с запасом

	configReader myConfigReader;
	myConfigReader.openConfigFile("config.txt");
	amountOfPoints = myConfigReader.readIntWithHeader();
	order= myConfigReader.readIntWithHeader();
	mode= myConfigReader.readIntWithHeader();
	myConfigReader.closeConfig();

	myConfigReader.openConfigFile(fileNamePrinter::getPathToConfigFile());
	myConfigReader.readRouteAmounts(term_amount,1,order);
	myConfigReader.closeConfig();

	myConfigReader.openPointsFile("points.txt");
	myConfigReader.readPoints(my_points);
	myConfigReader.closePointsFile();

	
	int total = 0;
	std::ofstream outSystem;

	//ƒќƒ≈Ћј“№!!!!!!!!!!!!!!!!!!!
	//пока просо копирует файлы, в идеале должно записывать разные файлы в один

	int step = 50;//количество слагаемых через которое надо начинать новый файл
	for (int jj = 0; jj<amountOfPoints; jj++)//перебираем все точки
	{
		
		//temp
		std::string temp;
		temp = fileNamePrinter::getPathToMainMathematicaFiles(my_points[jj], order);

		//end temp
		math_out.open(fileNamePrinter::getPathToMainMathematicaFiles(my_points[jj],order), std::ios::out);
	

		std::ifstream sysIn(fileNamePrinter::getPathToSystems(order + 1), std::ios::in);

		
		while (!sysIn.eof())
		{
			getline(sysIn, temp);
			if (temp.length()>0)
				math_out << temp << "\n";
		}
		sysIn.close();
		
		if (mode == 1)//full mode
			subOrder = 1;
		else
			subOrder = order;
		for (; subOrder <= order; subOrder++)
		{
			total = 0;
			if (subOrder<6)
				step = 50;
			else if (subOrder >= 6)
				step = 20;
			
			//добавл€ем в авто файл

			math_out << "nb1= NotebookOpen[StringJoin[{NotebookDirectory[], \""<< fileNamePrinter::getFileNameOfMathematicaFile(subOrder,my_points[jj], total / step)<< "\"}]];\n";
			math_out << "NotebookEvaluate[nb1];\n";
			math_out << "NotebookClose[nb1];\n";
			//конец добавки

			
			out.open(fileNamePrinter::getPathToMathematicaFiles(subOrder, my_points[jj], total / step), std::ios::out);


			for (int i = 1; i <= subOrder; i++)
			{
				std::cout << my_points[jj] << " " << i << " type 0\n";
				if ((i == 1 && subOrder == 1) || (i>1)) //слагаемые с одним оператором возмущени€ есть только в первом пор€дке
				{
					define_term(math_out, my_points[jj], 0, subOrder, i, total, out, term_amount, step);
				}

				//все тоже самое, только дл€  матриц type1
				std::cout << my_points[jj] << " " << i << " type 1\n";
				define_term(math_out, my_points[jj], 1, subOrder, i, total, out, term_amount, step);


				//все тоже самое, только дл€  матриц type2
				std::cout << my_points[jj] << " " << i << " type 2\n";
				define_term(math_out, my_points[jj], 2, subOrder, i, total, out, term_amount, step);


			}
			out.close();
		}
		math_out.close();
	}
	return 0;
}

