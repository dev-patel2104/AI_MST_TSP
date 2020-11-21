#include <iostream>
#include <cstdio>
#include <cstring>
#include <vector>
#include <algorithm>
#include <cmath>


using namespace std;

#define MAX_SIZE 20
#define INF 0x7FFFFFFF
#define SAFETY_RANGE 10



struct edge
{
	int vx, vy;
	double cost;
	edge(int u, int v, double c)
	{
		vx = u, vy = v, cost = c;
	}
};

bool isMST[MAX_SIZE][MAX_SIZE];
double graph[MAX_SIZE][MAX_SIZE];
vector<edge>Edges;	//Store all edges and corresponding edge weights
vector<edge> EdgeH; //Store all the edges and corresponding h(x) values considering goal and destination pair
int pathW[MAX_SIZE][MAX_SIZE];
pair<int, int> pairt[MAX_SIZE];
int minV = 999;
int cnt1 = 0;
int total_edges = 0;							// total numbers of edges in the TSP graph
vector<int>sol;									//Store Solution
bool	r[MAX_SIZE];							//r store
int		parent[MAX_SIZE];						//Parent store
double	px[MAX_SIZE], py[MAX_SIZE];	//point position - X axis and Y axis
int		total_number = 0, start;
bool	visited[MAX_SIZE];											//shows which nodes are visited

double minDistance = 0;

double	getDistance(int i, int j);		//Calculate distance of 2 place
void	userInput();					//Take input from user
bool	compareEdges(edge a, edge b);	//Copmare function for sorting
int		Find(int p);					//Find an element in the parents
void	Kruskal();						//Run Kruskal Algo
void	Astar(int u);						//Run Astar tree search Algo
void	printInput();					//Print all input taken
void	Union(int p, int q);				//Unite 2 points - change r and parent
void	findAllDistances();				//Find all distances of the graph
void	initializeValues();					//Initialize all the ranks and parents
double	findUsingAstar();				//Astar Algorithm to run
void	pairsort();

int main()
 {
	int x, y;
	double c;
	int temp = 0;
	cout << "Enter the total number of nodes" << endl;
	cin >> total_number;
	total_edges = ((total_number) * (total_number - 1)) / 2;
	cout << "Enter the path weights in order - in this " << total_edges <<" values" << endl;
	
	for (int i = 0; i < total_number - 1; i++)
	{
		for (int j = i + 1; j < total_number; j++)
		{
			cin >> temp;
			pathW[i][j] = temp * 20;
			pathW[j][i] = temp * 20;
		}
		visited[i] = 0;
	}
	visited[total_number - 1] = 0;

	cout << "now you are adding user inputs" << endl;
	userInput();

	printInput();

	cout << findUsingAstar() << endl;


	return 0;
}

void pairsort()
{

	// Storing the respective array 
	// elements in pairs. 
	for (int i = 0; i < total_edges; i++)
	{
		pairt[i].first = Edges[i].cost;
		pairt[i].second = EdgeH[i].cost;
	}

	// Sorting the pair array. 
	sort(pairt, pairt + total_edges);

	// sorting the actual edges vector
	sort(Edges.begin(), Edges.end(), compareEdges);

	// Modifying original arrays 
	for (int i = 0; i < total_edges; i++)
	{
		EdgeH[i].cost = pairt[i].second;
	}
}

void userInput()
{
	
	for (int i = 0; i < total_number; i++)
	{
		cout << "taking node " <<  i  << " as input" << endl;
		cin >> px[i] >> py[i];
	}
	cin >> start;
}


void initializeValues()
{
	for (int i = 0; i < total_number; i++)
	{
		r[i] = 1;
		parent[i] = i;
	}
}

void printInput()
{
	cout << "TOtal Input = ";
	cout << total_number << endl << endl << "Points are - " << endl;
	for (int j = 0; j < total_number; j++)
	{
		cout << px[j] << " " << py[j] << endl;
	}
	cout << "------------------------" << endl;
}

