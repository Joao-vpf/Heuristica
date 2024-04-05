#include "library.hpp"
#include "TSP.hpp"

int main()
{
	//ios
	ifstream adj(input);
	ull** mat;
	mat = 0;
	int n;
	adj >> n;

	for (int i = 0; i < n; i++) mat = new ull * [n+1];

	for (int i = 0; i < n; i++) mat[i] = new ull[n + 1];

	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			mat[i][j] = INF;

	int x, y;
	x = y = 0;
	ll p;
	while(adj >> x >> y >> p)
	{
		mat[y][x] = mat[x][y] = p;
	}

	TSP a(mat, n);
	a.genetic_algorithm(parameters);
}
