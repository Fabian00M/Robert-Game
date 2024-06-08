#ifndef BACKGROUND_H
#define BACKGROUND_H

#include "Windows.h"
#include "BitMap.h"

class BackGround {
protected:
	BitMap* img;
	COLORREF color;
	int width, height;

public:
	BackGround(BitMap*);
	BackGround(COLORREF, int, int);
	virtual ~BackGround();
	virtual void Draw(HDC);
	virtual void Update();
	int getWidth() { return width; };
	int getHeight() { return height; };

};

class StaryBackGround : BackGround {
protected:
	int numStars;
	int twinkleDelay;
	POINT stars[100];
	COLORREF starsColor[100];
public:
	StaryBackGround(int w, int h, int n = 100, int td = 50);
	virtual ~StaryBackGround();
	virtual void Draw(HDC);
	virtual void Update();

};

#endif