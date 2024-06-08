#include "PersonSprite.h"

PersonSprite::PersonSprite(BitMap* bm, RECT& boundry, BOUNDSACTION ba):
	Sprite(bm, boundry, ba) {}
PersonSprite::~PersonSprite() {}
void PersonSprite::UpdateFrame() {}
void PersonSprite::walk() {
	curFrame = 1 - curFrame;
}