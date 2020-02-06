#pragma once

#include "Scene.h"

// 导演类
class Director
{
private:
    Director(); // 禁止显示构造实例

    static Director *instance; // 单例模式的唯一性指针

    Window *window = nullptr;  // 游戏窗口指针实例
    Scene *nowScene = nullptr; // 当前受响应场景

public:
    Director(const Director &) = delete;
    Director &operator=(const Director &) = delete;
    ~Director();

    static Director *getInstance(); // 获得单例指针

    void setWindow(Window *window);    // 设置游戏窗口实例
    Window *getWindow();               // 获取游戏窗口实例
    void setNowScene(Scene *nowScene); // 设置当前受响应场景
    Scene *getNowScene();              // 获取当前受响应场景
};
