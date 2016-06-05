#ifndef KNAPSACK_H
#define KNAPSACK_H

#include "string.h"

struct Item
{
	char name[100];
	size_t size;
	size_t price;
	float priority;

	Item() :priority(1) { }

	bool operator>(Item& other)
	{
		if (this->priority > other.priority)
			return true;
		return false;
	}
};

class Knapsack
{
public:
	Knapsack():items(nullptr), maxSize(1) { }
	Knapsack(Knapsack&);
	Knapsack& operator=(Knapsack&);
	~Knapsack() { delete[] items; }

	void addItem(Item);
	bool includes(const char*);
	void empty_bag();

	size_t getWeight() { return weight; }
	size_t getValue() { return value; }

private:
	Item* items;
	size_t size;
	size_t maxSize;
	size_t weight;
	size_t value;

	void resize();
};

void Knapsack::empty_bag()
{
	delete[] items;
	items = nullptr;
	size = weight = value = 0;
	maxSize = 1;
}

bool Knapsack::includes(const char* name)
{
	for (size_t i = 0; i < size; i++)
	{
		if (!strcmp(items[i].name, name))
			return true;
	}
	return false;
}

void Knapsack::resize()
{
	Item* new_sized_items;
	new_sized_items = new Item[maxSize*2];
	for (size_t i = 0; i < size; i++)
	{
		new_sized_items[i] = items[i];
	}
	delete[] items;
	items = new_sized_items;
	maxSize *= 2;
}

void Knapsack::addItem(Item item)
{
	if (size >= maxSize)
	{
		resize();
	}
	if (items == nullptr)
	{
		items = new Item[maxSize];
	}
	items[size] = item;
	value += item.price;
	weight += item.size;
	size++;
}

Knapsack::Knapsack(Knapsack& other)
{
	for (size_t i = 0; i < other.size; i++)
	{
		addItem(other.items[i]);
	}
	size = other.size;
	maxSize = other.maxSize;
	weight = other.weight;
	value = other.value;
}

Knapsack& Knapsack::operator=(Knapsack& other)
{
	if (this != &other)
	{
		if (items)
		{
			delete[] items;
		}
		for (size_t i = 0; i < other.size; i++)
		{
			addItem(other.items[i]);
		}
		size = other.size;
		maxSize = other.maxSize;
		weight = other.weight;
		value = other.value;
	}
	return *this;
}

#endif