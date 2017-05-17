#include "stdafx.h"
#include "Summator.h"

const double Summator::Treshold_ = 0.00000001;
const int Summator::AllLengthes_ = -1;

Summator::Summator(const std::string _inputRoutesPath, int _curTermLength)
{
	inputRoutesPath_ = _inputRoutesPath;
	curRouteNumber_ = -1;
	curTermLength_ = _curTermLength;
}

void Summator::parseNewRoute(std::string inp)
{
	curRouteNumber_++;
	std::vector<std::pair<int, int>> nodes;
	std::istringstream inpStream;
	int tx, ty;
	char tChar;
	int index = inp.find_first_of('{');
	while (index != std::string::npos)
	{
		inp = inp.substr(index + 1);
		inpStream.str(inp);
		inpStream >> tx;
		inpStream >> tChar;//comma
		inpStream >> ty;
		if (find(nodes.begin(), nodes.end(), std::pair<int, int>(tx, ty)) == nodes.end())
		{
			nodes.push_back(std::pair<int, int>(tx, ty));
		}
		index = inp.find_first_of('{');
	}
	nodesInRoute_ = nodes.size();
}

void Summator::parseNodes(std::string inp, bool isUncut)
//create the set of nodes, order corressponds to input line
//input format for Uncut==false case:
//"i NodeNum NodeNum NodeNum NodeNum ..."
//input format for Uncut==true case:
//"NodeNum NodeNum NodeNum NodeNum ..."
{
	if (isUncut)
		inp = inp.substr(1);
	curRouteNodes_.clear();
	std::istringstream inpStream(inp);
	int tempNode;
	for (int i = 0; i < nodesInRoute_; i++)
	{
		inpStream >> tempNode;
		if (std::find(curRouteNodes_.begin(), curRouteNodes_.end(), tempNode) == curRouteNodes_.end())
			curRouteNodes_.push_back(tempNode);
	}
}

void Summator::addTerm(std::string inp)
{
	std::istringstream inpStream(inp);
	std::string termName, temp;
	Term curTerm;
	double value;

	//read
	inpStream >> termName;

	inpStream >> temp;

	inpStream >> value;

	//parse and add
	if (abs(value) > Treshold_)
	{
		curTerm.parseTerm(termName);
		//substitute numbers of plaquets to numbers of correcponding nodes 
		curTerm.substituteNodes(curRouteNodes_);

		//check if this term should be processed 
		if (curTerm.chekIsZeroNode() && (curTermLength_ == AllLengthes_ || curTerm.getTermLength() == curTermLength_))
		{
			auto iter = results_.find(curTerm);
			if (iter != results_.end())
			{
				iter->second += value;
			}
			else
			{
				results_.insert({ curTerm, value });
			}
		}
	}
}

void Summator::print(std::ofstream &out)
{
	for (auto &it : results_)
	{
		out << it.first.getName() << " " << it.second << "\n";
	}
}

void Summator::clear()
{
	curRouteNodes_.clear();
	results_.clear();
}
