#include <fstream>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <queue>

struct Date
{
	std::vector<unsigned int> *costs;
	std::unordered_map<int, std::unordered_set<int> > *blocate;

	Date(std::vector<unsigned int> *costs, std::unordered_map<int,
	 std::unordered_set<int> > *blocate)
	{
		this->costs = costs;
		this->blocate = blocate;
	}

	virtual ~Date()
	{
		delete this->costs;
		delete this->blocate;
	}
};


struct configuratie
{
	int x, y;
	unsigned int cost;
	short stand_face;
	short right_face;
	short up_face;
	bool vizitat;

	static int get_code(int x, int y)
	{
		// custom map
		// eficienta crescuta. Un overhead doar pentru if-uri
		// consider ca este o varianta acceptabila din cauza ca este constanta
		// implementarea mereu
		switch(x)
		{
			case 1:
				switch(y)
				{
					case 2:
						return 0;
					case 3:
						return 1;
					case 4:
						return 2;
					case 5:
						return 3;
				}
			case 2:
				switch(y)
				{
					case 1:
						return 4;
					case 3:
						return 5;
					case 4:
						return 6;
					case 6:
						return 7;
				}
			case 3:
				switch(y)
				{
					case 1:
						return 8;
					case 2:
						return 9;
					case 5:
						return 10;
					case 6:
						return 11;
				}
			case 4:
				switch(y)
				{
					case 1:
						return 12;
					case 2:
						return 13;
					case 5:
						return 14;
					case 6:
						return 15;
				}
			case 5:
				switch(y)
				{
					case 1:
						return 16;
					case 3:
						return 17;
					case 4:
						return 18;
					case 6:
						return 19;
				}
			case 6:
				switch(y)
				{
					case 2:
						return 20;
					case 3:
						return 21;
					case 4:
						return 22;
					case 5:
						return 23;
				}
			default:
			{
				perror("Ai dat indici necorespunzatori\n");
				return -1;
			}
		}
	}

	configuratie()
	{
		this->x = -1;
		this->vizitat = false;
	}

	configuratie(int x, int y, unsigned int cost, char stand_face,
	 char right_face, char up_face)
	{
		this->x = x;
		this->y = y;
		this->cost = cost;
		this->stand_face = stand_face;
		this->right_face = right_face;
		this->up_face = up_face;
		this->vizitat = false;
	}

	configuratie *get_right(std::vector<unsigned int> *costs,
		std::unordered_map<int, std::unordered_set<int> > *blocate)
	{
		if(blocate->find(x) != blocate->end())
		{
			if(blocate->find(x)->second.find(y + 1) == blocate->find(x)->second.end())
				return new configuratie(x, y + 1, cost + (*costs)[right_face],
			 right_face, 7 - stand_face, up_face);
		}
		else	
		{
			return new configuratie(x, y + 1, cost + (*costs)[right_face],
			 right_face, 7 - stand_face, up_face);
		}
		return NULL;
	}

	configuratie *get_left(std::vector<unsigned int> *costs,
		std::unordered_map<int, std::unordered_set<int> > *blocate)
	{
		if(blocate->find(x) != blocate->end())
		{
			if(blocate->find(x)->second.find(y - 1) == blocate->find(x)->second.end())
				return new configuratie(x, y - 1, cost + (*costs)[7 - right_face],
			 		7 - right_face, stand_face, up_face);
		}
		else
		{
			return new configuratie(x, y - 1, cost + (*costs)[7 - right_face],
			 		7 - right_face, stand_face, up_face);
		}	
		return NULL;
	}

	configuratie *get_up(std::vector<unsigned int> *costs,
		std::unordered_map<int, std::unordered_set<int> > *blocate)
	{
		if(blocate->find(x + 1) != blocate->end())
		{
			if(blocate->find(x + 1)->second.find(y) == blocate->find(x + 1)->second.end())
				return new configuratie(x + 1, y, cost + (*costs)[up_face],
			 		up_face, right_face, 7 - stand_face);
		}
		else
		{
			return new configuratie(x + 1, y, cost + (*costs)[up_face],
				up_face, right_face, 7 - stand_face);
		}
		return NULL;
	}

