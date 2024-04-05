#pragma once
#include "library.hpp"
#include "ga.hpp"

class TSP
{
	/*
	Traveling Salesman Problem (TSP) is the challenge of finding the shortest path or shortest route for a salesperson to take, 
	given a starting point, a number of cities (nodes), and optionally an ending point.
	*/
	ull** matrix_adj;
	int nodes;

	public:
		TSP(ull** input_matrix, int input_nodes)
		{
			matrix_adj = input_matrix;
			nodes = input_nodes;
			srand(time(0));
		}
		
		ull genetic_algorithm(string param)
		{
			genetic_main a(matrix_adj, nodes, param);
			return a.active();
		}

};