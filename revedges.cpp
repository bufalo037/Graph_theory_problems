#include <fstream>
#include <vector>
#include <utility>

#define MAXINT 999999999

struct Input
{
	std::vector<std::vector<unsigned int> >* costs;

	Input(std::vector<std::vector<unsigned int> >* costs)
	{
		this->costs = costs;
	}

	virtual ~Input()
	{
		delete this->costs;
	}
};


struct min
{
	bool operator()(const std::pair<int, int> &x, std::pair<int, int> &y) const
	{
		return x.second >= y.second;
	}
};


void Floyd_Warshall(Input *date, int n)
{
	unsigned int aux;
	for(int k = 1 ; k <= n; k++)
	{
		for(int i = 1; i <= n; i++)
		{
			for(int j = 1 ; j <= n; j++)
			{
				aux = (*date->costs)[i][k] + (*date->costs)[k][j];
				if((*date->costs)[i][j] > aux)
				{
					(*date->costs)[i][j] = aux;
				}
			}
		}
	}
}


void solve(int&n, int&m, int& q)
{
	std::ifstream input("revedges.in");
	std::ofstream output("revedges.out");
	input >> n >> m >> q;

	// -1 inseamna ca nu exista muchie
	std::vector<std::vector<unsigned int> > *costs;
	costs = new std::vector<std::vector<unsigned int> >
	(n + 1, std::vector<unsigned int>(n + 1, MAXINT));

	int node1, node2;

	for(int i = 0; i < m; i++)
	{	
		input >> node1 >> node2;
		// costul inainte este 0
		(*costs)[node1][node2] = 0;
		// costul inapoi este de 1 element pe muchie inversa
		if((*costs)[node2][node1] != 0)
			(*costs)[node2][node1] = 1;

	}

	for(int i = 1; i <= n; i++)
	{
		(*costs)[i][i] = 0;
	}

	Input *date = new Input(costs); 
	Floyd_Warshall(date, n);

	for(int i = 0; i < q; i++)
	{
		input >> node1 >> node2;
		output << (*date->costs)[node1][node2] << " ";
	}
	delete date;
	input.close();
	output.close();
}


int main()
{
	int n, m, q;
	solve(n, m, q);
	return 0;
}