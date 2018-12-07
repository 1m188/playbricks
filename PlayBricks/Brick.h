#ifndef BRICK_H
#define BRICK_H

#include "Sprite.h"

//砖块类
class Brick :public Sprite
{
private:
	//是否被撞上过
	bool isCollided;

public:
	Brick();
	Brick(int x, int y, int width, int height, QPixmap image, bool isCollided);
	~Brick();

	//获取/设置是否被撞上过
	const bool getIsCollided() const { return isCollided; }
	void setIsCollided(bool isCollided) { this->isCollided = isCollided; }
};

#endif // BRICK_H
