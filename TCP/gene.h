#pragma once
#include "bibli.h"

struct gene
{
	int n;
	ill happy = inf, cust=0;
	vector<int> path;
	set<int> st;
	gene()
	{
		st.clear();
		path.clear();
	}
	
	bool add(int x, int p = 0, bool fim=0)
	{
		if(fim || st.count(x)==0)
		{
			cust+=p;
			st.insert(x);
			path.push_back(x);
			return true;
		}
		return false;
	}
	void add_mut(int x, int p=0)
	{
		if(p == inf) cust = inf;
		else
			if(cust!=inf) cust+=p;
		path.push_back(x);
		st.insert(x);
	}
};
