#pragma once
class Vision
{
public:
	Vision(void);
	~Vision(void);

	virtual void onPaint(HDC hdc) = 0;
	virtual void onCommand(int type) = 0;
};