	configuratie *get_down(std::vector<unsigned int> *costs,
		std::unordered_map<int, std::unordered_set<int> > *blocate)
	{
		if(blocate->find(x - 1) != blocate->end())
		{
			if(blocate->find(x - 1)->second.find(y) == blocate->find(x - 1)->second.end())
				return new configuratie(x - 1, y, cost + (*costs)[7 - up_face],
			 		7 - up_face, right_face, stand_face);
		}
		else
		{
			return new configuratie(x - 1, y, cost + (*costs)[7 - up_face],
			 	7 - up_face, right_face, stand_face);
		}
		
		return NULL;
	}

	// debugging purposes
	friend std::ostream& operator<<(std::ostream & x, configuratie & y)
	{
		return x << "x = " << y.x << ", y = " << y.y << ", cost = " << y.cost <<
		 ", stand_face = " << y.stand_face << ", right_face = " << y.right_face
		   << ", up_face = " << y.up_face << ", vizitat = " << y.vizitat;  
	}
	

	~configuratie() {}
};


struct minn
{
	bool operator()(configuratie const &x, configuratie const &y)
	{
		return x.cost >= y.cost;
	}
};


struct cell
{
	std::vector<configuratie> configuratii;

	cell()
	{
		// hopefully decent
		this->configuratii = std::vector<configuratie>(24, configuratie());
	}

	~cell(){}
};


int dijkstra(int n, int m ,int Sx, int Sy, int Fx, int Fy, Date *date)
{
	std::priority_queue<configuratie, std::vector<configuratie>, minn> pq;
	std::vector<std::vector<cell> > matrix(n + 1, std::vector<cell>(m + 1, cell()));
	configuratie aux, *new_conf;

	matrix[Sx][Sy].configuratii[configuratie::get_code(1, 3)] =
	 configuratie(Sx, Sy, (*date->costs)[1], 1, 3, 5);

	pq.push(matrix[Sx][Sy].configuratii[configuratie::get_code(1, 3)]);

	while(!pq.empty())
	{
		aux = pq.top();
		pq.pop();

		if(matrix[aux.x][aux.y].configuratii[configuratie::get_code
			(aux.stand_face, aux.right_face)].vizitat == false)
		{
			matrix[aux.x][aux.y].configuratii[configuratie::get_code
				(aux.stand_face, aux.right_face)].vizitat = true;

			if(aux.x == Fx && aux.y == Fy)
				return aux.cost;

			if(aux.y + 1 <= m)
			{
				new_conf = aux.get_right(date->costs, date->blocate);
				if(new_conf)
				{
					if(matrix[new_conf->x][new_conf->y].configuratii[configuratie::
						get_code(new_conf->stand_face, new_conf->right_face)].x == -1)
					{
						matrix[new_conf->x][new_conf->y].configuratii[configuratie::
						get_code(new_conf->stand_face, new_conf->right_face)] = *new_conf; 
						pq.push(*new_conf);
					}
					else
					{
						if(matrix[new_conf->x][new_conf->y].configuratii[configuratie::
							get_code(new_conf->stand_face, new_conf->right_face)].cost >
							 new_conf->cost)
						{
							matrix[new_conf->x][new_conf->y].configuratii[configuratie::
							get_code(new_conf->stand_face, new_conf->right_face)].cost =
							 new_conf->cost;
						}
						if(matrix[new_conf->x][new_conf->y].configuratii[configuratie::
							get_code(new_conf->stand_face, new_conf->right_face)].vizitat
							 == false)
						{
							pq.push(matrix[new_conf->x][new_conf->y].configuratii[configuratie::
							get_code(new_conf->stand_face, new_conf->right_face)]);
						}
					}
				}
				delete new_conf;
			}

			if(aux.y - 1 >= 1)
			{
				new_conf = aux.get_left(date->costs, date->blocate);
				if(new_conf)
				{
					
					if(matrix[new_conf->x][new_conf->y].configuratii[configuratie::
						get_code(new_conf->stand_face, new_conf->right_face)].x == -1)
					{
						matrix[new_conf->x][new_conf->y].configuratii[configuratie::
						get_code(new_conf->stand_face, new_conf->right_face)] = *new_conf; 
						pq.push(*new_conf);
					}
					else
					{
						if(matrix[new_conf->x][new_conf->y].configuratii[configuratie::
							get_code(new_conf->stand_face, new_conf->right_face)].cost >
							 new_conf->cost)
						{
							matrix[new_conf->x][new_conf->y].configuratii[configuratie::
							get_code(new_conf->stand_face, new_conf->right_face)].cost =
							 new_conf->cost;
						}
						if(matrix[new_conf->x][new_conf->y].configuratii[configuratie::
							get_code(new_conf->stand_face, new_conf->right_face)].vizitat
							 == false)
						{
							pq.push(matrix[new_conf->x][new_conf->y].configuratii[configuratie::
							get_code(new_conf->stand_face, new_conf->right_face)]);
						}
					}
				}
				delete new_conf;
			}

			if(aux.x + 1 <= n)
			{
				new_conf = aux.get_up(date->costs, date->blocate);
				if(new_conf)
				{

					if(matrix[new_conf->x][new_conf->y].configuratii[configuratie::
						get_code(new_conf->stand_face, new_conf->right_face)].x == -1)
					{
						matrix[new_conf->x][new_conf->y].configuratii[configuratie::
						get_code(new_conf->stand_face, new_conf->right_face)] = *new_conf; 
						pq.push(*new_conf);
					}
					else
					{
						if(matrix[new_conf->x][new_conf->y].configuratii[configuratie::
							get_code(new_conf->stand_face, new_conf->right_face)].cost >
							 new_conf->cost)
						{
							matrix[new_conf->x][new_conf->y].configuratii[configuratie::
							get_code(new_conf->stand_face, new_conf->right_face)].cost =
							 new_conf->cost;
						}
						if(matrix[new_conf->x][new_conf->y].configuratii[configuratie::
							get_code(new_conf->stand_face, new_conf->right_face)].vizitat
							 == false)
						{
							pq.push(matrix[new_conf->x][new_conf->y].configuratii[configuratie::
							get_code(new_conf->stand_face, new_conf->right_face)]);
						}
					}
				}
				delete new_conf;
			}

			if(aux.x - 1 >= 1)
			{
				new_conf = aux.get_down(date->costs, date->blocate);
				if(new_conf)
				{
					
					if(matrix[new_conf->x][new_conf->y].configuratii[configuratie::
						get_code(new_conf->stand_face, new_conf->right_face)].x == -1)
					{
						matrix[new_conf->x][new_conf->y].configuratii[configuratie::
						get_code(new_conf->stand_face, new_conf->right_face)] = *new_conf; 
						pq.push(*new_conf);
					}
					else
					{
						if(matrix[new_conf->x][new_conf->y].configuratii[configuratie::
							get_code(new_conf->stand_face, new_conf->right_face)].cost >
							 new_conf->cost)
						{
							matrix[new_conf->x][new_conf->y].configuratii[configuratie::
							get_code(new_conf->stand_face, new_conf->right_face)].cost =
							 new_conf->cost;
						}
						if(matrix[new_conf->x][new_conf->y].configuratii[configuratie::
							get_code(new_conf->stand_face, new_conf->right_face)].vizitat
							 == false)
						{
							pq.push(matrix[new_conf->x][new_conf->y].configuratii[configuratie::
							get_code(new_conf->stand_face, new_conf->right_face)]);
						}
					}
				}
				delete new_conf;
			}
		}
	}
	return -1;
}


