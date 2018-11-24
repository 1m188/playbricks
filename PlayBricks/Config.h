#ifndef CONFIG_H
#define CONFIG_H

#include "QString"

//全局设置类
class Config
{
private:
	Config(); //禁止默认自动构造

	static Config *instance; //单例指针

	int highestScore = 0; //最高分
	QString highestScorePlayer = "None"; //最高分保持者
	QString themeResourceUrl = ":/theme/Resources/theme/default.qss"; //当前主题资源url

public:
	Config(const Config &) = delete;
	Config &operator=(const Config &) = delete;
	~Config();

	static Config *getInstance(); //获取单例指针

	void init(); //初始化所有的设置
	void uninit(); //保存已有设置

	void setHighestScore(int highestScore); //设置最高分分数
	int getHighestScore(); //获取最高分分数
	void setHighestScorePlayer(QString highestScorePlayer); //设置最高分保持者
	QString getHighestScorePlayer(); //获取最高分保持者
	void setThemeResourceUrl(QString themeResourceUrl); //设置当前主题资源url
	QString getThemeResourceUrl(); //获取当前主题资源url
};

#endif // CONFIG_H
