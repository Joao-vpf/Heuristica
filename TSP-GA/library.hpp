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