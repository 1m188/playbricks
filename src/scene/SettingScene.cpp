#include "SettingScene.h"
#include "Director.h"
#include "StartScene.h"
#include "ThemeChooseScene.h"
#include "FpsChooseScene.h"
#include "QLabel"
#include "QPushButton"
#include "QGridLayout"

SettingScene::SettingScene(Window *parent) : Scene(parent)
{
}

SettingScene::~SettingScene()
{
}

void SettingScene::init()
{
    // 控件
    QLabel *infoLabel = new QLabel(this);
    infoLabel->setAlignment(Qt::AlignCenter);
    infoLabel->setFont(QFont(u8"微软雅黑", 40));
    infoLabel->setText(tr(u8"设  置"));

    QPushButton *themeButton = new QPushButton(this);
    themeButton->setFont(QFont(u8"微软雅黑", 15));
    themeButton->setText(tr(u8"主题选择"));
    connect(themeButton, &QPushButton::clicked, this, &SettingScene::themeButtonClicked);

    QPushButton *fpsButton = new QPushButton(this);
    fpsButton->setFont(QFont(u8"微软雅黑", 15));
    fpsButton->setText(tr(u8"帧数选择"));
    connect(fpsButton, &QPushButton::clicked, this, &SettingScene::FpsButtonClicked);

    QPushButton *returnToStartSceneButton = new QPushButton(this);
    returnToStartSceneButton->setFont(QFont(u8"微软雅黑", 15));
    returnToStartSceneButton->setText(tr(u8"返回开始界面"));
    connect(returnToStartSceneButton, &QPushButton::clicked, this, &SettingScene::returnToStartSceneButtonClicked);

    // 布局
    QGridLayout *layout = new QGridLayout(this);
    layout->addWidget(infoLabel, 0, 0, 10, 15);
    layout->addWidget(themeButton, 10, 5, 2, 5);
    layout->addWidget(fpsButton, 12, 5, 2, 5);
    layout->addWidget(returnToStartSceneButton, 14, 5, 2, 5);
}

void SettingScene::themeButtonClicked()
{
    ThemeChooseScene *themeChooseScene = new ThemeChooseScene(Director::getInstance()->getWindow());
    Director::getInstance()->setNowScene(themeChooseScene);
    themeChooseScene->init();
    themeChooseScene->show();
    deleteLater();
}

void SettingScene::FpsButtonClicked()
{
    FpsChooseScene *fpsChooseScene = new FpsChooseScene(Director::getInstance()->getWindow());
    Director::getInstance()->setNowScene(fpsChooseScene);
    fpsChooseScene->init();
    fpsChooseScene->show();
    deleteLater();
}

void SettingScene::returnToStartSceneButtonClicked()
{
    StartScene *startScene = new StartScene(Director::getInstance()->getWindow());
    Director::getInstance()->setNowScene(startScene);
    startScene->init();
    startScene->show();
    deleteLater();
}