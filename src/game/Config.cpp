#include "Config.h"
#include "QFile"
#include "QApplication"
#include "QJsonObject"
#include "QJsonDocument"

Config *Config::instance = nullptr; // 初始化单例指针

Config::Config()
{
}

Config::~Config()
{
}

Config *Config::getInstance()
{
    if (!instance)
    {
        instance = new Config();
    }
    return instance;
}

void Config::init()
{
    // 初始化默认设置
    highestScore = 0;
    highestScorePlayer = "None";
    themeResourceUrl = ":/theme/Resources/theme/default.qss";
    fps = 30;

    QFile f("config.ini");
    // 如果文件存在的话则读取相关设置
    if (f.exists())
    {
        // 从文件中读取json
        f.open(QIODevice::ReadOnly);
        QByteArray temp = f.readAll();
        f.close();

        // 解析json
        QJsonDocument jsonDocument = QJsonDocument::fromJson(temp);
        QJsonObject json = jsonDocument.object();
        highestScore = json.value("HighestScore").toInt();                // 读取最高分
        highestScorePlayer = json.value("HighestScorePlayer").toString(); // 读取最高分保持者
        themeResourceUrl = json.value("ThemeResourceUrl").toString();     // 读取主题资源样式表保存路径
        fps = json.value("Fps").toInt();                                  // 读取帧数
        // 控制读取进来的fps范围
        if (fps > 60)
        {
            fps = 60;
        }
        else if (fps < 30)
        {
            fps = 30;
        }
    }
    // 否则创建设置文件并且使用默认的初始化设置进行游戏
    else
    {
        f.open(QIODevice::WriteOnly);
        f.close();
    }

    // 开始的时候设置当前主题
    setTheme(themeResourceUrl);
}

void Config::uninit()
{
    // json格式写入
    QJsonObject json;
    json.insert("HighestScore", highestScore);             // 写入最高分
    json.insert("HighestScorePlayer", highestScorePlayer); // 写入最高分保持者
    json.insert("ThemeResourceUrl", themeResourceUrl);     // 写入当前主题资源url
    json.insert("Fps", fps);                               // 写入当前FPS值

    QJsonDocument jsonDocument;
    jsonDocument.setObject(json);
    QByteArray byteArray = jsonDocument.toJson();

    // 写入文件
    QFile f("config.ini");
    f.open(QIODevice::WriteOnly | QIODevice::Truncate);
    f.write(byteArray);
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

const int Config::getHighestScore() const
{
    return highestScore;
}

void Config::setHighestScorePlayer(QString highestScorePlayer)
{
    this->highestScorePlayer = highestScorePlayer;
}

const QString Config::getHighestScorePlayer() const
{
    return highestScorePlayer;
}

void Config::setFps(int fps)
{
    this->fps = fps;
}

const int Config::getFps() const
{
    return fps;
}