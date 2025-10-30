#pragma once

#include "Handle.h"

#include<vector>
#include<queue>
#include<iostream>

template <typename T>
class ResourcePool
{
private:
	bool dirty = true;
	std::vector<T> resources;				//the actual resources we want to pool (textures, models, ...)
	std::vector<unsigned int> generations;	//equal in size to 'resources' to match indices
	std::queue<unsigned int> freeList;		//to keep track of free indices within the 'resources' vector

	std::vector<bool> active;				//Tracks which 'resource' elements are currently valid

public:

	//=============================================================================================
	//Constructors
	//=============================================================================================
	
	ResourcePool() {};
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
			index = freeList.front();
			freeList.pop();
			resources[index] = std::move(resource);
			active[index] = true;
		}
		else
		{
			index = resources.size();
			resources.emplace_back(std::move(resource));
			generations.push_back(0);
			active.push_back(true);
		}

		dirty = true;
		std::cout << "Resource Added to Pool" << std::endl;
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
		if (!active[handle.index]) return nullptr;

		return &resources[handle.index];
	}

	std::vector<T*> GetAll()
	{
		std::vector<T*> pointers;
		pointers.reserve(resources.size());

		for (unsigned int i = 0; i < (unsigned int)resources.size(); i++)
		{
			//only add active elements
			if (active[i])
				pointers.push_back(&resources[i]);
		}

		return pointers;
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

		//mark this 'resources' index as inactive
		active[handle.index] = false;

		dirty = true;
	}

	//=============================================================================================
	//GetCollection()
	//=============================================================================================
	const std::vector<Handle> GetAllHandles() const
	{
		std::vector<Handle> toReturn;
		toReturn.reserve(resources.size());

		for (unsigned int i = 0; i < resources.size(); i++)
		{
			if(active[i])
				toReturn.push_back({ i, generations[i] });	//return only active handles
		}

		return toReturn;
	}

	//=============================================================================================
	//Get/Set()
	//=============================================================================================
	const bool	GetIsDirty() const			{ return dirty; }

	void		SetIsDirty(bool val)		{ dirty = val; }
};

