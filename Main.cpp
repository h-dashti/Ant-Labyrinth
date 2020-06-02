// In this program we consider ant walks in a perculation cluster that generated with probabilty p.
// Ant start at random on one of th ocuipped sites of perculation cluaster.
// In ecah step(time) ant have 4 possibile motion , ant chose one way randomly, if that site is ocuppied then 
// ant moves to that site, otherwise, ant remains in its present position. Either way th time t increased by one.
// We Calculate <R^2> versus t, (R is the square of the distance between ants position at t=0, and its position in time t)
// We consider p > pc, and p < pc and  p = pc,  then investigate the dependence of <R^2> on p and t.



#include <iostream>
#include <ctime>
#include <fstream>
#include <cmath>
#include <cstdlib>

using namespace std;


// Global Variables
const int L = 110;
const int maxLength = L*L;
int xSeed[maxLength], ySeed[maxLength];
//int mass[maxLength/2 + 1];

int perX[3*maxLength], perY[3*maxLength];
int nX[] = {1, 0, -1, 0};  // Set Up direction vectors for latice
int nY[] = {0, 1, 0, -1};

char site[L+2][L+2];  // we consider L+2 for avoide checking boundary. our real array indes is 1:L
int n_ocupied;			// number of sites in cluster
double p = 1;		// propabilty of occuption for each site

//-------------------
int t; // time (step)
const int tMax = 50, iterations = 1000000 ; // number of simulation for counterin R^2(t)
double R2[tMax+1];
int x_ant, y_ant; // the present position of ant
//


// protoype Methods
inline int Rand( int nMin, int nMax );
inline double Rand();

void InitArrays();
void GenPerculatCluster();
bool ChechVerticalCluter();
void AntSimulation();


//

int main()
{
	srand(unsigned(time(NULL)));


	cout << "0%";
	for(int i = 1; i <= iterations; i++)
	{	
		do
		{
			InitArrays();
			GenPerculatCluster();
		}while(!ChechVerticalCluter());


		// afther generation perculation cluster, we simulate ant motions.
		AntSimulation();

		
	}

	ofstream outR2("R2.txt");
	for( t = t/2; t <= tMax; t++)
		if(t%1 == 0)
			outR2 << log((double)t) << "\t" << log(R2[t]) << endl;
	outR2.close();


	return 0;
}



void AntSimulation()
{
	int i0, x, x0, y, y0, dx, dy;
	x_ant = x0 = xSeed[0];
	y_ant = y0 = ySeed[0];

	for( t = 1; t <= tMax; t++)
	{
		i0 = rand()%4;
		x = x_ant + nX[i0];
		y = y_ant + nY[i0];
		if( x >= 1 && x <= L && y >= 1 && y <= L )  // because we set  boundry  1:L  
		{
			if(site[x][y] == 1)  // if site is ocuppied
			{
				x_ant = x;
				y_ant = y;
			}
		}
		dx = x_ant - x0;
		dy = y_ant - y0;
		R2[t] += ( dx * dx + dy * dy) / (double)iterations;	
	}

}


void InitArrays()
{
	int i, j;
	
	for( i = 1; i < L+1; i++)
		for(j = 1; j < L+1; j++)
			site[i][j] = 0;

	for( i = 0 ; i < L + 2; i++)  // set boundary -1
	{
		site[0][i] = -1;
		site[L+1][i] = -1;
		site[i][0] = -1;
		site[i][L+1] = -1;
	}
		
}


void GenPerculatCluster()
{
	int  i, n_per, i_per;

	int x_seed = L/2 + 1;			// seed at center of latice
	int y_seed = x_seed;

	site[x_seed][y_seed] = 1;
	n_ocupied = 1;
	xSeed[0] = x_seed;
	ySeed[0] = y_seed;

	for(i = 0; i < 4; i++)
	{
		perX[i] = x_seed + nX[i];
		perY[i] = y_seed + nY[i];
		site[perX[i]][perY[i]] = 2; 
	}

	n_per = 4;					// Initial number of perimeter list.
	int x, y, xNew, yNew;
	do
	{
		i_per = rand()%n_per;	// Index for random choosen perimeter site.
		x = perX[i_per];
		y = perY[i_per];

		// relabel remaining perimeter sites so that last perimeter site in array replace newly choosen site.
		perX[i_per] = perX[n_per - 1];
		perY[i_per] = perY[n_per - 1];

		n_per--;
		if(Rand() < p )
		{
			site[x][y] = 1;
			n_ocupied++;
			xSeed[n_ocupied-1] = x;
			ySeed[n_ocupied-1] = y;
			
			for(i = 0; i < 4; i++)
			{
				xNew = x + nX[i];
				yNew = y + nY[i];
				if(site[xNew][yNew] == 0)
				{
					perX[n_per] = xNew;
					perY[n_per] = yNew;
					n_per++;
					// place site in perimeter list
					site[xNew][yNew] = 2;
				}
			}
		}
		else
			site[x][y] = -1;	

	}while(n_per >= 1);

}




bool ChechVerticalCluter()
{
	int i;
	bool check = false;
	for(i = 1; i < L+1; i++)
	{
		if(site[i][1] == 1)
		{
			check = true;
			break;
		}
	}
	if(!check)
		return false;
	
	check = false;
	for(i = 1; i < L+1; i++)
	{
		if(site[i][L] == 1)
		{
			check = true;
			break;
		}
	}
	if(!check)
		return false;

	return true;

}



inline int Rand( int nMin, int nMax )
{
    int nRange = nMax - nMin;
    int nNum = rand() % nRange;
    return( nNum + nMin );
}
inline double Rand()
{
	return (double)rand() / RAND_MAX;
}
