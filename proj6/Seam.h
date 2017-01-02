/**
 * Seam.h
 * CMSC 435 Assignment 6 - Digital Photography
 * Written by Christopher Sidell (csidell1)
 *
 * This is just a basic class that defines what a 'seam' is * 
 * 	This class uses a vector as a seam can change size dynamically.
 * 	The cost of using a vector over a static array is negligible, for
 * 	even better optimization you can create a seam with a known length
 * 	so a vector can be allocated of that length. (not done here)
 */

#ifndef SEAM_H
#define SEAM_H

#include <vector>
#include <utility>

class Seam
{
public:
	//Default constructor
	Seam(): cost(0){};

	/**
	 * The seam is defined as an array of two part
	 * 	The first pair is of <coordinates,energy>,
	 * 	Coordinates is just another pair of ints <int,int>
	 *
	 * This effectively holds all points in a seam with coodinates 
	 * 	and energy
	 */
	std::vector< std::pair<std::pair<int,int>,double> > seam;

	//The cost of the seam
	double cost;

	//Coordinates(idx)
	//Returns the coordinates at index i
	std::pair<int,int> coordinates(int i)
	{
		return seam[i].first;
	}

	//Del(idx)
	//Delete entry at index i
	void del(int i)
	{
		//Remove cost from seam
		cost -= seam[i].second;

		//Remove it
		seam.erase(seam.begin()+i);
	}

	//Add(x,y,energy)
	//Add a point to the seam
	void add(int i, int j, double energy)
	{
		//Add cost to seam
		cost += energy;
		
		//Push it to the back
		seam.push_back(std::make_pair(std::make_pair(i,j),energy));
	}
};

#endif