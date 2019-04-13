#ifndef GAMESELECTOR_H
#define GAMESELECTOR_H

#include <QObject>
#include <QWidget>
#include <ui_GameSelector.h>
#include "SettingsController.h"
#include "ProfileEdit.h"
#include "soundwrapper.h"
#include <QVector>

class GameSelector : public QWidget
{
    Q_OBJECT
public:

    GameSelector(QWidget *parent = nullptr);

    Ui::GameSelector * _gameSelectorUi;
    ProfileEdit * _profileEdit;
signals:
     void RefreshWrappers(QVector<std::shared_ptr<SoundWrapper>>);

public slots:
     /*!
     * \brief RefreshProfiles
     *
     * Will refresh the combo box and fill it with the available profiles.
     * Will also re-load the active profile.
     * However it will prevent the combobox from emitting event to avoid infinite recursion.
     * It will MANUALLY call the function to AddSounds in the UI.
     * See ProfileController.h for more details
     */
    void RefreshProfiles();

};

#endif // GAMESELECTOR_H
