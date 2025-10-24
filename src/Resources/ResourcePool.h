#pragma once

#include "Handle.h"

#include<vector>
#include<queue>

template <typename T>
class ResourcePool
{
private:
	std::vector<T> resources;				//the actual resources we want to pool (textures, models, ...)
	std::vector<unsigned int> generations;	//equal in size to 'resources' to match indices
	std::queue<unsigned int> freeList;		//to keep track of free indices within the 'resources' vector

public:

	//=============================================================================================
	//add()
	//=============================================================================================

	//Add a new element to the pool, return a handle to it.
	//Its like when you go to a restaurant, you leave your coat at the coat check and are
	//given a ticket to be able to retrieve it whenever you want.
	Handle Add(T&& resource)
	{
		unsigned int index;

		//check if there are any freed up spaces within 'resources'
		if (!freeList.empty())
		{
			index = freeList.pop();
			resources[index] = std::move(resource);
		}
		else
		{
			index = resources.size();
			resources.emplace_back(std::move(resource));
			generations.push_back(0);
		}

		return Handle(index, generations[index]);
	}

	//=============================================================================================
	//Get()
	//=============================================================================================
	T* Get(const Handle& handle)
	{
		//if index is larger than the current pool, requested obj never existed
		if (handle.index >= resources.size()) return nullptr;

		//return nullptr if the handle is obsolete (generations dont match)
		if (generations[handle.index] != handle.generation) return nullptr;	

		return &resources[handle.index];
	}

	//=============================================================================================
	//Remove()
	//=============================================================================================
	void Remove(const Handle& handle)
	{
		//break if handle is invalid
		if (handle.index >= resources.size()) return;

		//increase generations once an element has been removed
		generations[handle.index]++;

		//add remaining index to the list(queue) of free spaces
		freeList.push(handle.index);
	}
};

