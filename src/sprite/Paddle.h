#pragma once

#include "Sprite.h"

// 挡板类
class Paddle : public Sprite
{
private:
    // 每次横向移动的正距离
    int dx;

    // 是否向左/右移动
    bool isLefting;
    bool isRighting;

public:
    Paddle();
    Paddle(int x, int y, int width, int height, QPixmap image, int dx, bool isLefting, bool isRighting);
    ~Paddle();

    // 获取/设置每次横向移动的正距离
    const int getDx() const { return dx; }
    void setDx(int dx) { this->dx = dx; }

    // 获取/设置是否向左/右移动
    const bool getIsLefting() const { return isLefting; }
    void setIsLefting(bool isLefting) { this->isLefting = isLefting; }
    const bool getIsRighting() const { return isRighting; }
    void setIsRighting(bool isRighting) { this->isRighting = isRighting; }

    // 挡板移动，传入左右极限边界
    void move(int left, int right);
};
