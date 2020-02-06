#pragma once

#include "Sprite.h"

// 球类
class Ball : public Sprite
{
private:
    // 每次移动的x、y方向上的距离
    int dx;
    int dy;

    // 是否在移动
    bool isMoving;

public:
    Ball();
    Ball(int x, int y, int width, int height, QPixmap image, int dx, int dy, bool isMoving);
    ~Ball();

    // 获取/设置每次移动的x、y方向上的距离
    const int getDx() const { return dx; }
    void setDx(int dx) { this->dx = dx; }
    const int getDy() { return dy; }
    void setDy(int dy) { this->dy = dy; }

    // 获取/设置是否在移动
    const int getIsMoving() const { return isMoving; }
    void setIsMoving(bool isMoving) { this->isMoving = isMoving; }

    // 球的移动，传入左右上极限边界
    void move(int left, int right, int up);
};
