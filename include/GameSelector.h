#ifndef GAMESELECTOR_H
#define GAMESELECTOR_H

#include <QObject>
#include <QWidget>
#include <ui_GameSelector.h>
#include "SettingsController.h"
#include "ProfileEdit.h"


class GameSelector : public QWidget
{
public:
    GameSelector(QWidget *parent = nullptr);

    Ui::GameSelector * _gameSelectorUi;
    ProfileEdit * _profileEdit;

public slots:
    void RefreshProfiles();

};

#endif // GAMESELECTOR_H
