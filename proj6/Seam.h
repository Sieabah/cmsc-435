#ifndef SEAM_H
#define SEAM_H

#include <vector>
#include <utility>

class Seam
{
public:
	Seam(): cost(0){};
	std::vector< std::pair<std::pair<int,int>,double> > seam;
	double cost;
	
	std::pair<int,int> coordinates(int i)
	{
		return seam[i].first;
	}

	void del(int i)
	{
		cost -= seam[i].second;
		seam.erase(seam.begin()+i);
	}

	void add(int i, int j, double energy)
	{
		cost += energy;
		seam.push_back(std::make_pair(std::make_pair(i,j),energy));
	}
};

#endif