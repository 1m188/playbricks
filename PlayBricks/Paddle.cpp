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
