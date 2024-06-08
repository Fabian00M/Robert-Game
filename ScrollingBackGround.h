#ifndef SCROLLINGBACKGROUND_H
#define SCROLLINGBACKGROUND_H

#include "BackGround.h"
#include "Windows.h"

typedef WORD SCROLLDIR;

const SCROLLDIR SD_UP = 0,
				SD_RT = 1,
				SD_DW = 2,
				SD_LT = 3;

class BackGroundLayer :BitMap {

protected:
	RECT viewPort;
	int speed;
	SCROLLDIR direction;

public:
	BackGroundLayer(HDC, std::wstring, int, SCROLLDIR);
	BackGroundLayer(HDC, UINT, HINSTANCE, int, SCROLLDIR);
	virtual ~BackGroundLayer();
	virtual void Draw(HDC, int, int, bool btrans = false,
		COLORREF ctrans = RGB(255, 0, 255));
	virtual void Update();
	void setSpeed(int s) {
		speed = s;
	};
	void setDirection(SCROLLDIR d) {
		direction = d;
	};
	void setViewPort(RECT& vp) {
		CopyRect(&viewPort, &vp);
	}

};

class ScrollingBackGround : BackGround {

protected:
	int numLayers;
	BackGroundLayer* layers[10];
	
public:
	ScrollingBackGround(int, int);
	virtual ~ScrollingBackGround();
	virtual void Draw(HDC, bool btrans = false, COLORREF ctrans = RGB(255, 0, 255));
	virtual void Update();
	void addLayer(BackGroundLayer*);

};

#endif