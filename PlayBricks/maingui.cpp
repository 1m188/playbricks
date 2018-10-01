#include "maingui.h"
#include "QApplication"
#include "QDeskTopWidget"
#include "QKeyEvent"

MainGui::MainGui(QWidget *parent)
	: QWidget(parent)
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

	//游戏相关初始化
	isStart = false; //游戏还未开始

	//图片初始化
	ballPixmap.load(":/PlayBricks/Resources/ball.png");
	paddlePixmap.load(":/PlayBricks/Resources/paddle.png");
	block_bluePixmap.load(":/PlayBricks/Resources/block_blue.png");
	block_greenPixmap.load(":/PlayBricks/Resources/block_green.png");
	block_redPixmap.load(":/PlayBricks/Resources/block_red.png");

	//挡板初始化
	paddleLabel = new QLabel(this);
	paddleLabel->setPixmap(paddlePixmap);
	paddleLabel->setFixedSize(paddlePixmap.size());
	paddleLabel->move(width() / 2 - paddleLabel->width() / 2, height() - paddleLabel->height() - 10);

	//球初始化
	ballLabel = new QLabel(this);
	ballLabel->setPixmap(ballPixmap);
	ballLabel->setFixedSize(ballPixmap.size());
	ballLabel->move(paddleLabel->x() + paddleLabel->width() / 2 - ballLabel->width() / 2, paddleLabel->y() - ballLabel->height() - 10);

	//砖块初始化
	for (int i = 0; i < 10; i++)
	{
		blockLabelVector.append(QVector<QLabel *>{});
		for (int j = 0; j < 16; j++)
		{
			QLabel *blockLabel = new QLabel(this);
			blockLabel->setPixmap(block_bluePixmap);
			blockLabel->setFixedSize(block_bluePixmap.size());
			blockLabel->move(j*block_bluePixmap.width(), i*block_bluePixmap.height());
			blockLabelVector[blockLabelVector.size() - 1].append(blockLabel);
		}
	}
}

MainGui::~MainGui()
{

}

void MainGui::keyPressEvent(QKeyEvent * event)
{
#ifdef _DEBUG
	qDebug() << event->key();
#endif // _DEBUG
}