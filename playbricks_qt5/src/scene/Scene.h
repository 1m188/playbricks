#pragma once

#include "Window.h"

// 场景类
class Scene : public QWidget
{
    Q_OBJECT

public:
    Scene(Window *parent);
    virtual ~Scene();

    virtual void init(); // 初始化场景

    // 将键盘响应重写在public中方便主窗口调用这些函数将响应在窗口的按键传递到场景之中
    virtual void keyPressEvent(QKeyEvent *event) override;
    virtual void keyReleaseEvent(QKeyEvent *event) override;
};
