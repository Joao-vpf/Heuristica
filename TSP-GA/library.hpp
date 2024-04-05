#pragma once
#include <iostream>
#include <string>
#include <time.h>
#include <stdlib.h> 
#include <fstream>
#include <algorithm>
#include <vector>
#include <math.h>

#define ull unsigned long long
#define ll long long
#define INF 10000000000000000
#define endl "\n"
#define input "input.txt"
#define parameters "params.txt"
#define ios ios_base::sync_with_stdio(0);cin.tie(0); cout.tie(0);

using namespace std;


ll random_range(ll start, ll end)
{
	/*
		From start to end (not contained).
	*/
	if (start > end)
	{
		swap(start, end);
	}

	ll range = end - start;
	return start + rand()%range;
}

template <typename T>
T random_range_vector(vector<T>& vet, int start=-1, int end=-1)
{
	/*
		From start to end (not contained).
	*/
	if(start == -1 and end == -1)
	{
		start = 0;
		end = (int)vet.size();
	}
	
	if (start > end)
	{
		swap(start, end);
	}
	int range = end - start;
	int idx = start + rand()%range;
	return vet[idx];
}

struct params
{
	int max_generations;
	int max_population;
	bool verbose;
	bool simple;
	int elite;
	int tx_parents;
	int balance;
	int fix;
	int alpha;
	vector<string> cross_activate_vector;
	
	params() 
	{
		max_generations = 100;
		max_population = 100;
		verbose = false;
		simple = true;
		elite = 10;
		fix = -1;
		tx_parents = 90;
		balance = 2;
		alpha = 2;
		cross_activate_vector = {"BCR", "AHCAVG"};
		
	}
	
	string cross_activate()
	{
		return random_range_vector<string>(cross_activate_vector);
	}
};
