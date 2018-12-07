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
