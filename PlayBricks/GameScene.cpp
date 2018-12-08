#include "GameScene.h"
#include "Director.h"
#include "Config.h"
#include "StartScene.h"
#include "QKeyEvent"
#include "QMessageBox"
#include "QInputDialog"
#include "QPainter"

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
	this->difficulty = difficulty % 3; //防止传入的参数超过限制

	//得分初始化
	nowScore = 0;

	//分数的文本高度初始化
	scoreTextHeight = 20;

	//帧数初始化
	fps = Config::getInstance()->getFps();

	//挡板初始化
	paddle = Paddle(width() / 2 - paddlePixmap.width() / 2, height() - paddlePixmap.height() - 10, paddlePixmap.width(), paddlePixmap.height(), paddlePixmap, (15 - this->difficulty * 2) * 30 / fps, false, false);

	//球初始化
	ball = Ball(paddle.getX() + paddle.getWidth() / 2 - ballPixmap.width() / 2, paddle.getY() - ballPixmap.height() - 10, ballPixmap.width(), ballPixmap.height(), ballPixmap, (this->difficulty * 3 + 5) * 30 / fps, (this->difficulty * 3 + 5) * 30 / fps, false);

	//砖块初始化
	for (int i = 0; i < this->difficulty * 4 + 10; i++)
	{
		bricksVector.append(QVector<Brick>{});
		for (int j = 0; j < 16; j++)
		{
			Brick brick(j*bricksImageVector.at(this->difficulty).width(), scoreTextHeight + i*bricksImageVector.at(this->difficulty).height(), bricksImageVector.at(this->difficulty).width(), bricksImageVector.at(this->difficulty).height(), bricksImageVector.at(this->difficulty), false);
			bricksVector[bricksVector.size() - 1].append(brick);
		}
	}

	//初始化并启动定时器
	timer = new QTimer(this);
	connect(timer, &QTimer::timeout, this, &GameScene::gameCycle);
	timer->setInterval(1000 / fps);
	timer->start();
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

void GameScene::paintEvent(QPaintEvent *event)
{
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
			//没有被撞过的才绘制
			if (!brick.getIsCollided())
			{
				painter->drawPixmap(brick.getX(), brick.getY(), brick.getImage());
			}
		}
	}

	//绘制分数
	painter->setPen(Qt::red);
	painter->setFont(QFont(u8"微软雅黑", 12));
	painter->drawText(0, 0, width(), scoreTextHeight, Qt::AlignCenter, QString(u8"分数：%1").arg(nowScore));

	painter->end();

	Scene::paintEvent(event);
}

