// FillMatrix.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <math.h>
#include <string>
#include <fstream>
#include <sstream>
#include<set>
#include<vector>
#include<algorithm>
#include "fileNamePrinter.h"

const int maxSpins = 10;


class Names
{
	std::set<std::string> names;
public:
	void add(std::string name)
	{
		names.insert(name);
	}
	void printToFile(std::ofstream &out)
	{
		//sort(names.begin(), names.end());
		out<<"{";
		int i=0;
		int amount = names.size();
		for (auto &curName: names)
		{
			out << curName;
			if (i < amount - 1)
				out << ",";
			++i;
		}
		out << "};\n";
	}
};
void setTripletSpins(int spins[], int Nconf, int size)
{
	int tempSpins[maxSpins];
	int singletFactor = (int)pow(2, size - 1);
	int curN = Nconf%(singletFactor);
	for (int j = 0; j <size; j++)
	{
		if (curN % 2 == 1)
			tempSpins[j] = 1;
		else
			tempSpins[j] = 0;
		curN = curN / 2;
	}
	int place = (Nconf / singletFactor)/3;
	int type = (Nconf / singletFactor) % 3;
	int cur = 0;
	for (int i = 0; i < size; i++)
	{
		if (i != place)
		{
			spins[i] = tempSpins[cur++];
		}
		else
			spins[i] = 2 + type; //двойка нужна чтобы отличать состояния триплетов от синглетов
	}
}
///i,j- 1 or 0,   return: 1- sz+,  2-sz-, 3-sp, 4-sm

void setMask(int mask[], int Nconf, int N)//преобразует число в двоичную маску
{
	for (int j = 0; j <= N; j++)
	{
		mask[j] = Nconf & 1;//остаток от деления на 2
		Nconf=Nconf >> 1; //деление на 2
	}
}


std::string retTriplet(int i, int j,int num)
{
	char c = '0';
	std::ostringstream s;
	if (i == 0 && j == 0) s << "1";
	if (i == 0 && j == 1) s << "ap" << (char)(c + num);
	if (i == 0 && j == 2) s << "bp" << (char)(c + num);
	if (i == 0 && j == 3) s << "cp" << (char)(c + num);
	if (i == 0 && j == 4) s << "dp" << (char)(c + num);

	if (i == 1 && j == 0) s << "am" << (char)(c + num);
	if (i == 1 && j == 1) s << "aN" << (char)(c + num);//1;
	if (i == 1 && j == 2) s << "am" << (char)(c + num) << "bp" << (char)(c + num);
	if (i == 1 && j == 3) s << "am" << (char)(c + num) << "cp" << (char)(c + num);
	if (i == 1 && j == 4) s << "am" << (char)(c + num) << "dp" << (char)(c + num);

	if (i == 2 && j == 0) s << "bm" << (char)(c + num);
	if (i == 2 && j == 1) s << "bm" << (char)(c + num) << "ap" << (char)(c + num);
	if (i == 2 && j == 2) s << "bN" << (char)(c + num);
	if (i == 2 && j == 3) s << "bm" << (char)(c + num) << "cp" << (char)(c + num);
	if (i == 2 && j == 4) s << "bm" << (char)(c + num) << "dp" << (char)(c + num);

	if (i == 3 && j == 0) s << "cm" << (char)(c + num);
	if (i == 3 && j == 1) s << "cm" << (char)(c + num) << "ap" << (char)(c + num);
	if (i == 3 && j == 2) s << "cm" << (char)(c + num) << "bp" << (char)(c + num);
	if (i == 3 && j == 3) s << "cN" << (char)(c + num);
	if (i == 3 && j == 4) s << "cm" << (char)(c + num) << "dp" << (char)(c + num);

	if (i == 4 && j == 0) s << "dm" << (char)(c + num);
	if (i == 4 && j == 1) s << "dm" << (char)(c + num) << "ap" << (char)(c + num);
	if (i == 4 && j == 2) s << "dm" << (char)(c + num) << "bp" << (char)(c + num);
	if (i == 4 && j == 3) s << "dm" << (char)(c + num) << "cp" << (char)(c + num);
	if (i == 4 && j == 4) s << "dN" << (char)(c + num);
	return s.str();
}

