#include "stdafx.h"
#include "waveFunction.h"


WaveFunction::WaveFunction()
{
	sorted = false;
	nodesAmount = -1;
}

WaveFunction::~WaveFunction()
{
}

void WaveFunction::sortWF()
{
	if (!sorted)
	{
		sort(eigenStates.begin(), eigenStates.end());
		sorted = true;
	}
}

bool WaveFunction::ifSorted()const
{
	return sorted;
}

void WaveFunction::collect( WaveFunction &wf)
{
	
	wf.sortWF();
	
	eigenStates.clear();
	int index = 0;
	eigenStates.push_back(wf.eigenStates[0]);
	for (unsigned int i = 1; i<wf.eigenStates.size(); i++)
	{
		if (wf.eigenStates[i].check(eigenStates[index]))
		{
			eigenStates[index].addToFactor(wf.eigenStates[i].getFactor());
		}
		else
		{
			if (abs(eigenStates[index].getFactor()) <= 0.000000000000001)
			{
				eigenStates.pop_back();
				--index;
			}
			++index;
			eigenStates.push_back(wf.eigenStates[i]);
		}
	}
	sorted = true;
}

void WaveFunction::collect(std::vector<State> &wf)
{
	sort(wf.begin(), wf.end());
	nodesAmount = wf[0].getNodesNumber();
	eigenStates.clear();
	int index = 0;
	eigenStates.push_back(wf[0]);
	for (unsigned int i = 1; i<wf.size(); i++)
	{
		if (wf[i].check(eigenStates[index]))
		{
			eigenStates[index].addToFactor(wf[i].getFactor());
		}
		else
		{
			if (abs(eigenStates[index].getFactor()) <= 0.000000000000001)
			{
				eigenStates.pop_back();
				--index;
			}
			++index;
			eigenStates.push_back(wf[i]);
		}
	}
	sorted = true;
}

void WaveFunction::setNodesAmount(int nodesAmount)
{
	this->nodesAmount = nodesAmount;
}

int WaveFunction::getNodesAmount()const
{
	return nodesAmount;
}

unsigned int WaveFunction::getEigenstatesAmount()const
{
	return eigenStates.size();
}

const State& WaveFunction::getEigenstateByNumber(int stateNumber)const
{
	if (stateNumber < eigenStates.size())
		return eigenStates[stateNumber];
	else
	{
		throw std::invalid_argument("Wrong eigenstate number!");
		return eigenStates[0];
	}
}

void WaveFunction::addEigenState(State newEigenstate)
{
	sorted = false;
	if (nodesAmount < 0)
		nodesAmount = newEigenstate.getNodesNumber();
	
	if(nodesAmount!= newEigenstate.getNodesNumber())
		throw std::invalid_argument("Different amount of nodes in states");
	else
		eigenStates.push_back(newEigenstate);
}

void WaveFunction::clear(int nodeNumVal)
{
	nodesAmount = nodeNumVal;
	eigenStates.clear();
	sorted = false;
}

int WaveFunction::findLastGroup(int startNumber)//return the number of the last element equal to elem with startNumber											///Binary search may give better results in case of big groups, in large groups' amount case not obvious
{
	this->sortWF();

	int last = startNumber;
	for (int i = startNumber + 1; i<eigenStates.size(); i++)
	{
		if (eigenStates[startNumber] == eigenStates[i])
		{
			last = i;
		}
		else
		{
			break;
		}
	}

	return last;

}
