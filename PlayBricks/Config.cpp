#include "Config.h"
#include "QFile"
#include "QTextStream"
#include "QApplication"

Config *Config::instance = nullptr; //初始化单例指针

Config::Config()
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
		ts >> themeResourceUrl; //读取当前主题资源url
		f.close();
	}
	//否则创建设置文件并且使用默认的初始化设置进行游戏
	else
	{
		f.open(QIODevice::WriteOnly);
		f.close();
	}

	//开始的时候设置当前主题
	QFile themeFile(themeResourceUrl);
	themeFile.open(QIODevice::ReadOnly);
	qApp->setStyleSheet(themeFile.readAll());
	themeFile.close();
}

void Config::uninit()
{
	QFile f("config.ini");
	f.open(QIODevice::WriteOnly | QIODevice::Truncate);
	QTextStream ts(&f);
	ts << highestScore << ' '; //写入最高分
	ts << highestScorePlayer << ' '; //写入最高分保持者
	ts << themeResourceUrl << ' '; //写入当前主题资源url
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

void Config::setThemeResourceUrl(QString themeResourceUrl)
{
	this->themeResourceUrl = themeResourceUrl;
}

QString Config::getThemeResourceUrl()
{
	return themeResourceUrl;
}