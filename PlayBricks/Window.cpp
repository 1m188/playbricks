#include "Window.h"
#include "Director.h"
#include "StartScene.h"
#include "QApplication"
#include "QDeskTopWidget"

Window::Window()
	: QWidget(Q_NULLPTR)
{
	//界面基本设置
	setAttribute(Qt::WA_DeleteOnClose, true);
	setAttribute(Qt::WA_QuitOnClose, true);

	//标题和大小
	setWindowTitle(tr(u8"打砖块"));
	setFixedSize(1000, 800);

	//移动到屏幕中央
	QRect rect = frameGeometry();
	rect.moveCenter(QApplication::desktop()->availableGeometry().center());
	move(rect.topLeft());
}

Window::~Window()
{

}

void Window::keyPressEvent(QKeyEvent * event)
{
	Director::getInstance()->getNowScene()->keyPressEvent(event);
}

void Window::keyReleaseEvent(QKeyEvent * event)
{
	Director::getInstance()->getNowScene()->keyReleaseEvent(event);
}

void Window::show()
{
	StartScene *startScene = new StartScene(this);
	Director::getInstance()->setNowScene(startScene);
	startScene->init();
	startScene->show();
	QWidget::show();
}