#ifndef DIRECTOR_H
#define DIRECTOR_H

#include "Window.h"

//导演类
class Director
{
private:
	Director(); //禁止显示构造实例

	static Director *instance; //单例模式的唯一性指针

	Window *window; //游戏窗口指针实例

public:
	Director(const Director &) = delete;
	Director &operator=(const Director &) = delete;
	~Director();

	static Director *getInstance(); //获得单例指针

	void setWindow(Window *window); //设置游戏窗口实例
	Window *getWindow(); //获取游戏窗口实例
};

#endif // !DIRECTOR_H
