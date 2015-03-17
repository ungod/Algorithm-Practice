#include "stdafx.h"
#include "AStar.h"
#include <stdlib.h>


AStar::AStar(void)
{
	init();
}


AStar::~AStar(void)
{
}

void AStar::init()
{
	memset(m_World, 0, sizeof(m_World)/sizeof(m_World[0][0]));

	m_World[10][10] = 
		m_World[11][10] = 
		m_World[12][10] = 
		m_World[13][10] = 
		m_World[14][10] = 
		m_World[15][10] = 
		m_World[16][10] = 1;

	m_StartPoint.row = 13;
	m_StartPoint.col = 4;
	m_StartPoint.lcol = m_StartPoint.lrow = -1;
	calcFGH(m_StartPoint);

	m_DestPoint.row = 13;
	m_DestPoint.col = 13;

	addToOpenList(m_StartPoint);
}

void AStar::doAlgorithm()
{
	while (true)
	{
		calcLeastFInOpen();

		if (m_LeastInOpen.H <= 0) break;

		m_OpenList.erase(m_OpenList.find(convertToString(m_LeastInOpen)));
		m_CloseList.insert(make_pair(convertToString(m_LeastInOpen), m_LeastInOpen));

		calcLeastFourTile();
	}

	TileList::iterator it;
	Tile tile = m_LeastInOpen;

	while (true)
	{
		it = m_CloseList.find(convertToString(tile.lrow, tile.lcol));
		tile = it->second;
		m_Path.push_back(tile);

		//start point
		if (tile.lrow == -1) break;
	}
}

void AStar::calcLeastFourTile()
{
	Tile tile, tempTile;
	tile = m_LeastInOpen;
	tempTile.lrow = tile.row;
	tempTile.lcol = tile.col;

	if (canCreateTile(tile.row, tile.col + 1))
	{
		tempTile.row = tile.row;
		tempTile.col = tile.col + 1;

		calcFGH(tempTile);
		addToOpenList(tempTile);
	}
	if (canCreateTile(tile.row - 1, tile.col))
	{
		tempTile.row = tile.row - 1;
		tempTile.col = tile.col;

		calcFGH(tempTile);
		addToOpenList(tempTile);
	}
	if (canCreateTile(tile.row, tile.col - 1))
	{
		tempTile.row = tile.row;
		tempTile.col = tile.col - 1;

		calcFGH(tempTile);
		addToOpenList(tempTile);
	}
	if (canCreateTile(tile.row + 1, tile.col))
	{
		tempTile.row = tile.row + 1;
		tempTile.col = tile.col;

		calcFGH(tempTile);
		addToOpenList(tempTile);
	}
}

std::string AStar::convertToString( const Tile& tile )
{
	char cstr[50];
	sprintf_s(cstr, "%d,%d", tile.row, tile.col);

	return std::string(cstr);
}

std::string AStar::convertToString( int row, int col )
{
	char cstr[50];
	sprintf_s(cstr, "%d,%d", row, col);

	return std::string(cstr);
}

bool AStar::canCreateTile( int row, int col )
{
	bool isInCloseList, isBlock;

	TileList::iterator it = m_CloseList.find(convertToString(row, col));

	isInCloseList = it != m_CloseList.end();
	isBlock = bool(m_World[row][col]);

	return !(isInCloseList || isBlock);
}

void AStar::addToOpenList( const Tile& tile )
{
	std::string tileString = convertToString(tile);
	TileList::iterator it = m_OpenList.find(tileString);

	//means its no tile in openlist
	if (it == m_OpenList.end())
	{
		m_OpenList.insert(make_pair(tileString, tile));
	}
	//the tile exist, compare with exist one and replace if H value is bigger
	else if (it->second.H >= tile.H)
	{
		m_OpenList.erase(it);
		m_OpenList.insert(make_pair(tileString, tile));
	}
}

void AStar::calcLeastFInOpen()
{
	TileList::iterator it = m_OpenList.begin();
	m_LeastInOpen.F = 0xffffffff;

	while (it != m_OpenList.end())
	{
		if (it->second.F <= m_LeastInOpen.F)
		{
			m_LeastInOpen = it->second;
		}
		it++;
	}
}

void AStar::onCommand( int type )
{

}

void AStar::onPaint( HDC hdc )
{
	using namespace Gdiplus;

	int w = 20;
	int h = 20;

	Graphics g(hdc);
	Pen pen(Color::Black);
	SolidBrush rb(Color::Red);
	SolidBrush gb(Color::Gray);
	SolidBrush geb(Color::Green);
	SolidBrush yeb(Color::Yellow);
	SolidBrush blb(Color::Blue);

	for (int i=0; i<20; i++)
	{
		for (int j=0; j<20; j++)
		{
			g.FillRectangle(m_World[i][j] == 1 ? &rb : &gb,  j*h, i*w, w, h);
			g.DrawRectangle(&pen, j*h, i*w, w, h);
		}
	}

	g.FillRectangle(&geb, m_StartPoint.col*h, m_StartPoint.row*w, w, h);
	g.DrawRectangle(&pen, m_StartPoint.col*h, m_StartPoint.row*w, w, h);

	g.FillRectangle(&blb, m_DestPoint.col*h, m_DestPoint.row*w, w, h);
	g.DrawRectangle(&pen, m_DestPoint.col*h, m_DestPoint.row*w, w, h);

	doAlgorithm();

	std::list<Tile>::iterator it;

	for (it = m_Path.begin(); it != m_Path.end(); it++)
	{
		g.FillRectangle(&yeb, it->col*h, it->row*w, w, h);
		g.DrawRectangle(&pen, it->col*h, it->row*w, w, h);
	}
}

void AStar::drawRect( HDC hdc, Gdiplus::Color color, int x, int y, int w, int h, bool sp)
{
}

void AStar::calcFGH(Tile& tile)
{
	if (tile.lrow == -1)
	{
		tile.G = 0;
	}
	else
	{
		TileList::iterator it = m_CloseList.find(convertToString(tile.lrow, tile.lcol));
		Tile last = it->second;

		tile.G = last.G + 1;
	}

	tile.H = abs(m_DestPoint.row -  tile.row) + abs(m_DestPoint.col - tile.col);
	tile.F = tile.G + tile.H;
}
