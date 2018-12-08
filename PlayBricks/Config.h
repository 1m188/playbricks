#ifndef CONFIG_H
#define CONFIG_H

#include "QString"

//全局设置类
class Config
{
private:
	Config(); //禁止默认自动构造

	static Config *instance; //单例指针

	int highestScore; //最高分
	QString highestScorePlayer; //最高分保持者
	QString themeResourceUrl; //当前主题资源url
	int fps; //帧数

public:
	Config(const Config &) = delete;
	Config &operator=(const Config &) = delete;
	~Config();

	static Config *getInstance(); //获取单例指针

	void init(); //初始化所有的设置
	void uninit(); //保存已有设置

	void setTheme(QString themeResourceUrl); //设置当前主题

	void setHighestScore(int highestScore); //设置最高分分数
	const int getHighestScore() const; //获取最高分分数
	void setHighestScorePlayer(QString highestScorePlayer); //设置最高分保持者
	const QString getHighestScorePlayer() const; //获取最高分保持者
	void setFps(int fps); //设置FPS
	const int getFps() const; //获取FPS
};

#endif // CONFIG_H
