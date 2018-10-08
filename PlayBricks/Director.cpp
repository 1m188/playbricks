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

bool Director::isCrash(QLabel * l1, QLabel * l2)
{
	return l1->x() >= l2->x() - l1->width() && l1->x() <= l2->x() + l2->width() && l1->y() >= l2->y() - l1->height() && l1->y() <= l2->y() + l2->height();
}