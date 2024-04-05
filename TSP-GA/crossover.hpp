#pragma once
#include "library.hpp"
#include "gene.hpp"

template<typename T> 
class crossover
{
	public:
	static void bcr(T& children, T& father,T& mother, ull** matrix_adj,const params& params_active_ga)
	{//Best Cost Route crossover
		int father_gene= params_active_ga.tx_parents/params_active_ga.alpha, nodes = mother.get_nodes();;
		children.init(nodes);
		vector<vector<int>> adj_map(nodes);
		vector<int> dic_father(nodes);
		vector<int> dic_mother(nodes);
		father.get_path();
		cout << endl;
		mother.get_path();
		cout << endl;
		
		for(int i=0; i<nodes; i++)
		{
			dic_father[father.get_microgene(i)] = i;
			dic_mother[mother.get_microgene(i)] = i;
			adj_map[father.get_microgene(i)].push_back(father.get_microgene((i+1)%nodes));
			adj_map[mother.get_microgene(i)].push_back(father.get_microgene((i-1+nodes)%nodes));
			adj_map[mother.get_microgene(i)].push_back(mother.get_microgene((i+1)%nodes));
			adj_map[mother.get_microgene(i)].push_back(mother.get_microgene((i-1+nodes)%nodes));
			//adj_map[(nodes-i-1)].push_back(mother.get_microgene(i-1));
//adj_map[i-1].push_back(mother.get_microgene(i));
		}
		
		children.insert_gene(0, father.get_microgene(0));
		int current_microgene = 0;
		
		for (int i = 1; i < nodes; i++)
		{
			int min_microgene = -1;
			ll min_cust = INF;
			for (auto adj_microgene : adj_map[current_microgene])
			{
				if(!children.has_microgene(adj_microgene))
				{
					ll cust = matrix_adj[current_microgene][adj_microgene];
					cout << current_microgene <<" -> "<< adj_microgene <<" : " << cust<<endl;
					if(cust < min_cust)
					{
						min_cust = cust;
						min_microgene = adj_microgene;
					}
				}
			}
			if (min_microgene == -1)
			{
				continue;
			}
			children.insert_gene(i, min_microgene);
			current_microgene = min_microgene;
		}
		children.get_path();
		cout << endl;
		return;
	}
	
	static void arithmetic_average(T& children, T& father,T& mother, ull** matrix_adj, const params& params_active_ga)
	{//arithmetic_average
		
		int father_gene= params_active_ga.tx_parents/params_active_ga.alpha, mom_gene = params_active_ga.tx_parents, nodes = mother.get_nodes();;
		children.init(nodes);
		
		int rate = random_range(0, 100);
		if (params_active_ga.fix != -1)
		{
			rate = random_range(0, mom_gene);
		}

		if (rate <= father_gene)
		{
			children.insert_gene(0, father.get_microgene(0));
		}
		else
		{
			if (rate <= mom_gene)
			{
				children.insert_gene(0, mother.get_microgene(0));
			}
			else
			{
				children.insert_gene(0, random_range(0,nodes));
			}
		}
		
		for (int i = 1; i < nodes; i++) 
		{
			rate = random_range(0, 100);

			if (rate <= father_gene)
			{
				children.insert_gene(i, father.get_microgene(i));
			}
			else
			{
				if (rate <= mom_gene)
				{
					children.insert_gene(i, mother.get_microgene(i));
				}
				else
				{
					children.insert_gene(i, random_range(0,nodes));
				}
			}
		}

	}
};
