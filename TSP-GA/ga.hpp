#pragma once
#include "library.hpp"

class gene
{
	int nodes;
	vector<int> path;
	vector<int> vis;
	ll cust;

	int mutant_gene()
	{
		return random_range(0, nodes-1);
	}

	void fx_cust(ull** matrix_adj)
	{
		cust = 0;
		ll cust_inf = INF;
		bool mutant = false;
		for (int i = 1; i < nodes - 1; i++)
		{
			if ((i == path[0] and vis[i] == 2) or (vis[i] == 1))
			{
				cust += matrix_adj[path[i - 1]][path[i]];
				cust_inf -= matrix_adj[path[i - 1]][path[i]];
			}
			else
			{
				mutant = true;
			}
		}
		if (mutant)
		{
			cust = cust_inf;
		}
	}

	public:
		gene()
		{
			nodes = -1;
			cust = INF;
		}

		~gene()
		{
		}

		void init(const int& input_nodes)
		{
			nodes = input_nodes+1;
			path = vector<int>(nodes,0);
			vis = vector<int>(nodes,0);
			cust = INF;
		}

		void random_gene(const int& initial, ull** matrix_adj)
		{
			path[0] = initial;
			vis[initial]++;
			for (int i = 1; i < nodes-1; i++)
			{
				int idx = random_range(0, nodes-1);
				while (vis[idx])
				{
					idx = random_range(0, nodes-1);
				}
				path[i] = idx;
				vis[idx]++;
			}
			path[nodes-1] = initial;
			fx_cust(matrix_adj);
		}

		void get_path()
		{
			for (int i = 0; i < nodes; i++)
			{
				cout << path[i] << (i + 1 < nodes ? " " : ".");
			}
		}

		int get_nodes()
		{
			return nodes-1;
		}

		int get_microgene(const int& idx)
		{
			return path[idx];
		}

		void insert_gene(const int& idx, const int& micro_gene)
		{
			path[idx] = micro_gene;
			vis[path[idx]]++;
		}

		void children(gene& father,gene& mother, const int& tx_parents, ull** matrix_adj, int fix = -1)
		{
			int father_gene= tx_parents/2, mom_gene = tx_parents;
			this->init(father.get_nodes());
			
			int rate = random_range(0, 100);
			if (fix != -1)
			{
				rate = random_range(0, mom_gene);
			}

			if (rate <= father_gene)
			{
				this->insert_gene(0, father.get_microgene(0));
			}
			else
			{
				if (rate <= mom_gene)
				{
					this->insert_gene(0, mother.get_microgene(0));
				}
				else
				{
					this->insert_gene(0, mutant_gene());
				}
			}

			for (int i = 1; i < nodes-1; i++) 
			{
				rate = random_range(0, 100);

				if (rate <= father_gene)
				{
					this->insert_gene(i, father.get_microgene(i));
				}
				else
				{
					if (rate <= mom_gene)
					{
						this->insert_gene(i, mother.get_microgene(i));
					}
					else
					{
						this->insert_gene(i, mutant_gene());
					}
				}
			}
			this->insert_gene(nodes - 1, path[0]);
			this->fx_cust(matrix_adj);
		}

		const bool operator<(gene& b)
		{
			return this->cust < b.get_cust();
		}

		const ll get_cust()
		{
			return cust;
		}

};

class genetic_main
{
	/*
	In computer science and operations research, a genetic algorithm (GA) is a metaheuristic inspired by the process of natural
	selection that belongs to the larger class of evolutionary algorithms (EA).
	*/
	ull** matrix_adj;
	ll min_cust;
	int nodes;
	int max_generations;
	int max_population;
	bool verbose;
	bool simple;
	int elite;
	int tx_parents;
	int balance;
	int fix;
	vector<gene> generation;

	void init_parents()
	{
		generation = vector<gene>(max_population);
		for (int i = 0; i < nodes; i++)
		{
			generation[i].init(nodes);
			generation[i].random_gene((fix==-1 ? i : fix), matrix_adj);
		}

		for (int i = nodes; i < max_population; i++)
		{
			generation[i].init(nodes);
			generation[i].random_gene((fix == -1 ? random_range(0, nodes) : fix), matrix_adj);
		}
	}

	void print_verbose(int idx_generation)
	{
		if (simple)
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
		for (int gen = 1; gen <= max_generations; gen++)
		{
			sort(generation.begin(), generation.end());
			min_cust = min(min_cust, generation[0].get_cust());

			int tx_insert_elite = (int)floor(max_population / ((int)floor(100 / elite)));
			vector<gene> new_generation(max_population);

			for (int i = 0; i < tx_insert_elite; i++)
			{
				new_generation[i] = generation[i];
			}


			for (int i = tx_insert_elite; i < max_population; i++)
			{
				int father = random_range(0, max_population/balance);
				int mother = random_range(0, max_population/balance);
				new_generation[i].children(generation[father], generation[mother], tx_parents, matrix_adj, fix);
			}

			generation = new_generation;

			if (verbose and gen%1000==0)
			{
				print_verbose(gen/1000);
			}
		}
	}

	void config_param(ifstream& input_param)
	{
		max_generations = 100;
		max_population = 100;
		verbose = false;
		simple = true;
		elite = 10;
		fix = -1;
		tx_parents = 90;
		balance = 1;
		string param;
		int value;
		while(input_param >> param >> value)
		{
			if (param == "max_generations" and value > -1)
			{
				this->max_generations = value;
			}
			if (param == "verbose")
			{
				this->verbose = (bool)value;
			}
			if (param == "simple")
			{
				this->simple = (bool)value;
			}
			if (param == "elite" and value > -1)
			{
				this->elite = value%100;
			}
			else if (param == "max_population" and value>=10)
			{
				this->max_population = value;
			}
			else if (param == "tx_parents" and value >= 4)
			{
				this->tx_parents = (value+4)%100;
			}
			else if (param == "fix" and value >= 0 and value <= nodes)
			{
				this->fix = value;
			}
			else if (param == "balance" and value >= 1 and value<=max_population)
			{
				this->balance = value;
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