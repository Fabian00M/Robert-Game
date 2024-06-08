#ifndef PERSON_SPRITE_H
#define PERSON_SPRITE_H

#include "Sprite.h"
#include "Windows.h"

class PersonSprite : public Sprite
{
public:
	PersonSprite(BitMap*, RECT&, BOUNDSACTION ba = BA_STOP);
	virtual ~PersonSprite();
	virtual void UpdateFrame();
	void walk();
};

#endif