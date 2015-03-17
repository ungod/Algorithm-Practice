#pragma once
#include "vision.h"
#include <list>

/*
 * MaxRect Algorithm
 * by Lynx
 */

class MaxRect :
	public Vision
{
private:
	struct RectInput
	{
		int width;
		int height;
		RectInput(){};
		RectInput(int w, int h):width(w), height(h){}
	};
	struct RectOutput
	{
		int x, y;
		int width, height;
		bool rotate;
		RectOutput():rotate(false){}
		RectOutput(int _x, int _y, int _w, int _h):x(_x), y(_y), width(_w), height(_h), rotate(false){}
	};
	struct FreeRect
	{
		int x, y;
		int width, height;
		int bottom, right;
		FreeRect(){}
		FreeRect(int _x, int _y, int _w, int _h):x(_x), y(_y), width(_w), height(_h)
		{
			refreshBR();
		}
		FreeRect(const RectOutput& rect)
		{
			x = rect.x;
			y = rect.y;
			width = rect.width;
			height = rect.height;
			refreshBR();
		}
		FreeRect(const FreeRect& rect)
		{
			x = rect.x;
			y = rect.y;
			width = rect.width;
			height = rect.height;
			refreshBR();
		}
		
		void refreshBR()
		{
			right = x + width;
			bottom = y + height;
		}

		void refreshWH()
		{
			width = right - x;
			height = bottom - y;
		}
	};
public:
	MaxRect(void);
	~MaxRect(void);

	void doAlgorithm();

	void onPaint(HDC hdc);
	void onCommand(int type);

private:
	void drawRect(HDC hdc, Gdiplus::Color color, int x, int y, int w, int h, bool sp = false);
	void drawString(HDC hdc, WCHAR* str, int size, int x, int y);

private:
	std::list<RectInput> m_input;
	std::list<RectOutput> m_output;
	std::list<FreeRect> m_freeRect;

	int m_curPower;
	int m_curSize;
};

