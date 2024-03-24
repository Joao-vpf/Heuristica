#include "bibli.h"
#include "tcp.h"

int main()
{
	cout.tie(0);
	ios_base::sync_with_stdio(false);
	freopen(input,"r", stdin);
	tcp teste;
	cout << teste.solve() << endl;
}
