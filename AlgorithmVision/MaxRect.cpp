#include "stdafx.h"
#include "MaxRect.h"
#include <math.h>

#include "resource.h"
#include <time.h>
#include "VisionMgr.h"

MaxRect::MaxRect(void)
{
	//m_input.clear();
	//m_output.clear();
	//m_freeRect.clear();
	srand((unsigned int)time(NULL));

	//m_input.push_back(RectInput(10, 40));
	//m_input.push_back(RectInput(50, 20));
	//m_input.push_back(RectInput(100, 200));
	//m_input.push_back(RectInput(50, 99));
	//m_input.push_back(RectInput(112, 400));
	//m_input.push_back(RectInput(555, 10));

	m_curPower = 6;

	m_curSize = (int)pow(2, m_curPower);
	m_freeRect.push_back(FreeRect(0, 0, m_curSize, m_curSize));
}


MaxRect::~MaxRect(void)
{
}

void MaxRect::onPaint( HDC hdc )
{
	Gdiplus::Graphics g(hdc);
	g.Clear(Gdiplus::Color::White);

	doAlgorithm();

	drawRect(hdc, Gdiplus::Color::Gray, 0, 0, m_curSize, m_curSize);

	std::list<RectOutput>::iterator it;
	for (it = m_output.begin(); it != m_output.end(); it++)
	{
		drawRect(hdc, Gdiplus::Color::Blue, it->x, it->y, it->width, it->height);
	}

	std::list<FreeRect>::iterator rit;
	for (rit = m_freeRect.begin(); rit != m_freeRect.end(); rit++)
	{
		drawRect(hdc, Gdiplus::Color::Yellow, rit->x, rit->y, rit->width, rit->height, true);
	}

	WCHAR wstr[256];
	memset(wstr, 0, 256);
	swprintf_s(wstr, 256, L"Rect Count : %d", m_output.size());
	drawString(hdc, wstr, 256, 1000, 10);

	memset(wstr, 0, 256);
	swprintf_s(wstr, 256, L"FreeRect Count : %d", m_freeRect.size());
	drawString(hdc, wstr, 256, 950, 30);

	memset(wstr, 0, 256);
	swprintf_s(wstr, 256, L" Bin Size : %d x %d", m_curSize, m_curSize);
	drawString(hdc, wstr, 256, 950, 50);
}


void MaxRect::drawString(HDC hdc, WCHAR* str, int size, int x, int y)
{
	using namespace Gdiplus;

	Graphics g(hdc);
	Font f(L"Arial", 12);
	PointF p(x, y);
	SolidBrush b(Color::Black);

	g.DrawString(str, size, &f, p, &b);
}


void MaxRect::onCommand( int type )
{
	if (type == ID_MAXRECT_ADD)
	{
		m_input.push_back(RectInput(rand()%128, rand()%128));
		VisionMgr::Instance()->redraw();
	}
}

void MaxRect::drawRect( HDC hdc, Gdiplus::Color color, int x, int y, int w, int h, bool sp)
{
	using namespace Gdiplus;

	Graphics g(hdc);
	Pen pen(sp ? Color::Red : Color::Black, 1);
	SolidBrush b(sp ? Color(50, 255, 255, 0) : color);

	g.FillRectangle(&b, x, y, w, h);
	g.DrawRectangle(&pen, x, y, w, h);
}

