#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>

//游戏主窗口
class Window : public QWidget
{
	Q_OBJECT

public:
	Window();
	~Window();

	void show(); //重写show函数，以放入一个开始场景

protected:
	void keyPressEvent(QKeyEvent *event) override;
	void keyReleaseEvent(QKeyEvent *event) override;
};

#endif // WINDOW_H
