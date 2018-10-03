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

	//一些游戏元素的标签
	QLabel *ballLabel;
	QLabel *paddleLabel;
	QVector<QVector<QLabel *>> blockLabelVector; //砖块二维数组

	//挡板移动定时器
	QTimer paddleMoveLeftTimer; //左
	QTimer paddleMoveRightTimer; //右
	QTimer ballMoveTimer; //球移动定时器

	int ballMoveDx;
	int ballMoveDy;

protected:
	void keyPressEvent(QKeyEvent *event) override; //重写按键事件
	void keyReleaseEvent(QKeyEvent *event) override; //...释放按键...

	private slots:
	void paddleMoveLeftSlot(); //挡板向左移动信号槽
	void paddleMoveRightSlot(); //...右...
	void ballMoveSlot(); //球移动
};

#endif // MAINGUI_H
