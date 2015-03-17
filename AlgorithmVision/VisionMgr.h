#pragma once

#include <vector>
#include "Vision.h"

class VisionMgr
{
public:
	VisionMgr(void);
	~VisionMgr(void);

	void init(HWND hwnd);
	void redraw();

	void paintCallback(HDC hdc);
	void commandCallback(int type);

private:
	void initVisionClass();

private:
	std::vector<Vision*> m_list;
	HWND m_hwnd;

public:
	static VisionMgr* m_pInstance;
	static VisionMgr* Instance()
	{
		if (m_pInstance == NULL)
		{
			m_pInstance = new VisionMgr;
		}
		return m_pInstance;
	}
};

