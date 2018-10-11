#include "maingui.h"
#include "QApplication"
#include "QDeskTopWidget"
#include "QKeyEvent"
#include "QMessageBox"
#include "Director.h"

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

	//游戏元素初始化
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

	//挡板移动定时器初始化
	paddleMoveLeftTimer.setInterval(10);
	paddleMoveRightTimer.setInterval(10);
	connect(&paddleMoveLeftTimer, &QTimer::timeout, this, &MainGui::paddleMoveLeftSlot);
	connect(&paddleMoveRightTimer, &QTimer::timeout, this, &MainGui::paddleMoveRightSlot);

	//球移动计时器
	ballMoveTimer.setInterval(20);
	connect(&ballMoveTimer, &QTimer::timeout, this, &MainGui::ballMoveSlot);

	//球移动距离
	ballMoveDx = 5;
	ballMoveDy = 5;
}

MainGui::~MainGui()
{

}

void MainGui::keyPressEvent(QKeyEvent * event)
{
	if (event->key() == Qt::Key_Left) //挡板向左移动
	{
		//只允许向左的定时器工作
		paddleMoveLeftTimer.start();
		paddleMoveRightTimer.stop();
	}
	else if (event->key() == Qt::Key_Right)
	{
		paddleMoveRightTimer.start();
		paddleMoveLeftTimer.stop();
	}
}

void MainGui::keyReleaseEvent(QKeyEvent * event)
{
	//释放按键则停止挡板移动
	if (event->key() == Qt::Key_Left)
	{
		paddleMoveLeftTimer.stop();
	}
	else if (event->key() == Qt::Key_Right)
	{
		paddleMoveRightTimer.stop();
	}
	//球开始移动
	else if (event->key() == Qt::Key_Space)
	{
		ballMoveTimer.start();
	}
}

void MainGui::paddleMoveLeftSlot()
{
	int x = paddleLabel->x(); //获取当前的x坐标
	x -= 5; //移动
	if (x < 0) //判定向左移动是否会超出边界
	{
		paddleLabel->move(0, paddleLabel->y());
	}
	else
	{
		paddleLabel->move(x, paddleLabel->y());
	}
}

void MainGui::paddleMoveRightSlot()
{
	int x = paddleLabel->x(); //获取当前的x坐标
	x += 5; //移动
	if (x + paddleLabel->width() > width()) //判定向左移动是否会超出边界
	{
		paddleLabel->move(width() - paddleLabel->width(), paddleLabel->y());
	}
	else
	{
		paddleLabel->move(x, paddleLabel->y());
	}
}

void MainGui::ballMoveSlot()
{
	//更新球坐标
	int x = ballLabel->x() + ballMoveDx;
	int y = ballLabel->y() + ballMoveDy;

	//如果球超出边界，则把球限制在边界之内，同时改变速度方向
	if (x + ballLabel->width() > width())
	{
		x = width() - ballLabel->width();
		ballMoveDx *= -1;
	}
	if (x < 0)
	{
		x = 0;
		ballMoveDx *= -1;
	}
	if (y + ballLabel->height() > height())
	{
		y = height() - ballLabel->height();
		ballMoveDy *= -1;
	}
	if (y < 0)
	{
		y = 0;
		ballMoveDy *= -1;
	}
	//刷新球的位置
	ballLabel->move(x, y);

	//判断球是否和挡板相撞
	if (Director::getInstance()->isCrash(ballLabel, paddleLabel))
	{
		ballMoveDy *= -1;
	}

	//判断是否撞到了砖块
	for (int i = 0; i < blockLabelVector.size(); i++)
	{
		for (int j = 0; j < blockLabelVector[i].size(); j++)
		{
			if (Director::getInstance()->isCrash(ballLabel, blockLabelVector[i][j]) && !blockLabelVector[i][j]->isHidden())
			{
				ballMoveDy *= -1;
				blockLabelVector[i][j]->hide();
				goto outside;
			}
		}
	}
outside:;

	//判断是否游戏结束
	if (ballLabel->y() + ballLabel->height() >= height())
	{
		//停止一切
		paddleMoveLeftTimer.stop();
		paddleMoveRightTimer.stop();
		ballMoveTimer.stop();
		//显示信息
		QMessageBox::about(this, u8"游戏结束", u8"游戏结束啦！");

		//重新摆放挡板位置
		paddleLabel->move(width() / 2 - paddleLabel->width() / 2, height() - paddleLabel->height() - 10);
		//重新摆放球的位置并重新设定球的移动方向
		ballLabel->move(paddleLabel->x() + paddleLabel->width() / 2 - ballLabel->width() / 2, paddleLabel->y() - ballLabel->height() - 10);
		ballMoveDx = 5;
		ballMoveDy = 5;
		//重新显示所有的砖块
		for (int i = 0; i < blockLabelVector.size(); i++)
		{
			for (int j = 0; j < blockLabelVector[i].size(); j++)
			{
				blockLabelVector[i][j]->show();
			}
		}
	}
}