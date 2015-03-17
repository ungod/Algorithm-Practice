#pragma once
#include "vision.h"
#include <list>
#include <map>
#include <string>

/*
 * AStar Algorithm
 * by Lynx
 */

class AStar : public Vision
{
private:
	struct Tile
	{
		int row, col;
		int lrow, lcol;
		unsigned int F,G,H;
		Tile():row(0), col(0), F(0), G(0), H(0){}
	};
	typedef std::map<std::string, Tile> TileList;
public:
	AStar(void);
	~AStar(void);

	void init();
	void doAlgorithm();

	void calcLeastFourTile();
	void calcLeastFInOpen();
	void calcFGH(Tile& tile);
	bool canCreateTile(int row, int col);
	void addToOpenList(const Tile& tile);

	std::string convertToString(const Tile& tile);
	std::string convertToString(int row, int col);

	void onPaint(HDC hdc);
	void onCommand(int type);

	void drawRect(HDC hdc, Gdiplus::Color color, int x, int y, int w, int h, bool sp = false);

private:
	char m_World[20][20];
	Tile m_StartPoint;
	Tile m_DestPoint;
	Tile m_LeastInOpen;

	TileList m_OpenList;
	TileList m_CloseList;

	std::list<Tile> m_Path;
};

