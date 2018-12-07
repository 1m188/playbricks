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

	QLabel *sliderValueLabel = new QLabel(this);
	sliderValueLabel->setAlignment(Qt::AlignCenter);
	sliderValueLabel->setFont(QFont(u8"Î¢ÈíÑÅºÚ", 20));
	sliderValueLabel->setText(QString::number(30));

	QSlider *slider = new QSlider(this);
	slider->setOrientation(Qt::Horizontal);
	slider->setMinimum(30);
	slider->setMaximum(60);
	slider->setSingleStep(1);

	QPushButton *returnToStartScene = new QPushButton(this);
	returnToStartScene->setFont(QFont(u8"Î¢ÈíÑÅºÚ", 15));
	returnToStartScene->setText(tr(u8"·µ»Ø¿ªÊ¼½çÃæ"));

	//²¼¾Ö
	QGridLayout *layout = new QGridLayout(this);
	layout->addWidget(infoLabel, 0, 0, 10, 15);
	layout->addWidget(fpsLabel, 10, 0, 4, 5);
	layout->addWidget(slider, 10, 5, 4, 5);
	layout->addWidget(sliderValueLabel, 10, 10, 4, 5);
	layout->addWidget(returnToStartScene, 14, 5, 4, 5);
}