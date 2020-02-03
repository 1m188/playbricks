#include "Ball.h"

Ball::Ball() :Sprite(), dx(0), dy(0), isMoving(false)
{

}

Ball::Ball(int x, int y, int width, int height, QPixmap image, int dx, int dy, bool isMoving) : Sprite(x, y, width, height, image), dx(dx), dy(dy), isMoving(isMoving)
{

}

Ball::~Ball()
{

}

void Ball::move(int left, int right, int up)
{
	setX(getX() + dx);
	setY(getY() + dy);

	//ײǽ
	if (getX() < left)
	{
		setX(left);
		dx *= -1;
	}
	else if (getX() + getWidth() > right)
	{
		setX(right - getWidth());
		dx *= -1;
	}
	if (getY() < up)
	{
		setY(up);
		dy *= -1;
	}
}