#include "FpsChooseScene.h"
#include "Director.h"
#include "Config.h"
#include "StartScene.h"
#include "QLabel"
#include "QPushButton"
#include "QSlider"
#include "QGridLayout"

FpsChooseScene::FpsChooseScene(Window *parent) : Scene(parent)
{
}

FpsChooseScene::~FpsChooseScene()
{
}

void FpsChooseScene::init()
{
    // 控件
    QLabel *infoLabel = new QLabel(this);
    infoLabel->setAlignment(Qt::AlignCenter);
    infoLabel->setFont(QFont(u8"微软雅黑", 40));
    infoLabel->setText(tr(u8"帧  数  选  择"));

    QLabel *fpsLabel = new QLabel(this);
    fpsLabel->setAlignment(Qt::AlignCenter);
    fpsLabel->setFont(QFont(u8"微软雅黑", 20));
    fpsLabel->setText(tr(u8"FPS"));

    QLabel *fpsValueLabel = new QLabel(this);
    fpsValueLabel->setAlignment(Qt::AlignCenter);
    fpsValueLabel->setFont(QFont(u8"微软雅黑", 20));
    fpsValueLabel->setText(QString::number(Config::getInstance()->getFps()));

    QSlider *fpsSlider = new QSlider(this);
    fpsSlider->setOrientation(Qt::Horizontal);
    fpsSlider->setMinimum(30);
    fpsSlider->setMaximum(60);
    fpsSlider->setSingleStep(1);
    fpsSlider->setValue(fpsValueLabel->text().toInt());
    // fps值显示的label上的显示值随slider滑动而变化
    connect(fpsSlider, &QSlider::valueChanged, this, [=](int value) { fpsValueLabel->setText(QString::number(value)); });
    // 直到用户释放slider确定之后才更改设置类里的fps值
    connect(fpsSlider, &QSlider::sliderReleased, this, [=]() { Config::getInstance()->setFps(fpsSlider->value()); });

    QPushButton *returnToStartSceneButton = new QPushButton(this);
    returnToStartSceneButton->setFont(QFont(u8"微软雅黑", 15));
    returnToStartSceneButton->setText(tr(u8"返回开始界面"));
    connect(returnToStartSceneButton, &QPushButton::clicked, this, &FpsChooseScene::returnToStartSceneButtonClicked);

    // 布局
    QGridLayout *layout = new QGridLayout(this);
    layout->addWidget(infoLabel, 0, 0, 10, 15);
    layout->addWidget(fpsLabel, 10, 0, 4, 5);
    layout->addWidget(fpsSlider, 10, 5, 4, 5);
    layout->addWidget(fpsValueLabel, 10, 10, 4, 5);
    layout->addWidget(returnToStartSceneButton, 14, 5, 4, 5);
}

void FpsChooseScene::returnToStartSceneButtonClicked()
{
    StartScene *startScene = new StartScene(Director::getInstance()->getWindow());
    Director::getInstance()->setNowScene(startScene);
    startScene->init();
    startScene->show();
    deleteLater();
}