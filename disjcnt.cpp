#include <fstream>
#include <vector>
#include <unordered_map>
#include <iostream>
#include <stdio.h>

// combinari de <de> luate cate 2
unsigned long long combinari2(unsigned long long de)
{
	return 1ULL * de * (de - 1)/2;
}

void afisare(unsigned long long nr)
{
	
	std::ofstream output("disjcnt.out");
	output << nr;
	output.close();
}


unsigned int DFS(int node, std::vector<std::unordered_map<int,
 unsigned int> >& vect_adi, 
 std::vector<bool>& vizitate, std::vector<unsigned int>& indx,
  std::vector<unsigned int>& low, 
  unsigned int &crt_indx, unsigned long long &nr_perechi)
{
	vizitate[node] = true;
	indx[node] = crt_indx;
	low[node] = crt_indx;
	crt_indx++;
	unsigned int nr_componente = 0;

	for(auto it = vect_adi[node].begin(); it != vect_adi[node].end(); it++)
	{
		if(it->second >= 1)
		{
			it->second--;
			vect_adi[it->first][node]--;
			
			if(vizitate[it->first] == false)
			{
				nr_componente += DFS(it->first, vect_adi, vizitate, indx,
				 low, crt_indx, nr_perechi);
				if(low[node] > low[it->first])
				{
					low[node] = low[it->first];
				}
			}
			else
			{
				if(indx[it->first] < low[node])
				{
					low[node] = indx[it->first];
				}
			}
		}
	}

	nr_componente++;
	if(low[node] == indx[node])
	{
		//nod radacina in ciclu
		if(nr_componente != 1)
		{
			nr_perechi += combinari2(nr_componente);
		}
		nr_componente = 0;
	}
	return nr_componente;
}

std::vector<std::unordered_map<int, unsigned int> > citire(int &n, int &m)
{
	int node1, node2;
	std::ifstream input("disjcnt.in");
	input >> n >> m;
	std::vector<std::unordered_map<int, unsigned int> > vec_adi(n + 1);
	for(int i = 1; i <= m; i++)
	{
		input >> node1 >> node2;
		vec_adi[node1][node2]++;
		vec_adi[node2][node1]++;

	}

	input.close();
	return vec_adi;
}

int main()
{

	int n, m;
	std::vector<std::unordered_map<int, unsigned int> > vect_adi;
	vect_adi = citire(n, m);

	// bool este implementat separat pentru vector pentru eficienta.
	std::vector<bool> vizitate(n + 1, false);
	std::vector<unsigned int> indx(n + 1, 0);
	std::vector<unsigned int> low(n + 1, 0);
	unsigned long long nr_perechi = 0;
	unsigned int crt_indx = 1;

	// avand in vedere ca graful este connex nu este necesar sa fac un for
	DFS(1, vect_adi, vizitate, indx, low, crt_indx, nr_perechi);

	afisare(nr_perechi);
	return 0;
}