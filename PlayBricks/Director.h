#ifndef DIRECTOR_H
#define DIRECTOR_H

#include "QLabel"

//导演类
class Director
{
private:
	Director(); //禁止显示构造实例
	static Director *instance; //单例模式的唯一性指针

public:
	~Director();
	static Director *getInstance(); //获得单例指针

	bool isCrash(QLabel *l1, QLabel *l2); //是否碰撞
};

#endif // !DIRECTOR_H
