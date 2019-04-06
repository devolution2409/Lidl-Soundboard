#ifndef GAMESELECTOR_H
#define GAMESELECTOR_H

#include <QObject>
#include <QWidget>
#include <ui_GameSelector.h>

class GameSelector : public QWidget
{
public:
    GameSelector(QWidget *parent = nullptr);

    Ui::GameSelector * _gameSelectorUi;
};

#endif // GAMESELECTOR_H
