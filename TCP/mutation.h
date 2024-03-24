#pragma once
#include "bibli.h"
#include "gene.h"


ill peso(vector<pair<int, ill>>* adj,ill x, ill y)
{
    for(auto e : adj[x])
    {
        if(e.first == y)
        {
            return e.second;
        }
    }
    return inf;
}


gene* cross(gene* a, gene* b,vector<pair<int, ill>>* adj, int m)
{
    int n =  a->path.size();
    srand(time(0));
    gene* nv = new gene;
    ill ini;
    if(rand()%2 == 0)
    {
        if(rand()%2 == 0)
            ini = a->path[0];
        else
            ini = b->path[0];
    }
    else
    {
        do
        {
            ini = rand()%m+1;
        }
        while(b->path[0]==ini or a->path[0]==ini);
    }

    nv->add_mut(ini);

    for(int i=1; i<n-1; i++)
    {
        if(rand()%2 == 0)
        {
            if(rand()%2==0)
                nv->add_mut(a->path[i], peso(adj,nv->path.back(), a->path[i]));
            else
                nv->add_mut(b->path[i], peso(adj,nv->path.back(), b->path[i]));
        }
        else
        {
            int r = rand()%m+1;
            while(b->path[i]==r or a->path[i]==r)
            {
                r = rand()%m+1;
            }
            nv->add_mut(r, peso(adj,nv->path.back(), r));
        }
    }
    nv->add_mut(ini, peso(adj,nv->path.back(), ini));
    
    return nv;
}


void calc_happy(gene* a, ill max, int cont, int n)
{//gene, maximo atual, e qual state ele é
    int m = a->st.size();
    if(n==m)
    {
        if(a->cust>max)
        {
            a->happy=a->cust+max+cont;
        }
        else
        {
            a->happy=a->cust + (a->cust==max?cont : 0);
        }
    }
}


bool ordenareverso(gene* a, gene* b)
{
    if(a->happy == b->happy) return a->cust < b->cust;
    return a->happy < b->happy;
}

bool ordena(gene* a, gene* b)
{
    if(a->cust == b->cust) return a->happy > b->happy; 
    return a->cust > b->cust;
}

void dead(vector<gene*>& states)
{//mata alguns casos
    if(states.size()<6000)return;  // População maxima
    srand(time(0));
    sort(states.begin(), states.end(), ordenareverso);
    int sz = states.size();
    int n = (rand()%(sz/2) + sz/3 + sqrt(sz));
    while(n--)
    {
        gene* a = states.back();
        states.pop_back();
        delete a;
    }
    
    if(states.size() > 8000) n = states.size()-8000;
    else return;
    
    while(n--)
    {
        gene* a = states.back();
        states.pop_back();
        delete a;
    }
}

void gemeos(vector<gene*>& states)
{//mata alguns casos repetidos
    int sz = states.size();
    sort(states.begin(), states.end(), ordena);
    set<int> foi;
    priority_queue<int> q;
    for(int i=sz-1; i>(sz*2)/3; i--)
    {
        for(int j=i-1; j>(sz*2)/3; j--)
        {
            if(states[i]->path == states[j]->path)
            {
                if(states[i]->happy > states[j]->happy) 
                {
                    if(!foi.count(i))
                    {
                        foi.insert(i);
                        q.push(i);
                    }
                }
                else 
                {
                    if(!foi.count(j))
                    {
                        foi.insert(j);
                        q.push(j);
                    }
                }
            }
        }
    }

    while(q.size())
    {
        int u = q.top();
        q.pop();
        delete states[u];
        states.erase(states.begin()+u);
    }
}

void embaralha(vector<gene*>& states)
{
    srand(time(0));
    for (int i = states.size()-1; i>0;  i--) 
    {
        int j = rand() % (i + 1);
        swap(states[i], states[j]);
    }
}