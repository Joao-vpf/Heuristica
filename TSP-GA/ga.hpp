#pragma once
#include "library.hpp"
#include "gene.hpp"


class genetic_main
{
	/*
	In computer science and operations research, a genetic algorithm (GA) is a metaheuristic inspired by the process of natural
	selection that belongs to the larger class of evolutionary algorithms (EA).
	*/
	ull** matrix_adj;
	ll min_cust;
	int nodes;
	params params_active_ga;
	
	vector<gene> generation;

	void init_parents()
	{
		generation = vector<gene>(params_active_ga.max_population);
		for (int i = 0; i < nodes; i++)
		{
			generation[i].init(nodes);
			generation[i].random_gene((params_active_ga.fix==-1 ? i : params_active_ga.fix), matrix_adj);
		}

		for (int i = nodes; i < params_active_ga.max_population; i++)
		{
			generation[i].init(nodes);
			generation[i].random_gene((params_active_ga.fix == -1 ? random_range(0, nodes) : params_active_ga.fix), matrix_adj);
		}
	}

	void print_verbose(int idx_generation)
	{
		if (params_active_ga.simple)
		{
			cout << "Generation: "<< idx_generation <<  " Min_cust : " << min_cust << endl;
		}
		else
		{
			cout << "Generation: " << idx_generation << " Min_cust : " << min_cust << endl;
			cout << "Best_path: ";
			generation[0].get_path();
			cout << endl;
		}
	}

	void simulation()
	{
		for (int gen = 1; gen <= params_active_ga.max_generations; gen++)
		{
			sort(generation.begin(), generation.end());
			min_cust = min(min_cust, generation[0].get_cust());

			int tx_insert_elite = (int)floor(params_active_ga.max_population / ((int)floor(100 / params_active_ga.elite)));
			vector<gene> new_generation(params_active_ga.max_population);

			for (int i = 0; i < tx_insert_elite; i++)
			{
				new_generation[i] = generation[i];
			}

			for (int i = tx_insert_elite; i < params_active_ga.max_population; i++)
			{
				int father = random_range(0, params_active_ga.max_population/params_active_ga.balance);
				int mother = random_range(0, params_active_ga.max_population/params_active_ga.balance);
				new_generation[i].children(generation[father], generation[mother], matrix_adj, params_active_ga);
				return;
			}
			
			generation = new_generation;

			if (params_active_ga.verbose and gen%1000==0)
			{
				print_verbose(gen/1000);
			}
		}
	}

	void config_param(ifstream& input_param)
	{
		string param;
		int value;
		while(input_param >> param >> value)
		{
			if (param == "max_generations" and value > -1)
			{
				params_active_ga.max_generations = value;
			}
			if (param == "verbose")
			{
				params_active_ga.verbose = (bool)value;
			}
			if (param == "simple")
			{
				params_active_ga.simple = (bool)value;
			}
			if (param == "elite" and value > -1 and value<=100)
			{
				params_active_ga.elite = value;
			}
			else if (param == "max_population" and value>=10)
			{
				params_active_ga.max_population = value;
			}
			else if (param == "tx_parents" and value >= 4)
			{
				params_active_ga.tx_parents = (value+4)%100;
			}
			else if (param == "fix" and value >= 0 and value <= nodes)
			{
				params_active_ga.fix = value;
			}
			else if (param == "balance" and value >= 1 and value<=params_active_ga.max_population)
			{
				params_active_ga.balance = value;
			}
			else if (param == "alpha" and value >= 2 and value<=100)
			{
				params_active_ga.alpha = value;
			}
			else if (param == "BCR" and value == 0)
			{
				auto idx = find(params_active_ga.cross_activate_vector.begin(), 
				params_active_ga.cross_activate_vector.end(), "BCR");
				if (idx != params_active_ga.cross_activate_vector.end())
					params_active_ga.cross_activate_vector.erase(idx);
			}
			else if (param == "AHCAVG" and value == 0)
			{
				auto idx = find(params_active_ga.cross_activate_vector.begin(), 
				params_active_ga.cross_activate_vector.end(), "AHCAVG");
				if (idx != params_active_ga.cross_activate_vector.end())
					params_active_ga.cross_activate_vector.erase(idx);
			}
		}
	}

	public:
		genetic_main(ull** input_matrix, int input_nodes, string param)
		{
			matrix_adj = input_matrix;
			min_cust = INF;
			nodes = input_nodes;
			ifstream input_param(param);
			config_param(input_param);
		}

		~genetic_main()
		{
		}

		ull active()
		{
			init_parents();
			simulation();
			return min_cust;
		}
};
