#include "stdafx.h"
#include "aOperators.h"
#include "LadderOperator.h"
#include "cos.h"

bool bilinearFourierLadderOperators::operator==(const bilinearFourierLadderOperators sec) const
{
	if (names_.size() != sec.names_.size())
		return false;
	for (int i = 0; i < names_.size(); i++)
	{
		if (types_[i] != sec.types_[i])
			return false;
		if (names_[i] != sec.names_[i])
			return false;
		if (nodes_[i] != sec.nodes_[i])
			return false;
	}
	return true;
} 

bool bilinearFourierLadderOperators::operator<(const bilinearFourierLadderOperators sec) const
{
	if (names_.size() < sec.names_.size()) return true;
	else if (names_.size() > sec.names_.size()) return false;
	for (int i = 0; i < names_.size(); i++)	{
		if (names_[i] < sec.names_[i]) return true;
		else if (names_[i] > sec.names_[i]) return false;
	
		if (nodes_[i] < sec.nodes_[i]) return true;
		else if (nodes_[i] > sec.nodes_[i]) return false;
	}
	return false;
}

void bilinearFourierLadderOperators::parseFromString(std::string inpName)
{
	clear();
	std::string opName;
	while (inpName.length() >= 3)
	{
		opName = inpName.substr(0,2);
		
		if (std::find(LadderOperator::OpNames.begin(), LadderOperator::OpNames.end(), opName) != LadderOperator::OpNames.end())
		{
			inpName = inpName.substr(2);
			int i = 0;
			for (; i < inpName.length(); i++)
			{
				if (!(inpName[i] <= '9'&&inpName[i] >= '0'))
					break;
			}
			if (i > 0)
			{
				types_.push_back(opName[0]);
				names_.push_back(opName[1]);
				nodes_.push_back(std::stoi(inpName.substr(0, i)));
			}
			inpName = inpName.substr(i);
		}
		else
			break;
	}
}

void bilinearFourierLadderOperators::printAterm(std::ofstream &F, int **m, int size, bool if_print_coeff)const
{
	if (names_.size() == 2)
	{
		if (if_print_coeff)
			F << coefficient_ << "*";
		if (names_[0] != names_[1])
			F << "G_{";
		else if (names_[0] == 'm')
			F << "F";
		else if (names_[0] == 'p')
			F << "Fp";
		else
			F << "\n\n Strange ops \n\n";
		if (nodes_[0] != nodes_[1])
		{
			//if (node[0]!=0)
			//	F << "\n\n Strange nodes \n\n";
			int da1, db1, da2, db2;
			Cos::findCos( nodes_[0], da1, db1);
			Cos::findCos( nodes_[1], da2, db2);
			F << "*Cos[";
			switch ((da2 - da1)){
			case 0: break;
			case -1: F << "-ka"; break;
			case 1: F << "ka"; break;
			default: F << "ka*" << (da2 - da1);
			}
			switch ((db2 - db1)){
			case 0: break;
			case -1: F << "-kb"; break;
			case 1: F << "+kb"; break;
			default: F << "+kb*" << (db2 - db1);
			}
			F << "]";
		}
	}
	else
	{
		F << "\n\n Strange length \n\n";
	}
}

void bilinearFourierLadderOperators::clear()
{
	names_.clear();
	nodes_.clear();
	double coeff = 0;
	int order = 0;
}

//Hash functions for unordered maps
namespace std {
	template <>
	struct hash<bilinearFourierLadderOperators>
	{
		std::size_t operator()(const bilinearFourierLadderOperators& k) const
		{
			std::ostringstream out;
			out << " " << k.coefficient_ << " " << k.names_.size();
			for (auto elem : k.names_)
				out << elem << " ";
			for (auto elem : k.nodes_)
				out << elem << " ";
			return (std::hash<string>()(out.str()));
		}
	};
}