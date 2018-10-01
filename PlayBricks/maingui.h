#ifndef MAINGUI_H
#define MAINGUI_H

#include <QWidget>
#include "QPixmap"
#include "QLabel"

#ifdef _DEBUG
#include "QDebug"
#endif // _DEBUG


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

	bool isStart; //游戏是否开始

protected:
	void keyPressEvent(QKeyEvent *event) override; //重写按键事件
};

#endif // MAINGUI_H