void MaxRect::doAlgorithm()
{
	RectInput input;
	RectOutput output;
	
	int extend,offset = 0;

	while (m_input.size() != 0)
	{
		input = m_input.front();
		m_input.pop_front();

		std::list<FreeRect>::iterator curFreeRect = m_freeRect.begin();
		while (true)
		{
			if (curFreeRect->width >= input.width && curFreeRect->height >= input.height ||
				curFreeRect->width >= input.height && curFreeRect->height >= input.width)
			{

				if (curFreeRect->width < input.width || curFreeRect->height < input.height)
				{
					//rotate the rect if necessary
					output.width = input.height;
					output.height = input.width;
					output.rotate = true;
				}
				else
				{
					output.width = input.width;
					output.height = input.height;
				}

				//put new rect to left-up corner
				output.x = curFreeRect->x;
				output.y = curFreeRect->y;

				m_output.push_back(output);

				//calc new free rects by max-rect scheme at last
				FreeRect frectW(output.x, output.y+output.height, curFreeRect->width, curFreeRect->height-output.height);
				FreeRect frectH(output.x+output.width, output.y, curFreeRect->width-output.width, curFreeRect->height);

				m_freeRect.erase(curFreeRect);

				FreeRect outputRect(output);
				FreeRect intstRect;

				std::list<FreeRect> splitRects;
				std::list<FreeRect>::iterator it = m_freeRect.begin();

				while (it != m_freeRect.end())
				{
					intstRect.x = (outputRect.x < it->x) ? it->x : outputRect.x;
					intstRect.y = (outputRect.y < it->y) ? it->y : outputRect.y;
					intstRect.right = (outputRect.right > it->right) ? it->right : outputRect.right;
					intstRect.bottom = (outputRect.bottom > it->bottom) ? it->bottom : outputRect.bottom;
					intstRect.refreshWH();

					//there is intersect rect between currentRect to outputRect
					if (intstRect.width > 0 && intstRect.height > 0)
					{
						FreeRect rects[4] = 
						{
							FreeRect(*it), FreeRect(*it),
							FreeRect(*it), FreeRect(*it)
						};
						rects[0].right = intstRect.x;
						rects[1].bottom = intstRect.y;
						rects[2].x = intstRect.right;
						rects[3].y = intstRect.bottom;

						for (int i=0; i<4; i++)
						{
							rects[i].refreshWH();
							if (rects[i].width > 0 && rects[i].height > 0)
							{
								splitRects.push_back(rects[i]);
							}
						}

						it = m_freeRect.erase(it);
					}
					else
					{
						it++;
					}
				}

				//copy split rects to free rects list
				for (it = splitRects.begin(); it != splitRects.end(); it++)
				{
					m_freeRect.push_back(*it);
				}

				//add new freeRect by maxRect scheme before merge
				m_freeRect.push_back(frectW);
				m_freeRect.push_back(frectH);

				//now merge the overlay elements by twins
				std::list<FreeRect>::iterator fst;
				std::list<FreeRect>::iterator scn;
				for (fst = m_freeRect.begin();fst != m_freeRect.end(); fst++)
				{
					scn = fst;
					scn++;
					if (scn == m_freeRect.end())
					{
						break;
					}

					while (scn != m_freeRect.end())
					{
						if (scn->x >= fst->x && scn->y >= fst->y && 
							scn->right <= fst->right && scn->bottom <= fst->bottom)
						{
							scn = m_freeRect.erase(scn);
						}
						else if (fst->x >= scn->x && fst->y >= scn->y && 
							fst->right <= scn->right && fst->bottom <= scn->bottom)
						{
							*fst = *scn;
							scn = m_freeRect.erase(scn);
						}
						else
						{
							scn++;
						}
					}
				}

				break;
			}
			else 
			{
				curFreeRect++;
				if (curFreeRect == m_freeRect.end())
				{
					//its too small, expand the whole rect
					//bigger current input at least

					extend = 0;
					while (extend < input.width || extend < input.height)
					{
						m_curPower++;
						extend = (int)pow(2, m_curPower);
					}

					offset = extend - m_curSize;

					for (curFreeRect = m_freeRect.begin(); curFreeRect != m_freeRect.end(); curFreeRect++)
					{
						if (curFreeRect->x + curFreeRect->width >= m_curSize)
						{
							curFreeRect->width += offset;
							curFreeRect->refreshBR();
						}
						if (curFreeRect->y + curFreeRect->height >= m_curSize)
						{
							curFreeRect->height += offset;
							curFreeRect->refreshBR();
						}
					}
					m_curSize = extend;
					curFreeRect = m_freeRect.begin();
				}
			}
		}
	}
}