#ifndef GAMESCENE_H
#define GAMESCENE_H

#include "Scene.h"
#include "QPixmap"
#include "QLabel"
#include "QTimer"

//游戏主场景
class GameScene : public Scene
{
	Q_OBJECT

private:
	//一系列资源图片
	QPixmap ballPixmap; //球的图片
	QPixmap paddlePixmap; //挡板图片
	QVector<QPixmap> blockPixmapVector; //砖块图片（分三个颜色，所以是数组）

	QLabel *ballLabel; //球
	QLabel *paddleLabel; //挡板
	QVector<QVector<QLabel *>> blockLabelVector; //砖块二维数组

	QLabel *scoreLabel; //得分显示
	unsigned int nowScore; //当前得分

	unsigned int difficulty; //难度系数

	QTimer paddleMoveLeftTimer; //挡板左移动定时器
	QTimer paddleMoveRightTimer; //挡板右移动定时器
	QTimer ballMoveTimer; //球移动定时器

	QPair<int, int> ballMoveDxy; //球每次在x方向和y方向移动的距离
	int paddleMoveDx; //挡板在横向方向每次移动的距离

	void paddleMove(int distance); //挡板移动
	bool isCrash(QLabel *l1, QLabel *l2); //是否碰撞

public:
	GameScene(Window *parent);
	~GameScene();

	void init(int difficulty); //隐藏基类同名函数，初始化，传入难度，目前有0，1，2三个阶位

protected:
	void keyPressEvent(QKeyEvent *event) override; //重写按键事件
	void keyReleaseEvent(QKeyEvent *event) override; //重写释放按键事件

	private slots:
	void ballMoveSlot(); //球移动信号槽
};

#endif // GAMESCENE_H