void findAllDistances()
{
	int cnt = 0;
	for (int i = 0; i < total_number - 1; i++)
	{
		for (int j = i + 1; j < total_number; j++)
		{
			graph[i][j] = graph[j][i] = getDistance(i, j);
			Edges.push_back(edge(i, j, pathW[i][j]));
			EdgeH.push_back(edge(i, j, graph[i][j]));
			/*Edges.push_back(edge(i, j, graph[i][j]));
			EdgeH.push_back(edge(i, j, pathW[i][j]));*/
			cnt++;
		}
	}
}

double getDistance(int i, int j)
{
	return 2*(sqrt((px[i] - px[j]) * (px[i] - px[j]) + (py[i] - py[j]) * (py[i] - py[j])));
}

bool compareEdges(edge a, edge b)
{
	return (a.cost < b.cost);
}

int Find(int p)
{
	if (parent[p] == p)
		return p;
	return Find(parent[p]);
}

void Union(int p, int q)
{
	if (p == q)
		return;
	if (r[p] > r[q])
		swap(p, q);

	r[q] += r[p];
	parent[p] = q;
}

void Kruskal()
{
	cout << "MST includes following edges" << endl;
	int cnt = 0;
	for (int i = 0; i < Edges.size(); i++)
	{
		if (cnt == total_number - 1)
			break;
		int p = Find(Edges[i].vx);
		int q = Find(Edges[i].vy);
		if (p != q)
		{
			cnt++;
			Union(p, q);
			isMST[Edges[i].vx][Edges[i].vy] = isMST[Edges[i].vy][Edges[i].vx] = 1;
			cout << Edges[i].vx << "-----" << Edges[i].vy << endl; 
		}
	}
}

void Astar(int u)
{
	int temp = 0;
	int ent_cnt = 99;
	minV = 999;
	std::vector<int>::iterator it;
	it = std::find(sol.begin(), sol.end(), u);
	if (it != sol.end() && !sol.empty()) // this if else statement confirms that the same element is not added twice
	{
		
	}
	else
	{
		sol.push_back(u);
	}
	
	visited[u] = 1;
	while (ent_cnt != 0)
	{
		ent_cnt = 0;
		for (int i = 0; i < total_number; i++)
		{
			if (isMST[u][i] && !visited[i])
			{
				ent_cnt++;
				cnt1 = cnt1 + (graph[sol.back()][0] + pathW[sol.back()][i]);
				if (cnt1 < minV)
				{
					minV = cnt1;
					temp = i;
				}
				cnt1 = cnt1 - (graph[sol.back()][0] + pathW[sol.back()][i]);
			}
		}
 
		if (!visited[temp] && ent_cnt != 0)
		{
			cnt1 = cnt1 +pathW[sol.back()][temp];
			Astar(temp);
		}

	}
}

double findUsingAstar()
{
	findAllDistances();	
	pairsort();										// sort the edges as well as the h(x) and g(x) pair
	initializeValues();								//Initialize all the values - Parent and
	Kruskal();										// create a MST using kruskal algo
	Astar(start);									// traverse using the concept of Astar
		
	//sol vector has the nodes traveled according to the A star algorithm 
	cout << "The path is :" << endl;
	for (int i = 0; i < sol.size() - 1; i++)		
	{
		minDistance += (pathW[sol[i]][sol[i + 1]]);  // we add the distance between two consecutive nodes 
		cout << sol[i] << "--->" << sol[i + 1] << endl;
		cout << "Total cost upto this point upto this point" << endl;
		cout << minDistance << endl;
	}
	cout << sol.back() << "--->" << start << endl;
	cout << "Total cost upto this point upto this point" << endl;
	minDistance = minDistance +  pathW[sol.back()][start];
	cout << minDistance << endl;
	cout << "The final total cost is" << endl;
	return minDistance; 
}



