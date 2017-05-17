// Define Systems.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"



const std::string sys = "sys";
const std::string input = "input";
const std::string res = "results";

const std::string math_res = "results";
const std::string delim = "\\";
const std::string math_delim = "\\\\";
std::string root_path;

std::vector<std::string> my_points;
const int J1 = 1;

void define_term(std::ofstream &math_out, std::string &point, int type, int order, int subOrder, int &Total, std::ofstream &out, int term_amount[][3], int step)
{
	std::ostringstream fname, tmpStr;
	std::ifstream cur_f;
	std::string MatrixStr, temp;
	int size;

	for (int j = 1; j <= term_amount[subOrder][type]; j++)
	{

		Total++;
		if (Total%step == 0)
		{
			out.close();
			fname.str("");
			fname << res << delim << point << delim << "!_" << order << "_results_J2=" << point << "(" << Total / step << ").txt";
			//добавл€ем в авто файл
			math_out << "nb1= NotebookOpen[StringJoin[{NotebookDirectory[], \"" << "!_" << order << "_results_J2=" << point << "(" << Total / step << ").txt" << "\"}]];\n";
			math_out << "NotebookEvaluate[nb1];\n";
			math_out << "NotebookClose[nb1];\n";
			//конец добавки
			out.open(fname.str(), std::ios::out);
		}
		
		fname.str("");
		fname << type;
		temp = fname.str();
		
		fname.str("");
		fname << root_path << "results_" << temp << delim << order << "." << subOrder << delim << order << "_" << subOrder << "_" << j << "_res_" << temp << ".txt";
		cur_f.open(fname.str(), std::ios::in);
		
		cur_f >> size;
		tmpStr.str("");
		tmpStr << "res" << temp << "$" << order << "$" << subOrder << "$" << j;
		out << tmpStr.str() << "=";
		while (!cur_f.eof())
		{
			getline(cur_f, MatrixStr);
			if (MatrixStr.length()>0)
				out << MatrixStr;
		}
		out << ";\n";
		out << "Eff" << tmpStr.str() << "=Expand[Chop[Solve[Sys";
		switch (size)
		{
		case 3:	  temp = "1"; break;
		case 12:  temp = "2"; break;
		case 36:  temp = "3"; break;
		case 96:  temp = "4"; break;
		case 240: temp = "5"; break;
		case 576: temp = "6"; break;
		}
		out << temp << "==" << tmpStr.str() << "/.{J1->1,J2->" << point << "},Var" << temp << "]]];\n";
		out << "tt = OpenWrite[\"D:" << math_delim << math_res << math_delim << point << math_delim << tmpStr.str() << "_" << point << ".txt\"];\n";
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

	std::ifstream conf("config.txt", std::ios::in);
	std::string temp;

	std::ofstream out;
	std::ofstream math_out;

	std::ifstream cur_f;
	std::ostringstream fname, tmpStr;
	std::string MatrixStr;

	int order, subOrder, cur_amount, cur_amount_nl, size, mode;
	int amount, term_amount[10][3];//с запасом

	getline(conf, temp);
	getline(conf, root_path);
	getline(conf, temp);
	conf >> amount;
	getline(conf, temp);
	getline(conf, temp);
	conf >> order;
	getline(conf, temp);
	getline(conf, temp);
	conf >> mode;
	getline(conf, temp);
	getline(conf, temp);

	for (int i = 1; i <= order; i++)
	{
		conf >> subOrder >> term_amount[i][0] >> term_amount[i][1] >> term_amount[i][2];
	}
	conf.close();


	std::ifstream my_points_f("points.txt", std::ios::in);
	for (int jj = 1; jj <= amount; jj++)
	{
		getline(my_points_f, temp);
		my_points.push_back(temp);
	}
	my_points_f.close();

	int Total = 0;
	std::ofstream out_sys;

	//ƒќƒ≈Ћј“№!!!!!!!!!!!!!!!!!!!
	//пока просо копирует файлы, в идеале должно записывать разные файлы в один

	int step = 50;//количество слагаемых через которое надо начинать новый файл
	for (int jj = 0; jj<amount; jj++)//перебираем все точки
	{
		fname.str("");
		fname << res << delim << my_points[jj] << delim << order << "_" << my_points[jj] << "general_math.txt";
		math_out.open(fname.str(), std::ios::out);
		fname.str("");
		fname << sys << delim << order + 1<< "var_sys" << ".txt";
		std::ifstream sysIn(fname.str(), std::ios::in);
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
			Total = 0;
			if (subOrder<6)
				step = 50;
			else if (subOrder >= 6)
				step = 20;
			//else
			//	step=5;
			fname.str("");
			fname << res << delim << my_points[jj] << delim << "!_" << subOrder << "_results_J2=" << my_points[jj] << "(" << Total / step << ").txt";
			//добавл€ем в авто файл

			math_out << "nb1= NotebookOpen[StringJoin[{NotebookDirectory[], \"!_" << subOrder << "_results_J2=" << my_points[jj] << "(" << Total / step << ").txt" << "\"}]];\n";
			math_out << "NotebookEvaluate[nb1];\n";
			math_out << "NotebookClose[nb1];\n";
			//конец добавки
			out.open(fname.str(), std::ios::out);


			for (int i = 1; i <= subOrder; i++)
			{
				std::cout << my_points[jj] << " " << i << " type 0\n";
				if ((i == 1 && subOrder == 1) || (i>1)) //слагаемые с одним оператором возмущени€ есть только в первом пор€дке
				{
					define_term(math_out, my_points[jj], 0, subOrder, i, Total, out, term_amount, step);
				}

				//все тоже самое, только дл€  матриц типа 1
				std::cout << my_points[jj] << " " << i << " type 1\n";
				define_term(math_out, my_points[jj], 1, subOrder, i, Total, out, term_amount, step);


				//все тоже самое, только дл€ n_single матриц
				std::cout << my_points[jj] << " " << i << " type 2\n";
				define_term(math_out, my_points[jj], 2, subOrder, i, Total, out, term_amount, step);


			}
			out.close();
		}
		math_out.close();
	}
	return 0;
}

