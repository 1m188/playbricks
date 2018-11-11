#ifndef MAINGUI_H
#define MAINGUI_H

#include <QWidget>
#include "QPixmap"
#include "QLabel"
#include "QTimer"

//游戏主界面
class MainGui : public QWidget
{
	Q_OBJECT

public:
	MainGui(QWidget *parent);
	~MainGui();

private:
	//一系列资源图片
	QPixmap ballPixmap;
	QPixmap paddlePixmap;
	QPixmap block_bluePixmap;
	QPixmap block_greenPixmap;
	QPixmap block_redPixmap;

	QLabel *ballLabel; //球
	QLabel *paddleLabel; //挡板
	QVector<QVector<QLabel *>> blockLabelVector; //砖块二维数组

	//挡板移动定时器
	QTimer paddleMoveLeftTimer; //左
	QTimer paddleMoveRightTimer; //右
	QTimer ballMoveTimer; //球移动定时器

	int ballMoveDx; //球每次在x方向移动的距离
	int ballMoveDy; //球每次在y方向移动的距离

	bool isCrash(QLabel *l1, QLabel *l2); //是否碰撞

protected:
	void keyPressEvent(QKeyEvent *event) override; //重写按键事件
	void keyReleaseEvent(QKeyEvent *event) override; //...释放按键...

	private slots:
	void paddleMoveLeftSlot(); //挡板向左移动信号槽
	void paddleMoveRightSlot(); //...右...
	void ballMoveSlot(); //球移动
};

#endif // MAINGUI_H
