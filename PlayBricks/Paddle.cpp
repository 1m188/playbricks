#include "Paddle.h"

Paddle::Paddle() :Sprite(), dx(0), isLefting(false), isRighting(false)
{

}

Paddle::Paddle(int x, int y, int width, int height, QPixmap image, int dx, bool isLefting, bool isRighting) : Sprite(x, y, width, height, image), dx(dx), isLefting(isLefting), isRighting(isRighting)
{

}

Paddle::~Paddle()
{

}

void Paddle::move(int left, int right)
{
	//×óÓÒÒÆ¶¯
	if (isLefting)
	{
		setX(getX() - dx);
	}
	if (isRighting)
	{
		setX(getX() + dx);
	}

	//×²Ç½
	if (getX() < left)
	{
		setX(left);
	}
	else if (getX() + getWidth() > right)
	{
		setX(right - getWidth());
	}
}