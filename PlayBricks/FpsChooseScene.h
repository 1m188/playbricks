#ifndef FPSCHOOSESCENE_H
#define FPSCHOOSESCENE_H

#include "Scene.h"

//fps选择场景
class FpsChooseScene : public Scene
{
	Q_OBJECT

public:
	FpsChooseScene(Window *parent);
	~FpsChooseScene();

	void init() override; //初始化场景
};

#endif // FPSCHOOSESCENE_H
