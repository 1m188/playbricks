#include "maingui.h"
#include "QApplication"
#include "QDeskTopWidget"
#include "gamescene.h"

MainGui::MainGui(QWidget *parent)
	: QWidget(parent)
{
	//界面基本设置
	setAttribute(Qt::WA_DeleteOnClose, true);
	setAttribute(Qt::WA_QuitOnClose, true);

	//标题和大小
	setWindowTitle(tr("QtGame"));
	setFixedSize(1000, 800);

	//移动到屏幕中央
	QRect rect = frameGeometry();
	rect.moveCenter(QApplication::desktop()->availableGeometry().center());
	move(rect.topLeft());

	GameScene *gameScene = new GameScene(this);
	gameScene->setFixedSize(size());
}

MainGui::~MainGui()
{

}
