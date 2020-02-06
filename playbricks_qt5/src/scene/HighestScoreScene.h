#pragma once

#include "Scene.h"

// 最高分场景
class HighestScoreScene : public Scene
{
    Q_OBJECT

public:
    HighestScoreScene(Window *parent);
    ~HighestScoreScene();

    void init() override; // 初始化场景

private slots:
    void returnToStartSceneButtonClicked(); // 单击返回开始界面按钮
};