Date *citire(int &n, int &m, int &Sx, int &Sy, int &Fx, int &Fy, int &k)
{
	std::ifstream input("rtd.in");
	int cel1, cel2;
	input >> n >> m >> Sx >> Sy >> Fx >> Fy >> k;

	// Am scris-o desfasurat ca e un pic de tot mai rapid ca nu face nici un jump
	// Si avand in vedere ca inputul era constatn dc nu as face-o. Zarul nu are
	// un numar variabil de fete.
	std::vector<unsigned int> *costs = new std::vector<unsigned int>(7);
	input >> (*costs)[1] >> (*costs)[2] >> (*costs)[3] >> (*costs)[4] >> 
	(*costs)[5] >> (*costs)[6];

	std::unordered_map<int, std::unordered_set<int> > *blocate = new
	std::unordered_map<int, std::unordered_set<int> >();

	for(int i = 0; i < k ; i++)
	{
		input >> cel1 >> cel2;
		(*blocate)[cel1].insert(cel2);
	}
	Date *date = new Date(costs, blocate);
	input.close();

	return date;
}

int main()
{
	int n, m ,Sx, Sy, Fx, Fy, k;
	Date *date = citire(n, m ,Sx, Sy ,Fx, Fy, k);
	std::ofstream output("rtd.out");
	output << dijkstra(n, m, Sx, Sy, Fx, Fy, date);
	delete date;
	output.close();
	return 0;
}