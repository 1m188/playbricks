#pragma once

#include "Scene.h"

// 难度选择场景
class DifficultyChooseScene : public Scene
{
    Q_OBJECT

public:
    DifficultyChooseScene(Window *parent);
    ~DifficultyChooseScene();

    void init() override; // 初始化场景

private slots:
    void difficultyChooseButtonClicked();   // 单击难度选择按钮
    void returnToStartSceneButtonClicked(); // 单击返回开始场景按钮
};