void GameScene::gameCycle()
{
	//计算状态
	//挡板移动
	if (paddle.getIsLefting())
	{
		paddle.setX(paddle.getX() - paddle.getDx());
	}
	if (paddle.getIsRighting())
	{
		paddle.setX(paddle.getX() + paddle.getDx());
	}

	//挡板碰到地图边界
	if (paddle.getX() < 0)
	{
		paddle.setX(0);
	}
	else if (paddle.getX() + paddle.getWidth() > width())
	{
		paddle.setX(width() - paddle.getWidth());
	}

	//球移动
	if (ball.getIsMoving())
	{
		ball.setX(ball.getX() + ball.getDx());
		ball.setY(ball.getY() + ball.getDy());
	}

	//球碰到地图边界
	//碰到左边界
	if (ball.getX() < 0)
	{
		ball.setX(0);
		ball.setDx(ball.getDx()*-1);
	}
	//碰到右边界
	else if (ball.getX() + ball.getWidth() > width())
	{
		ball.setX(width() - ball.getWidth());
		ball.setDx(ball.getDx()*-1);
	}
	//碰到上边界
	else if (ball.getY() < scoreTextHeight)
	{
		ball.setY(scoreTextHeight);
		ball.setDy(ball.getDy()*-1);
	}

	//球是否和挡板碰撞
	if (isCrash(ball, paddle))
	{
		updateBallDxy(paddle);
	}

	//球是否和砖块碰撞
	for (auto &vector : bricksVector)
	{
		for (Brick &brick : vector)
		{
			if (isCrash(ball, brick) && !brick.getIsCollided())
			{
				updateBallDxy(brick);
				brick.setIsCollided(true);
				nowScore += difficulty + 1;
				goto outside;
			}
		}
	}
outside:;

	//游戏是否结束
	//球落地
	if (ball.getY() + ball.getHeight() >= height())
	{
		//停止更新画面
		timer->stop();

		//游戏结束之后如果为最高分的话，更新最高分和最高分保持者
		if (nowScore > Config::getInstance()->getHighestScore())
		{
			Config::getInstance()->setHighestScore(nowScore);
			Config::getInstance()->setHighestScorePlayer(QInputDialog::getText(this, tr(u8"最高分"), tr(u8"你的分数是%1，恭喜你成为最高分，请输入你的尊姓大名。").arg(nowScore)));
		}
		//显示游戏结束信息，如果不再来一局的话回到开始场景
		if (QMessageBox::information(this, tr(u8"游戏结束"), tr(u8"游戏结束，你的分数是%1，是否以当前难度再来一局？").arg(nowScore), QMessageBox::Yes, QMessageBox::No) == QMessageBox::No)
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
			//重新设置挡板状态
			paddle.setX(width() / 2 - paddle.getWidth() / 2);
			paddle.setY(height() - paddle.getHeight() - 10);
			paddle.setIsLefting(false);
			paddle.setIsRighting(false);

			//重新设置球的状态
			ball.setX(paddle.getX() + paddle.getWidth() / 2 - ball.getWidth() / 2);
			ball.setY(paddle.getY() - ball.getHeight() - 10);
			ball.setDx((this->difficulty * 3 + 5) * 30 / fps);
			ball.setDy((this->difficulty * 3 + 5) * 30 / fps);
			ball.setIsMoving(false);

			//重新初始化分数
			nowScore = 0;

			//重新显示所有的砖块
			for (auto &vector : bricksVector)
			{
				for (Brick &brick : vector)
				{
					brick.setIsCollided(false);
				}
			}

			//开始更新画面
			timer->start();
		}
	}
	//砖块打完
	else
	{
		//对每个砖块遍历判断，如果有砖块显示（没被打）的话就跳出去，否则进行砖块打完之后的行为
		for (auto &vector : bricksVector)
		{
			for (Brick &brick : vector)
			{
				if (!brick.getIsCollided())
				{
					goto judgeEnd;
				}
			}
		}

		//除了分数不重新刷新之外，其他的重新刷新重来一遍，直到某一次球落地为止，此间的分数一直累加
		//重新设置挡板状态
		paddle.setX(width() / 2 - paddle.getWidth() / 2);
		paddle.setY(height() - paddle.getHeight() - 10);
		paddle.setIsLefting(false);
		paddle.setIsRighting(false);

		//重新设置球的状态
		ball.setX(paddle.getX() + paddle.getWidth() / 2 - ball.getWidth() / 2);
		ball.setY(paddle.getY() - ball.getHeight() - 10);
		ball.setDx((this->difficulty * 3 + 5) * 30 / fps);
		ball.setDy((this->difficulty * 3 + 5) * 30 / fps);
		ball.setIsMoving(false);

		//重新显示所有的砖块
		for (auto &vector : bricksVector)
		{
			for (Brick &brick : vector)
			{
				brick.setIsCollided(false);
			}
		}
	}
judgeEnd:;

	//绘制
	update();
}

bool GameScene::isCrash(Sprite s1, Sprite s2) const
{
	return s1.getX() >= s2.getX() - s1.getWidth() && s1.getX() <= s2.getX() + s2.getWidth() && s1.getY() >= s2.getY() - s1.getHeight() && s1.getY() <= s2.getY() + s2.getHeight();
}

void GameScene::updateBallDxy(Sprite s)
{
	//获取球和物体相撞的时候的共同的宽和高
	int overWidth = ((ball.getX() + ball.getWidth() < s.getX() + s.getWidth()) ? (ball.getX() + ball.getWidth()) : (s.getX() + s.getWidth())) - (ball.getX() > s.getX() ? ball.getX() : s.getX());
	int overHeight = ((ball.getY() + ball.getHeight() < s.getY() + s.getHeight()) ? (ball.getY() + ball.getHeight()) : (s.getY() + s.getHeight())) - (ball.getY() > s.getY() ? ball.getY() : s.getY());
	//判定哪边更长，如果是宽说明球的趋势应是从上或者下相撞的，否则应该是左右相撞，极端情况则完全反射
	if (overWidth > overHeight)
	{
		ball.setDy(ball.getDy()*-1);
	}
	else if (overWidth < overHeight)
	{
		ball.setDx(ball.getDx()*-1);
	}
	else
	{
		ball.setDx(ball.getDx()*-1);
		ball.setDy(ball.getDy()*-1);
	}
}