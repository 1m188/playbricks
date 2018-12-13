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
	return x >= s.getX() - width && x <= s.getX() + s.getWidth() && y >= s.getY() - height && y <= s.getY() + s.getHeight();
}