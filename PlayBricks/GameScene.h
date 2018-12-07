#ifndef GAMESCENE_H
#define GAMESCENE_H

#include "Scene.h"
#include "Paddle.h"
#include "Ball.h"
#include "Brick.h"
#include "QTimer"

//游戏主场景
class GameScene : public Scene
{
	Q_OBJECT

private:
	//一系列资源图片
	QPixmap ballPixmap; //球的图片
	QPixmap paddlePixmap; //挡板图片
	QVector<QPixmap> bricksImageVector; //砖块图片（分三个颜色，所以是数组）

	Paddle paddle; //挡板
	Ball ball; //球
	QVector<QVector<Brick>> bricksVector; //砖块二维数组

	QTimer *timer; //每隔一定时间进行计算、绘制的定时器

	int difficulty; //难度系数
	int nowScore; //当前得分

	bool isCrash(Sprite s1, Sprite s2) const; //是否碰撞
	void updateBallDxy(Sprite s); //通过原来的每次移动距离和与球相撞的物体来更新球的下一步移动方向

public:
	GameScene(Window *parent);
	~GameScene();

	void init(int difficulty); //隐藏基类同名函数，初始化，传入难度，目前有0，1，2三个阶位

	void keyPressEvent(QKeyEvent *event) override; //重写按键事件
	void keyReleaseEvent(QKeyEvent *event) override; //重写释放按键事件

protected:
	void paintEvent(QPaintEvent *event) override;

	private slots:
	void gameCycle(); //游戏循环
};

#endif // GAMESCENE_H
