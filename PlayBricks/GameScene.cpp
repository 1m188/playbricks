#include "GameScene.h"
#include "Director.h"
#include "StartScene.h"
#include "QApplication"
#include "QDeskTopWidget"
#include "QKeyEvent"
#include "QMessageBox"

GameScene::GameScene(Window *parent)
	: Scene(parent)
{

}

GameScene::~GameScene()
{

}

void GameScene::init(int difficulty)
{
	//游戏元素初始化
	//图片初始化
	ballPixmap.load(":/PlayBricks/Resources/ball.png");
	paddlePixmap.load(":/PlayBricks/Resources/paddle.png");
	blockPixmapVector.resize(3);
	blockPixmapVector[0].load(":/PlayBricks/Resources/block_green.png");
	blockPixmapVector[1].load(":/PlayBricks/Resources/block_blue.png");
	blockPixmapVector[2].load(":/PlayBricks/Resources/block_red.png");

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

	//初始化难度系数
	this->difficulty = difficulty;

	//砖块初始化
	for (int i = 0; i < 10; i++)
	{
		blockLabelVector.append(QVector<QLabel *>{});
		for (int j = 0; j < 16; j++)
		{
			QLabel *blockLabel = new QLabel(this);
			blockLabel->setPixmap(blockPixmapVector[difficulty]);
			blockLabel->setFixedSize(blockPixmapVector[difficulty].size());
			blockLabel->move(j*blockPixmapVector[difficulty].width(), i*blockPixmapVector[difficulty].height());
			blockLabelVector[blockLabelVector.size() - 1].append(blockLabel);
		}
	}

	//挡板移动定时器初始化
	paddleMoveLeftTimer.setInterval(10);
	paddleMoveRightTimer.setInterval(10);
	connect(&paddleMoveLeftTimer, &QTimer::timeout, this, &GameScene::paddleMoveLeftSlot);
	connect(&paddleMoveRightTimer, &QTimer::timeout, this, &GameScene::paddleMoveRightSlot);

	//球移动计时器
	ballMoveTimer.setInterval(20);
	connect(&ballMoveTimer, &QTimer::timeout, this, &GameScene::ballMoveSlot);

	//球移动距离
	ballMoveDx = 5;
	ballMoveDy = 5;
}

void GameScene::keyPressEvent(QKeyEvent * event)
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

void GameScene::keyReleaseEvent(QKeyEvent * event)
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

void GameScene::paddleMoveLeftSlot()
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

void GameScene::paddleMoveRightSlot()
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

void GameScene::ballMoveSlot()
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
	if (isCrash(ballLabel, paddleLabel))
	{
		ballMoveDy *= -1;
	}

	//判断是否撞到了砖块
	for (int i = 0; i < blockLabelVector.size(); i++)
	{
		for (int j = 0; j < blockLabelVector[i].size(); j++)
		{
			if (isCrash(ballLabel, blockLabelVector[i][j]) && !blockLabelVector[i][j]->isHidden())
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
		//显示游戏结束信息，如果不再来一局的话
		if (QMessageBox::information(this, tr(u8"游戏结束"), tr(u8"游戏结束，是否以当前难度再来一局？"), QMessageBox::Yes, QMessageBox::No) == QMessageBox::No)
		{
			StartScene *startScene = new StartScene(Director::getInstance()->getWindow());
			Director::getInstance()->setNowScene(startScene);
			startScene->init();
			startScene->show();
			deleteLater();
		}
		//否则重新再来一局，重新初始化所有游戏元素内容
		else
		{
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
}

bool GameScene::isCrash(QLabel * l1, QLabel * l2)
{
	return l1->x() >= l2->x() - l1->width() && l1->x() <= l2->x() + l2->width() && l1->y() >= l2->y() - l1->height() && l1->y() <= l2->y() + l2->height();
}