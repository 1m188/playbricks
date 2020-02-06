#pragma once

#include "Sprite.h"

// 单个砖块类
class Brick : public Sprite
{
private:
    // 是否被撞过
    bool isCollided;

public:
    Brick();
    Brick(int x, int y, int width, int height, QPixmap image, bool isCollided);
    ~Brick();

    // 获取/设置是否被撞过
    const bool getIsCollided() const { return isCollided; }
    void setIsCollided(bool isCollided) { this->isCollided = isCollided; }
};
