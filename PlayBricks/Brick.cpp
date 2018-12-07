#include "Brick.h"

Brick::Brick() :Sprite(), isCollided(false)
{

}

Brick::Brick(int x, int y, int width, int height, QPixmap image, bool isCollided) : Sprite(x, y, width, height, image), isCollided(false)
{

}

Brick::~Brick()
{

}
