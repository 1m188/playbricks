#pragma once

#include "Scene.h"

// fps选择场景
class FpsChooseScene : public Scene
{
    Q_OBJECT

public:
    FpsChooseScene(Window *parent);
    ~FpsChooseScene();

    void init() override; // 初始化场景

private slots:
    void returnToStartSceneButtonClicked(); // 单击返回开始场景按钮
};
