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
			for (int i = 1; i < nodes; i++)
			{
				if ((path[i] == path[0] and vis[i] == 2) or (vis[i] == 1))
				{
					cust += matrix_adj[path[i - 1]][path[i]];
					if (matrix_adj[path[i - 1]][path[i]] != INF)
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
			vis[nodes - 1]++;
			repath[initial] = nodes-1;
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