std::string printBilinear(int spins[],int amount,Names &names)
{
	std::ostringstream out;
	std::string opType;
	std::string result;
	out.str("");
	opType = "";
	char c = '0';
	bool first = true;
	for (int i = 0; i < amount; i++)
	{
		out.str("");
		switch (spins[i])
		{
			case 0: continue;//
			case 1:	opType = "a";	break;
			case 2: opType = "b";	break;
			case 3: opType = "c";  break;
			case 4: opType = "d";	break;
		}
		if (!first)
			result+='+';
		first = false;

		out << opType <<"p" << (char)(c + i) << opType <<"m" << (char)(c + i);
		
		names.add(out.str());

		result += out.str();
	}
	return result;
}



int _tmain(int argc, _TCHAR* argv[])
{
	
	int power =5;//количество спинов
	int size = 3 * power*(int)pow((double)2, (int)power - 1);
	
	int *spins = new int[power];
	int *spinsOut = new int[power];
	int *mask = new int[power];
	int *groundStatesMask = new int[power];
	
	int mode = 2;

	if (mode == 1)
	{
		std::ofstream outspins(fileNamePrinter::getPathToSpinsOrder(power), std::ios::out);
		size = 3 * power * (int)pow(2, power - 1);
		for (int j = 0; j < size; j++)
		{
			setTripletSpins(spins, j, power);
			for (int i = 0; i < power; i++)
			{
				outspins << spins[i] << " ";
			}
			outspins << "\n";
		}
		return 0;
	}
	

	
	if(mode==2)
	{
		std::ofstream out(fileNamePrinter::getPathToSystems(power), std::ios::out);
		for (int curP = 1; curP <= power; curP++)
		{
			Names curOrderNames;
			size = 3 * curP*(int)pow((double)2, (int)curP - 1);

			for (int j = 0; j < power; j++)
			{
				spins[j] = 0;
				spinsOut[j] = 0;
				mask[j] = 0;
			}
			
			std::string s, temp, temp2;
			bool flag;
			char c = '0';
			
			out << "\nSys" << curP << "={";
			for (int i = 0; i < size; i++)
			{
				setTripletSpins(spins, i, curP);
				out << "{";
				std::cout << i <<"\n";
				for (int j = 0; j < size; j++)
				{
					setTripletSpins(spinsOut, j, curP);
					s.clear();
					
					for (int tt = 0; tt < curP; tt++)
					{
						if (spins[tt] == 0 && spinsOut[tt] == 0)
							groundStatesMask[tt] = 1;
						else
							groundStatesMask[tt] = 0;
					}

					std::set<std::string> names;
					for (int k = 0;k<= (int)pow(2, curP) - 1;k++)//перебераем маски
					{
						setMask(mask, k, curP);
						flag = false;
						for (int tt = 0; tt < curP; tt++)
							if (groundStatesMask[tt] == 1 && mask[tt] == 1)
								flag = true;
						if (flag)
							continue;
						flag = true;
						temp.clear();
						
						for (int l = 0; l < curP; l++)
						{
							if (mask[l] == 0) //Можно ли не вставлять операторы для данной пары спинов
							{
								if (!(spins[l] == spinsOut[l]))
								{
									flag = false;
									break;
								}
							}
							else //Вставляем операторы для данной пары спинов если нужно 
							{
								temp2 = retTriplet(spins[l], spinsOut[l], l);
								if (temp2.compare("1") != 0)
									temp += temp2;
							}

						}
						if (flag)
						{							
							if (temp.length() > 0&&temp.find_first_of("bcd") != std::string::npos)
							{
								names.insert(temp);
							}
						}
					}
					int curn = 0;
					for (auto& elem:names)
					{ 
						curOrderNames.add(elem);
						out << elem;
						if (curn != names.size() - 1)
							out << "+";
						++curn;
					}
					
					if (j == size - 1)
						out << "}";
					else
						out << ",";
				}	
				if (i == size - 1)
					out << "};";
				else
					out << ",\n";
			}
			out << "\n\nVar" << curP << "=";
			curOrderNames.printToFile(out);
		}
		out.close();
	}
	return 0;
	
}

