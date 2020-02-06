#pragma once

#include "QPixmap"

// 精灵类
class Sprite
{
private:
    // 左上角坐标
    int x;
    int y;

    // 宽高
    int width;
    int height;

    // 图片
    QPixmap image;

public:
    Sprite();
    Sprite(int x, int y, int width, int height, QPixmap image);
    virtual ~Sprite();

    // 获取/设置坐标
    int getX() const { return x; }
    void setX(int x) { this->x = x; }
    int getY() const { return y; }
    void setY(int y) { this->y = y; }

    // 获取/设置宽高
    int getWidth() const { return width; }
    void setWidth(int width) { this->width = width; }
    int getHeight() const { return height; }
    void setHeight(int height) { this->height = height; }

    // 获取/设置图片
    QPixmap getImage() const { return image; }
    void setImage(QPixmap image) { this->image = image; }

    // 碰撞检测
    bool isCollided(Sprite s) const;
};
