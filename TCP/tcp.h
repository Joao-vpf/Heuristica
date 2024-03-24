#pragma once
#include "bibli.h"
#include "gene.h"
#include "mutation.h"

class tcp{
	ill max;
	int n;
	vector<pair<int, ill>>* adj;
	vector<gene*> states;
	
	void fill()
	{//preenche com o caso de teste
		adj = new vector<pair<int, ill>>[n+1];
		int x, y,p;
		while(cin>> x >> y >> p)
		{
			adj[x].push_back({y,p});
			adj[y].push_back({x,p});
		}
	}
	
	void first()
	{ // Acha os caminhos base
		for(int i =0; i<2; i++)
		{
			int x = rand()%n+1,y, ini;
			//x=1; //comecar com 1
			ini = x;
			gene* a;
			states.push_back(new gene);
			a = states.back();
			a->add(x);
			int cont = n-1;
			while(cont--)
			{
				
				do
				{
					y = rand()%n+1;
				//	cout << a->st.count(y) << " " << y<<endl;
				}while(a->st.count(y));
				for(auto e : adj[x])
				{
					if(e.first == y)
					{
						a->add(y, e.second);
						break;
					}
				}
				x = y;
			}
			
			for(auto e : adj[x])
			{
				if(e.first == ini)
				{
					a->add(ini, e.second, 1);
				}
			}
			calc_happy(a, max, states.size(), n);
			if(a->happy != inf){
				max = min(max, a->cust);
			}
		}
	}
	
	void print()
	{
		gene* res;
		for(auto e:states)
		{
			if(e->cust == max and e->happy != inf)
			{
				res = e;
			}
			
			cout << "Custo: "<< e->cust << " Caminho: ";
			for(auto u : e->path)
			{
				cout << u << "-->" ;
			}
			cout << endl;
		}

		cout << endl << endl;
		cout << "Resposta: " <<endl;
		cout << "Custo: "<< res->cust << " Caminho: ";
		for(auto u : res->path)
		{
			cout << u << "-->" ;
		}
		cout << endl;
	}
	

	void simulation()
	{//Simula as mutaçoes
		int rep = 20000;
		while(rep--)
		{
			int m = states.size();
			srand(time(0));
			vector<gene*> baby;
			for(int i=0; i<m; i++)
			{//Cria os novos genes
				int r = -1;
				do
				{
					r =rand()%m;
				}
				while(r==i);
				baby.push_back(cross(states[i], states[r], adj, n));
			}
			for(auto e : baby)
			{//Faz os calculos para os novos genes
				calc_happy(e, max, states.size(), n);
				if(e->happy != inf)
				{
					if(e->cust<=max)
					{
						max = min(max, e->cust);
					}
				}
				states.push_back(e);
			}
			dead(states);//Mata alguns genes
			if(rep%100 == 0)
			gemeos(states);//Tira genes repetidos dos melhores locais
			embaralha(states);//Shuffle
			if(rep%1000==0)
			cout << rep <<" " << states.size() << " " << max <<endl;
		}
	}

	public:
	tcp()
	{
		max = inf;
		srand(time(0));
	}

	~tcp()
	{
		delete[] adj;
		for(auto e: states)
			delete e;
	}
	
	ill solve()
	{//Main do TCP
		cin>> n;
		fill();
		first();
		simulation();
		print();
		return max;
	}
};
