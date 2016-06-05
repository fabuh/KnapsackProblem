#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <time.h>
#include "Knapsack.h"

//statistically solving the Knapsack problem.
//cap 100, should give an answer of 400

using namespace std;

//A helper function which promotes the items in a Knapsack.
void promoteItems(Knapsack& bag, float promotion, Item* all_items, size_t count)
{
	for (size_t i = 0; i < count; i++)
	{
		if (bag.includes(all_items[i].name))
			all_items[i].priority *= promotion;
	}
}

//quick sort, for sorting the items by their priority
int partition(Item* all_items, int start, int end)
{
	float splitter = all_items[end].priority;
	int index = start;

	for (int i = start; i < end; i++)
	{
		if (all_items[i].priority < splitter)
		{
			swap<Item>(all_items[i], all_items[index]);
			index++;
		}
	}
	swap<Item>(all_items[index], all_items[end]);
	return index;
}

void quickSort(Item* all_items, int start, int end)
{
	if (start < end)
	{
		int l = partition(all_items, start, end);
		quickSort(all_items, start, l - 1);
		quickSort(all_items, l + 1, end);
	}
}

//checking which is the lightest item not added to the knapsack
size_t lightestNotAdded(Knapsack& bag, Item* all_items, size_t count)
{
	size_t lightest = all_items[0].size;
	for (size_t i = 0; i < count; i++)
	{
		if (!bag.includes(all_items[i].name))
		{
			if (lightest > all_items[i].size)
				lightest = all_items[i].size;
		}
	}
	return lightest;
}

//filling a knapsack with the best possible items by their priority
void fillWithBest(Knapsack& bag, size_t cap, Item* all_items, size_t count)
{
	quickSort(all_items, 0, count - 1);
	size_t i = 0;

	while (bag.getWeight() + all_items[i].size <= cap)
	{
		bag.addItem(all_items[i]);
		i++;
	}
}

//filling a knapsack with random elements from all the items
void randomFillKnapsack(Knapsack& bag, size_t cap, Item* all_items, size_t count)
{
	size_t lightest = lightestNotAdded(bag, all_items, count);
	while (bag.getWeight() + lightest <= cap)
	{
		srand(time(0));
		int random_item = rand() % count;
		if (all_items[random_item].size + bag.getWeight() <= cap && !bag.includes(all_items[random_item].name))
		{
			bag.addItem(all_items[random_item]);
			if (all_items[random_item].size == lightest)
				lightest = lightestNotAdded(bag, all_items, count);
		}
	}
}

//the magic
void solver(ifstream& in)
{
	Knapsack bag;
	size_t cap, count;
	in >> cap >> count;
	Item* all_items;
	all_items = new Item[count];
	for (size_t i = 0; i < count; i++)
	{
		in >> all_items[i].name >> all_items[i].size >> all_items[i].price;
	}

	size_t resultSize = 0;
	size_t* result = new size_t[count*count];
	size_t cycles = 0;
	float min = 0.0;

	randomFillKnapsack(bag, cap, all_items, count);
	min = (float)bag.getValue() / cap;
	result[0] = bag.getValue();
	cout << "The first knapsack is with a value of " << bag.getValue() << ".\n";

	while (resultSize < count*count - 1 && cycles < count*3 )
	{
		bag.empty_bag();
		randomFillKnapsack(bag, cap, all_items, count);
		promoteItems(bag, ((float)(bag.getValue() / cap))/min, all_items, count);

		if (bag.getValue() > result[resultSize])
		{
			resultSize++;
			result[resultSize] = bag.getValue();
			cout << "We have found a knapsack with a higher value and it is " << bag.getValue() << ".\n";
		}
		cycles++;
	}

	bag.empty_bag();
	fillWithBest(bag, cap, all_items, count);
	cout << "The bast value possible knapsack with the given space is " << bag.getValue() << ".\n";
	delete[] result;
}



int main()
{
	ifstream in("items.txt");

	solver(in);

	system("pause");
	return 0;
}