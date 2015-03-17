#pragma once
#include "vision.h"

/*
 * Dijkstra Algorithm
 * by Lynx
 */

class Dijkstra :
	public Vision
{
public:
	Dijkstra(void);
	~Dijkstra(void);

	void init();
	void doAlgorithm();

	void onPaint(HDC hdc);
	void onCommand(int type);

	int S[5];
	bool U[5];
	int pre[5];
	int dist[5];
	int path[5][5];
	int SLen;
};

