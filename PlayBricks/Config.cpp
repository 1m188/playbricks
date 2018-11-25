#include "Config.h"
#include "QFile"
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
	//初始化默认设置
	highestScore = 0;
	highestScorePlayer = "None";
	themeResourceUrl = ":/theme/Resources/theme/default.qss";

	QFile f("config.ini");
	//如果文件存在的话则读取相关设置
	if (f.exists())
	{
		f.open(QIODevice::ReadOnly);
		QString temp;
		//读取最高分
		temp = f.readLine();
		temp.chop(1);
		highestScore = temp.toInt();
		//读取最高分保持者
		temp = f.readLine();
		temp.chop(1);
		highestScorePlayer = temp;
		//读取当前主题资源url
		temp = f.readLine();
		temp.chop(1);
		themeResourceUrl = temp;
		f.close();
	}
	//否则创建设置文件并且使用默认的初始化设置进行游戏
	else
	{
		f.open(QIODevice::WriteOnly);
		f.close();
	}

	//开始的时候设置当前主题
	setTheme(themeResourceUrl);
}

void Config::uninit()
{
	QFile f("config.ini");
	f.open(QIODevice::WriteOnly | QIODevice::Truncate);
	f.write(QString::number(highestScore).toStdString().c_str()); //写入最高分
	f.write("\n");
	f.write(highestScorePlayer.toStdString().c_str()); //写入最高分保持者
	f.write("\n");
	f.write(themeResourceUrl.toStdString().c_str()); //写入当前主题资源url
	f.write("\n");
	f.close();
}

void Config::setTheme(QString themeResourceUrl)
{
	QFile f(themeResourceUrl);
	f.open(QIODevice::ReadOnly);
	qApp->setStyleSheet(f.readAll());
	f.close();
	this->themeResourceUrl = themeResourceUrl;
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