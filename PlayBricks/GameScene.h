#ifndef GAMESCENE_H
#define GAMESCENE_H

#include "Scene.h"
#include "CalculateStatusThread.h"

//游戏主场景
class GameScene : public Scene
{
	Q_OBJECT

private:
	//一系列资源图片
	QPixmap ballPixmap; //球的图片
	QPixmap paddlePixmap; //挡板图片
	QVector<QPixmap> bricksImageVector; //砖块图片（分三个颜色，所以是数组）

	int difficulty; //难度系数
	int nowScore; //当前得分

public:
	GameScene(Window *parent);
	~GameScene();

	void init(int difficulty); //隐藏基类同名函数，初始化，传入难度，目前有0，1，2三个阶位

	void keyPressEvent(QKeyEvent *event) override; //重写按键事件
	void keyReleaseEvent(QKeyEvent *event) override; //重写释放按键事件

protected:
	void paintEvent(QPaintEvent *event) override; //重写绘制事件
};

#endif // GAMESCENE_H
