#include "stdafx.h"
#include "VisionMgr.h"
#include "Dijkstra.h"
#include "MaxRect.h"

VisionMgr* VisionMgr::m_pInstance = NULL;

VisionMgr::VisionMgr(void)
{
}


VisionMgr::~VisionMgr(void)
{
}

void VisionMgr::initVisionClass()
{
	m_list.push_back(new MaxRect);
}

void VisionMgr::paintCallback( HDC hdc )
{
	std::vector<Vision*>::iterator it;
	for (it = m_list.begin(); it != m_list.end(); it++)
	{
		(*it)->onPaint(hdc);
	}
}

void VisionMgr::init(HWND hwnd)
{
	m_hwnd = hwnd;
	initVisionClass();
}

void VisionMgr::commandCallback( int type )
{
	std::vector<Vision*>::iterator it;
	for (it = m_list.begin(); it != m_list.end(); it++)
	{
		(*it)->onCommand(type);
	}
}

void VisionMgr::redraw()
{
	InvalidateRect(m_hwnd, NULL, true);
	UpdateWindow(m_hwnd);
}
