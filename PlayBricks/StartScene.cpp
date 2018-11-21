#include "StartScene.h"
#include "Director.h"
#include "DifficultyChooseScene.h"
#include "QLabel"
#include "QPushButton"
#include "QGridLayout"
#include "QApplication"

StartScene::StartScene(Window *parent)
	: Scene(parent)
{

}

StartScene::~StartScene()
{

}

void StartScene::init()
{
	//¿Ø¼þ
	QLabel *infoLabel = new QLabel(this);
	infoLabel->setAlignment(Qt::AlignCenter);
	infoLabel->setFont(QFont(u8"Î¢ÈíÑÅºÚ", 40));
	infoLabel->setText(tr(u8"´ò  ×©  ¿é"));

	QPushButton *newGameButton = new QPushButton(this);
	newGameButton->setFont(QFont(u8"Î¢ÈíÑÅºÚ", 15));
	newGameButton->setText(tr(u8"¿ªÊ¼ÐÂÓÎÏ·"));
	connect(newGameButton, &QPushButton::clicked, this, &StartScene::newGameButtonClicked);

	QPushButton *exitButton = new QPushButton(this);
	exitButton->setFont(QFont(u8"Î¢ÈíÑÅºÚ", 15));
	exitButton->setText(tr(u8"ÍË³ö"));
	connect(exitButton, &QPushButton::clicked, QApplication::quit);

	//²¼¾Ö
	QGridLayout *layout = new QGridLayout(this);
	layout->addWidget(infoLabel, 0, 0, 10, 15);
	layout->addWidget(newGameButton, 10, 5, 3, 5);
	layout->addWidget(exitButton, 13, 5, 3, 5);
}

void StartScene::newGameButtonClicked()
{
	DifficultyChooseScene *difficultyChooseScene = new DifficultyChooseScene(Director::getInstance()->getWindow());
	Director::getInstance()->setNowScene(difficultyChooseScene);
	difficultyChooseScene->init();
	difficultyChooseScene->show();
	deleteLater();
}