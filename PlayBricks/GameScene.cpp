#include "GameScene.h"
#include "Director.h"
#include "Config.h"
#include "StartScene.h"
#include "QKeyEvent"
#include "QMessageBox"
#include "QInputDialog"
#include "QPainter"
#include "GameSprite.h"

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
	ballPixmap.load(":/game/Resources/game/ball.png");
	paddlePixmap.load(":/game/Resources/game/paddle.png");
	bricksImageVector.resize(3);
	bricksImageVector[0].load(":/game/Resources/game/block_green.png");
	bricksImageVector[1].load(":/game/Resources/game/block_blue.png");
	bricksImageVector[2].load(":/game/Resources/game/block_red.png");

	//初始化难度系数
	this->difficulty = difficulty % 3; //防止传入的难度系数超过范围

	//挡板初始化
	paddle = Paddle(width() / 2 - paddlePixmap.width() / 2, height() - paddlePixmap.height() - 10, paddlePixmap.width(), paddlePixmap.height(), paddlePixmap, 6 - this->difficulty, false, false);

	//球初始化
	ball = Ball(paddle.getX() + paddle.getWidth() / 2 - ballPixmap.width() / 2, paddle.getY() - ballPixmap.height() - 10, ballPixmap.width(), ballPixmap.height(), ballPixmap, this->difficulty * 2 + 5, this->difficulty * 2 + 5, false);

	//得分初始化
	nowScore = 0;

	//砖块初始化
	for (int i = 0; i < (this->difficulty) * 4 + 10; i++)
	{
		bricksVector.append(QVector<Brick>{});
		for (int j = 0; j < 16; j++)
		{
			Brick brick(j*bricksImageVector.at(this->difficulty).width(), i*bricksImageVector.at(this->difficulty).height(), bricksImageVector.at(this->difficulty).width(), bricksImageVector.at(this->difficulty).height(), bricksImageVector.at(this->difficulty), false);
			bricksVector[bricksVector.size() - 1].append(brick);
		}
	}
}

void GameScene::keyPressEvent(QKeyEvent * event)
{
	if (!event->isAutoRepeat())
	{
		//挡板向左移动
		if (event->key() == Qt::Key_Left)
		{
			paddle.setIsLefting(true);
		}
		//挡板向右移动
		else if (event->key() == Qt::Key_Right)
		{
			paddle.setIsRighting(true);
		}
		//按住空格键球加速
		else if (event->key() == Qt::Key_Space)
		{
			ball.setDx(ball.getDx() * 3);
			ball.setDy(ball.getDy() * 3);
		}
		//如果球没有移动的话，按任意键后球开始移动
		if (!ball.getIsMoving())
		{
			ball.setIsMoving(true);
		}
	}
}

void GameScene::keyReleaseEvent(QKeyEvent * event)
{
	if (!event->isAutoRepeat())
	{
		//挡板停止向左移动
		if (event->key() == Qt::Key_Left)
		{
			paddle.setIsLefting(false);
		}
		//挡板停止向右移动
		else if (event->key() == Qt::Key_Right)
		{
			paddle.setIsRighting(false);
		}
		//释放空格键后球回到原来的速度
		else if (event->key() == Qt::Key_Space)
		{
			ball.setDx(ball.getDx() / 3);
			ball.setDy(ball.getDy() / 3);
		}
	}
}

void GameScene::paintEvent(QPaintEvent * event)
{
	//绘制
	QPainter *painter = new QPainter(this);

	//绘制挡板
	painter->drawPixmap(paddle.getX(), paddle.getY(), paddle.getImage());

	//绘制球
	painter->drawPixmap(ball.getX(), ball.getY(), ball.getImage());

	//绘制砖块
	for (auto &vector : bricksVector)
	{
		for (Brick &brick : vector)
		{
			if (!brick.getIsCollided())
			{
				painter->drawPixmap(brick.getX(), brick.getY(), brick.getImage());
			}
		}
	}

	painter->end();

	Scene::paintEvent(event);
}