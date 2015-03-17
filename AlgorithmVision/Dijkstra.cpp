#include "stdafx.h"
#include "Dijkstra.h"

Dijkstra::Dijkstra(void)
{
	init();
}


Dijkstra::~Dijkstra(void)
{
}

void Dijkstra::init()
{
	memset(S, 0, sizeof(S));
	memset(U, true, sizeof(U));
	memset(pre, -1, sizeof(pre));

	memset(dist, 999999, sizeof(dist));
	memset(path, -1, sizeof(path));

	path[0][1] = 10;
	path[0][3] = 30;
	path[0][4] = 100;

	path[1][2] = 50;
	path[2][4] = 10;
	path[3][2] = 20;
}

void Dijkstra::doAlgorithm()
{
	int u = 0;

	SLen = 0;
	dist[0] = 0;

	while (SLen < 5)
	{
		S[SLen]  = u;
		U[u] = false;

		for (int i=0; i<5; i++)
		{
			if (path[u][i] != -1)
			{
				if (pre[i] == -1 || dist[u] + path[u][i] < dist[i])
				{
					dist[i] =  dist[u] + path[u][i];
					pre[i] = u;
				}
			}
		}

		u = -1;
		for (int i=0; i<5; i++)
		{
			if (U[i])
			{
				if (u == -1 || dist[i] < dist[u])
				{
					u = i;
				}
			}
		}
		SLen++;
	}
}

void Dijkstra::onPaint( HDC hdc )
{
	doAlgorithm();
}

void Dijkstra::onCommand( int type )
{

}
