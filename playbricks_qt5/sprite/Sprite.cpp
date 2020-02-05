#include "Sprite.h"

Sprite::Sprite() :Sprite(0, 0, 0, 0, QPixmap())
{

}

Sprite::Sprite(int x, int y, int width, int height, QPixmap image) : x(x), y(y), width(width), height(height), image(image)
{

}

Sprite::~Sprite()
{

}

bool Sprite::isCollided(Sprite s) const
{
	return x >= s.x - width && x <= s.x + s.width && y >= s.y - height && y <= s.y + s.height;
}