#ifndef GAMENAMEOVERLAY_H
#define GAMENAMEOVERLAY_H

#include <QObject>
#include <QWidget>
#include <QLabel>
#include <QPixmap>
#include <QGridLayout>
#include <QString>
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>

class GameNameOverlay : public QWidget
{
    Q_OBJECT
public:
    explicit GameNameOverlay(QWidget *parent = nullptr);
    GameNameOverlay(QString text,QWidget *parent = nullptr);
private:

    QLabel * _labelGame;

signals:

public slots:
    void SetText(QString text);
    void FadeOut(int duration);
};

#endif // GAMENAMEOVERLAY_H
