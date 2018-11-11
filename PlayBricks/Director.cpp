#include "Director.h"

Director *Director::instance = nullptr; //初始化单例指针

Director::Director()
{

}

Director::~Director()
{

}

Director * Director::getInstance()
{
	//如果单例指针为空，则构造一个
	if (!instance)
	{
		instance = new Director();
	}
	return instance;
}

void Director::setWindow(Window * window)
{
	this->window = window;
}

Window * Director::getWindow()
{
	return window;
}