#include "FpsChooseScene.h"
#include "QLabel"
#include "QPushButton"
#include "QSlider"
#include "QGridLayout"

FpsChooseScene::FpsChooseScene(Window *parent)
	: Scene(parent)
{

}

FpsChooseScene::~FpsChooseScene()
{

}

void FpsChooseScene::init()
{
	//¿Ø¼þ
	QLabel *infoLabel = new QLabel(this);
	infoLabel->setAlignment(Qt::AlignCenter);
	infoLabel->setFont(QFont(u8"Î¢ÈíÑÅºÚ", 40));
	infoLabel->setText(tr(u8"Ö¡ Êý Ñ¡ Ôñ"));

	QLabel *fpsLabel = new QLabel(this);
	fpsLabel->setAlignment(Qt::AlignCenter);
	fpsLabel->setFont(QFont(u8"Î¢ÈíÑÅºÚ", 20));
	fpsLabel->setText(tr(u8"FPS"));

	QLabel *fpsValueLabel = new QLabel(this);
	fpsValueLabel->setAlignment(Qt::AlignCenter);
	fpsValueLabel->setFont(QFont(u8"Î¢ÈíÑÅºÚ", 20));
	fpsValueLabel->setText(QString::number(30));

	QSlider *fpsSlider = new QSlider(this);
	fpsSlider->setOrientation(Qt::Horizontal);
	fpsSlider->setMinimum(30);
	fpsSlider->setMaximum(60);
	fpsSlider->setSingleStep(1);

	QPushButton *returnToStartSceneButton = new QPushButton(this);
	returnToStartSceneButton->setFont(QFont(u8"Î¢ÈíÑÅºÚ", 15));
	returnToStartSceneButton->setText(tr(u8"·µ»Ø¿ªÊ¼½çÃæ"));

	//²¼¾Ö
	QGridLayout *layout = new QGridLayout(this);
	layout->addWidget(infoLabel, 0, 0, 10, 15);
	layout->addWidget(fpsLabel, 10, 0, 4, 5);
	layout->addWidget(fpsSlider, 10, 5, 4, 5);
	layout->addWidget(fpsValueLabel, 10, 10, 4, 5);
	layout->addWidget(returnToStartSceneButton, 14, 5, 4, 5);
}