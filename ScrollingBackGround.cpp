#include "ScrollingBackGround.h"
#include "Windows.h"

BackGroundLayer::BackGroundLayer(HDC hdc, std::wstring fileName,int spd, 
	SCROLLDIR dir):BitMap(hdc, fileName) {
	
	speed = spd;
	direction = dir;
	viewPort.top = viewPort.left = 0;
	viewPort.right = width;
	viewPort.bottom = height;

}

BackGroundLayer::BackGroundLayer(HDC hdc, UINT resInt, HINSTANCE currInstance, 
	int spd, SCROLLDIR dir) : BitMap(hdc, resInt, currInstance){

	speed = spd;
	direction = dir;
	viewPort.top = viewPort.left = 0;
	viewPort.right = width;
	viewPort.bottom = height;

}

BackGroundLayer::~BackGroundLayer() {
	//blank
}

void BackGroundLayer::Draw(HDC hdc, int x, int y, bool btrans,
	COLORREF ctrans) {

	if (viewPort.top < 0 && viewPort.left < 0) {
		drawPart(hdc, x, y, (width + viewPort.left), 
			(height + viewPort.top), -viewPort.left, -viewPort.top,
			btrans, ctrans);
		drawPart(hdc, (x - viewPort.left), y, 0, (height + viewPort.top),
			viewPort.right, -viewPort.top, btrans, ctrans);
		drawPart(hdc, x, (y - viewPort.top), (width + viewPort.left), 0,
			-viewPort.left, viewPort.bottom, btrans, ctrans);
		drawPart(hdc, x - viewPort.left, y - viewPort.top, 0, 0,
			viewPort.right, viewPort.bottom, btrans, ctrans);
	}
	else if (viewPort.top < 0 && viewPort.right > width) {
		drawPart(hdc, x, y, viewPort.left, height + viewPort.top,
			width - viewPort.left, -viewPort.top,
			btrans, ctrans);
		drawPart(hdc, x + width - viewPort.left, y, 0,
			height - viewPort.top, viewPort.right - width, -viewPort.top,
			btrans, ctrans);
		drawPart(hdc, x, y - viewPort.top, viewPort.left, 0,
			width - viewPort.left, viewPort.bottom, btrans, ctrans);
		drawPart(hdc, x + width - viewPort.left, y - viewPort.top,
			0, 0, viewPort.right - width, viewPort.bottom, btrans, ctrans);
	}
	else if (viewPort.bottom > height && viewPort.left < 0) {
		drawPart(hdc, x, y, width + viewPort.left, viewPort.top,
			-viewPort.left, height - viewPort.top, btrans, ctrans);
		drawPart(hdc, x - viewPort.left, y, 0, viewPort.top, viewPort.right,
			height - viewPort.top, btrans, ctrans);
		drawPart(hdc, x, y + (height - viewPort.top), width + viewPort.left,
			0, -viewPort.left, viewPort.bottom - height, btrans, ctrans);
		drawPart(hdc, x - viewPort.left, y + (height - viewPort.top),
			0, 0, viewPort.right, viewPort.bottom - height, btrans, ctrans);
	}
	else if (viewPort.right > width && viewPort.bottom > height) {
		drawPart(hdc, x, y, viewPort.left, viewPort.top,
			width - viewPort.left, height - viewPort.top, btrans, ctrans);
		drawPart(hdc, x + (width - viewPort.left), y, 0, viewPort.top,
			viewPort.right - width, height - viewPort.top, btrans, ctrans);
		drawPart(hdc, x, y + (height - viewPort.top), viewPort.left,
			0, width - viewPort.left, viewPort.bottom - height, btrans, ctrans);
		drawPart(hdc, x + (width - viewPort.left), y + (height - viewPort.top),
			0, 0, viewPort.right - width, viewPort.bottom - height, btrans, ctrans);
	}
	else if (viewPort.top < 0) {
		drawPart(hdc, x, y, viewPort.left, height + viewPort.top,
			viewPort.right - viewPort.left, -viewPort.top, btrans, ctrans);
		drawPart(hdc, x, y - viewPort.top, viewPort.left, 0,
			viewPort.right - viewPort.left, viewPort.bottom, btrans, ctrans);
	}
	else if (viewPort.bottom > height) {
		drawPart(hdc, x, y, viewPort.left, viewPort.top,
			viewPort.right - viewPort.left, height - viewPort.top, btrans, ctrans);
		drawPart(hdc, x, y + (height - viewPort.top), viewPort.left, 0,
			viewPort.right - viewPort.left, viewPort.bottom - height, btrans, ctrans);
	}
	else if (viewPort.right > width) {
		drawPart(hdc, x, y, viewPort.left, viewPort.top, width - viewPort.left,
			viewPort.bottom - viewPort.top, btrans, ctrans);
		drawPart(hdc, x + (width - viewPort.left), y, 0, viewPort.top,
			viewPort.right - width, viewPort.bottom - viewPort.top, btrans, ctrans);
	}
	else if (viewPort.left < 0) {
		drawPart(hdc, x, y, width + viewPort.left, viewPort.top,
			-viewPort.left, viewPort.bottom - viewPort.top, btrans, ctrans);
		drawPart(hdc, x - viewPort.left, y, 0, viewPort.top,
			viewPort.right, viewPort.bottom - viewPort.top, btrans, ctrans);
	}
	else {
		drawPart(hdc, x, y, viewPort.left, viewPort.top,
			viewPort.right - viewPort.left, viewPort.bottom - viewPort.top, btrans, ctrans);
	}

}

void BackGroundLayer::Update() {
	switch (direction) {
		case SD_LT:
			viewPort.right += speed;
			viewPort.left += speed;
			if (viewPort.left > width) {
				viewPort.right = viewPort.right - viewPort.left;
				viewPort.left = 0;
			}
			break;
		case SD_RT:
			viewPort.right -= speed;
			viewPort.left -= speed;
			if (viewPort.right < 0) {
				viewPort.left = width - (viewPort.right - viewPort.left);
				viewPort.right = width;
			}
			break;
		case SD_UP:
			viewPort.top += speed;
			viewPort.bottom += speed;
			if (viewPort.top > height) {
				viewPort.bottom = viewPort.bottom - viewPort.top;
				viewPort.top = 0;
			}
			break;
		case SD_DW:
			viewPort.top -= speed;
			viewPort.bottom -= speed;
			if (viewPort.bottom < 0) {
				viewPort.top = height - (viewPort.bottom - viewPort.top);
				viewPort.bottom = height;
			}
			break;
	}
}

ScrollingBackGround::ScrollingBackGround(int x, int y) : BackGround(RGB(0, 0, 0), x, y) {
	numLayers = 0;
}


ScrollingBackGround::~ScrollingBackGround() {
	//empty
}

void ScrollingBackGround::Draw(HDC hdc, bool btrans, COLORREF ctrans) {
	for (int i = 0; i < numLayers; i++) {
		layers[i]->Draw(hdc, 0, 0, btrans, ctrans);
	}
}

void ScrollingBackGround::Update() {
	for (int i = 0; i < numLayers; i++) {
		layers[i]->Update();
	}
}

void ScrollingBackGround::addLayer(BackGroundLayer* layer) {
	if (numLayers < 10) {
		layers[numLayers++] = layer;
	}
}

