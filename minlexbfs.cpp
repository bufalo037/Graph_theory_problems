#include <fstream>
#include <queue>
#include <vector>
#include <sstream>

struct min
{
	bool operator()(const int& x, const int& y) const
	{
		return x >= y;
	}
};

void get_output(std::ostringstream& out)
{
	std::ofstream output("minlexbfs.out");
	output << out.str();
	output.close();
}


void BFS(std::vector<std::priority_queue<int, std::vector<int>, min> >& liste_add,
	int n)
{
	std::ostringstream output;
	std::vector<int> vizitate(n + 1, 0);
	std::queue<int> queue_bfs;
	queue_bfs.push(1);
	vizitate[1] = 1;
	output << 1 ;
	int node_crt;
	int nod_adiacent;

	// cerinta spune ca e graf conex deci nu trebuie sa imi fac griji ca ar 
	// trbui sa fac mai multe parcurgeri pana acopar tot graful.
	while(!queue_bfs.empty())
	{
		node_crt = queue_bfs.front();
		queue_bfs.pop();
		while(!liste_add[node_crt].empty())
		{
			nod_adiacent = liste_add[node_crt].top();
			liste_add[node_crt].pop();
			if(vizitate[nod_adiacent] == 0)
			{
				vizitate[nod_adiacent] = 1;
				queue_bfs.push(nod_adiacent);
				output << " " << nod_adiacent;
			}
		}
	}
	get_output(output);
}


std::vector<std::priority_queue<int, std::vector<int>, min> > read_input(int& n, int& m)
{
	std::vector<std::priority_queue<int, std::vector<int>, min> > liste_add;
	//std::priority_queue< <int , std::vector<int>, min> aux; 
	std::ifstream input("minlexbfs.in");
	input >> n >> m;
	int node1, node2;
	liste_add.reserve(n + 1);
	for(int i = 1; i <= m; i++)
	{
		input >> node1 >> node2;
		liste_add[node1].push(node2);
		liste_add[node2].push(node1);
	}
	input.close();
	return liste_add;
}

int main()
{
	int n, m;
	std::vector<std::priority_queue<int, std::vector<int>, min> > liste_add =
	 read_input(n, m);
	BFS(liste_add, n);
	return 0;
}