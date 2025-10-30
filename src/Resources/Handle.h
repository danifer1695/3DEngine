#pragma once

struct Handle
{
	unsigned int index = 0;			//used to access an object pool's element's index
	unsigned int generation = 0;	//used to detect stale/outdated handles since they will be reused

	//Constructors
	Handle() {};
	Handle(unsigned int index, unsigned int generation)
		:index{ index }, generation{ generation } {};

	//overloaded equality operator
	bool operator== ( const Handle & lhs ) const
	{
		return index == lhs.index && generation == lhs.generation;
	}
};

