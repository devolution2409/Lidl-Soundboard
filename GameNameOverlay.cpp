#include "GameNameOverlay.h"

GameNameOverlay::GameNameOverlay(QWidget *parent) : QWidget(parent)
{
    QGridLayout *layout = new QGridLayout(this);
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::SplashScreen);
    QPixmap image(":/icon/resources/forsenAim.png");

    QLabel *imageLabel = new QLabel();

    imageLabel->setFixedSize(50,50);
    imageLabel->setPixmap(image);
    imageLabel->setScaledContents(true);
    this->setFixedSize(200,100);
    layout->addWidget(imageLabel,0,0,1,1);
    QLabel * blah = new QLabel("Switched to Profile:");
    layout->addWidget(blah,0,1,2,2);

    _labelGame = new QLabel("Playerunknown's Battlegrounds");
    layout->addWidget(_labelGame,3,0,2,3);
    layout->setSpacing(0);
}

GameNameOverlay::GameNameOverlay(QString text,QWidget *parent) : GameNameOverlay(parent)
{
         _labelGame->setText("Loaded'" + text + "' profile !");

}

void GameNameOverlay::SetText(QString text)
{
    _labelGame->setText(text);
}

void GameNameOverlay::FadeOut(int duration)
{
    Q_UNUSED(duration);
    // find a way to fade in or animate this shit :)
    this->close();
}
