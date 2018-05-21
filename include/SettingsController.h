#ifndef INCLUDESETTINGSCONTROLLER_H
#define INCLUDESETTINGSCONTROLLER_H
#include <QString>
#include <QVector>
#include <QObject>
#include <QWidget>

#include <ui_Settings.h>
#include <QFileDialog>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QFile>
// Class handling the settings
// we make it a singleton so we are gucci
namespace LIDL{


class SettingsController : public QObject
{
    Q_OBJECT
public:

    static SettingsController * GetInstance();

    // getters
    int GetDefaultMainVolume();
    int GetDefaultVacVolume();
    // Public methods
    void OpenSettings();
    // Can still use constructor to set default valuues
    // Because it should be called before any other methods
    //void SetDefaultValues();
    void SaveSettings();


private:
    // Singleton class
    SettingsController();
    // the pointer will exist even if class isn't instancied yet
    // but we need to initalize it in the .cpp with a weird syntax
    static SettingsController * self;


    // Default folder to open sounboard (lidljson) files
    QString defaultSoundboardFolder;
    // Default folder to add sound
    QString defaultSoundsFolder;

    // Number of files to show in recent files (and to be saved)
    int recentFileCount;


    // Default volume for the add sound dialog (or the open exp soundboard)
    int defaultMainVolume;
    int defaultVacVolume;

    Ui::Settings * ui;

public slots:
    void SetDefaultMainVolume(int);
    void SetDefaultVacVolume(int);
    void SetRecentFileCount(int);
    void ShowSettingsWindow();

    // slots for button in the ui
    void buttonBrowseLidl();
    void buttonBrowseSound();

};
}
#endif // INCLUDESETTINGSCONTROLLER_H
