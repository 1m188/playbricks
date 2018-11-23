#include "Config.h"
#include "QFile"
#include "QTextStream"

Config *Config::instance = nullptr; //初始化单例指针

Config::Config() :highestScore(0), highestScorePlayer("None")
{

}

Config::~Config()
{

}

Config * Config::getInstance()
{
	if (!instance)
	{
		instance = new Config();
	}
	return instance;
}

void Config::init()
{
	QFile f("config.ini");
	//如果文件存在的话则读取相关设置
	if (f.exists())
	{
		f.open(QIODevice::ReadOnly);
		QTextStream ts(&f);
		ts >> highestScore; //读取最高分
		ts >> highestScorePlayer; //读取最高分保持者
		f.close();
	}
	//否则创建设置文件并且使用默认的初始化设置进行游戏
	else
	{
		f.open(QIODevice::WriteOnly);
		f.close();
	}
}

void Config::uninit()
{
	QFile f("config.ini");
	f.open(QIODevice::WriteOnly | QIODevice::Truncate);
	QTextStream ts(&f);
	ts << highestScore << ' '; //写入最高分
	ts << highestScorePlayer << ' '; //写入最高分保持者
	f.close();
}

void Config::setHighestScore(int highestScore)
{
	this->highestScore = highestScore;
}

int Config::getHighestScore()
{
	return highestScore;
}

void Config::setHighestScorePlayer(QString highestScorePlayer)
{
	this->highestScorePlayer = highestScorePlayer;
}

QString Config::getHighestScorePlayer()
{
	return highestScorePlayer;
}
