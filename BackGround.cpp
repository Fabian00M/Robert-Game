#include "BackGround.h"

BackGround::BackGround(BitMap* bitmap) {
	img = bitmap;
	color = 0;
	width = bitmap->getWidth();
	height = bitmap->getHeight();
}

BackGround::BackGround(COLORREF c, int w, int h) {
	img = NULL;
	color = c;
	width = w;
	height = h;
}

BackGround::~BackGround() {

}

void BackGround::Draw(HDC hdc) {
	if (img == NULL) {
		RECT bck = { 0,0,width, height };
		HBRUSH brush = CreateSolidBrush(color);
		FillRect(hdc, &bck, brush);
		DeleteObject(brush);
	}
	else {
		img->draw(hdc, 0, 0);
	}
}

void BackGround::Update() {
	// Nothing, yay
}

StaryBackGround::StaryBackGround(int w, int h, int n, int td):
	BackGround(RGB(0, 0, 0), w, h) {
	numStars = min(n, 100);
	twinkleDelay = td;
	for (int i = 0; i < numStars; i++) {

		stars[i].x = rand() % w;
		stars[i].y = rand() % h;
		starsColor[i] = RGB(128, 128, 128);

	}
}

StaryBackGround::~StaryBackGround() {}

void StaryBackGround::Draw(HDC hdc) {

	RECT bck = { 0,0,width, height };
	HBRUSH brush = CreateSolidBrush(RGB(0, 0, 0));
	FillRect(hdc, &bck, brush);
	DeleteObject(brush);

	for (int i = 0; i < numStars; i++) {

		SetPixel(hdc, stars[i].x, stars[i].y, starsColor[i]);

	}

}

void StaryBackGround::Update() {

	for (int i = 0; i < numStars; i++) {

		if (rand() % twinkleDelay == 0) {

			int c = rand() % 256;
			starsColor[i] = RGB(c, c, c);

		}

	}

}