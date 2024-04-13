#pragma once
#include "library.hpp"
#include "crossover.hpp"

class gene
{
	int nodes;
	vector<int> path;
	vector<int> repath;
	vector<int> vis;
	ll cust;

	ll fx_cust(ull** matrix_adj, vector<int> path_copy)
	{
		ll cust_copy = 0;
		ll cust_inf = INF;
		bool mutant = false;
		for (int i = 1; i < nodes - 1; i++)
		{
			if (vis[path_copy[i]] == 1)
			{
				cust_copy += matrix_adj[path_copy[i - 1]][path_copy[i]];
				if (matrix_adj[path_copy[i - 1]][path_copy[i]] != INF)
					cust_inf -= matrix_adj[path_copy[i - 1]][path_copy[i]];
			}
			else
			{
				if (matrix_adj[path_copy[i - 1]][path_copy[i]] != INF)
					cust_inf -= matrix_adj[path_copy[i - 1]][path_copy[i]];
				mutant = true;
			}
		}

		if (vis[path_copy[0]] == 2)
		{
			cust_copy += matrix_adj[path_copy[0]][path_copy[1]];
			cust_copy += matrix_adj[path_copy[nodes - 2]][path_copy[0]];
		}
		else
		{
			if (matrix_adj[path_copy[0]][path_copy[1]] != INF)
			{
				cust_inf -= matrix_adj[path_copy[0]][path_copy[1]];
			}
			if (matrix_adj[path_copy[nodes - 2]][path_copy[0]] != INF)
			{
				cust_inf -= matrix_adj[path_copy[nodes - 2]][path_copy[0]];
			}

			mutant = true;
		}

		if (mutant)
		{
			cust_copy = cust_inf;
		}

		return cust_copy;
	}

	void opt_path(ull** matrix_adj,params& params_active_ga)
	{
		for (int i = 0; i < params_active_ga.opt_path_swap_it; i++)
		{
			vector<int> path_copy = path;
			int idxA = random_range(1, nodes - 2);
			int idxB = random_range(1, nodes - 2);

			while (idxB == idxA)
				idxB = random_range(1, nodes - 2);

			if (idxA > idxB)
				swap(idxA, idxB);

			while (idxA < idxB)
			{
				swap(path_copy[idxA], path_copy[idxB]);
				idxA++;
				idxB--;
			}
			ll cust_copy = fx_cust(matrix_adj, path_copy);
			if (cust_copy < cust)
			{
				path = path_copy;
				this->fx_cust(matrix_adj);
			}
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

		void fx_cust(ull** matrix_adj)
		{
			cust = 0;
			ll cust_inf = INF;
			bool mutant = false;
			for (int i = 1; i < nodes-1; i++)
			{
				if (vis[path[i]] == 1)
				{
					cust += matrix_adj[path[i - 1]][path[i]];
					if (matrix_adj[path[i - 1]][path[i]] != INF)
						cust_inf -= matrix_adj[path[i - 1]][path[i]];
				}
				else
				{
					if (matrix_adj[path[i - 1]][path[i]] != INF)
						cust_inf -= matrix_adj[path[i - 1]][path[i]];
					mutant = true;
				}
			}

			if (vis[path[0]] == 2)
			{
				cust += matrix_adj[path[0]][path[1]];
				cust += matrix_adj[path[nodes-2]][path[0]];
			}
			else
			{
				if (matrix_adj[path[0]][path[1]] != INF)
				{
					cust_inf -= matrix_adj[path[0]][path[1]];
				}
				if (matrix_adj[path[nodes - 2]][path[0]] != INF)
				{
					cust_inf -= matrix_adj[path[nodes - 2]][path[0]];
				}

				mutant = true;
			}

			if (mutant)
			{
				cust = cust_inf;
			}
		}

		void init(const int& input_nodes)
		{
			nodes = input_nodes+1;
			repath = vector<int>(nodes, 0);
			path = vector<int>(nodes,0);
			vis = vector<int>(nodes,0);
			cust = INF;
		}

		void random_gene(const int& initial, ull** matrix_adj)
		{
			path[0] = initial;
			repath[initial] = 0;
			vis[initial]++;
			for (int i = 1; i < nodes-1; i++)
			{
				int idx = random_range(0, nodes-1);
				while (vis[idx])
				{
					idx = random_range(0, nodes-1);
				}
				path[i] = idx;
				repath[idx] = i;
				vis[idx]++;
			}
			path[nodes-1] = initial;
			repath[initial] = nodes - 1;
			vis[initial]++;
			fx_cust(matrix_adj);
		}

		void get_path()
		{
			for (int i = 0; i < nodes; i++)
			{
				cout << path[i] << (i + 1 < nodes ? " " : ".");
			}
		}

		void mutation_swap(ull** matrix_adj, params& params_active_ga)
		{
			int idxA = random_range(1, nodes - 2);
			int idxB = random_range(1, nodes - 2);
			swap(this->path[idxA], this->path[idxB]);
			this->fx_cust(matrix_adj);
			opt_path(matrix_adj, params_active_ga);
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
			repath[micro_gene] = idx;
			vis[path[idx]]++;
		}

		void children(gene& father, gene& mother, ull** matrix_adj, params& params_active_ga)
		{
			string activate_cross = params_active_ga.cross_activate();
			if (activate_cross == "BCR")
			{
				crossover<gene>::bcr(*this, father, mother, matrix_adj, params_active_ga);
			}
			else if (activate_cross == "AHCAVG")
			{
				crossover<gene>::arithmetic_average(*this, father, mother, matrix_adj, params_active_ga);
			}
			else if (activate_cross == "CX")
			{
				crossover<gene>::cx(*this, father, mother, matrix_adj, params_active_ga);
			}

		}

		bool has_microgene(int microgene)
		{
			return vis[microgene];
		}

		int idx_microgene(int microgene)
		{
			return repath[microgene];
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
