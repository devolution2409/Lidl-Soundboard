#ifndef WRAPPERPROPERTIES_H
#define WRAPPERPROPERTIES_H

//#include <QDebug>


#include <QWidget>
#include <QPushButton>
#include <QListWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include "soundwrapper.h"
#include <QLabel>
#include <QLineEdit>
#include <QRadioButton>
#include <QGroupBox>
#include <QCloseEvent>
#include <QListWidgetItem>
#include <QFileDialog>
//#include <QTreeWidget>
//#include <QTreeWidgetItem>
#include <QString>

#include <QButtonGroup>
#include <QKeySequence>
#include <QKeyEvent>
#include <QSet>
#include <QTimer>
#include <QPoint>
#include "soundwrapper.h"

// The class to save me from going insane
#include <QKeySequenceEdit>
#include "CustomShortcutEdit.h"
#include <QMessageBox>
#include "CustomListWidget.h"
#include "EnumsAndStructs.h"
#include <QSlider>
// #include "QFancySlider"
#include <CustomListWidgetItem.h>
#include <QSpinBox>
#include "SettingsController.h"
#include "Spoiler.h"            // UI:SFX
#include <QCheckBox>            // UI:SFX
#include <QTabWidget>           // UI:SFX Tab widget
#include "CustomSoundFile.h"    // LIDL::SoundFile methods
#include <QComboBox>            // UI:SFX:Chorus
#include <QScrollArea>          // UI:SFX
#include <QInputDialog>         // Asking user for URL

#include <QTcpSocket>           // TCP Socket to check if file exist in HTTP
#include <QSslSocket>           // SSL Socket to check if file exist in HTTPS
#include <QRegExp>              // RegExp to find out the MIMEType of the file so user can't retarded stuff
class WrapperProperties : public QWidget
{
    Q_OBJECT
public:
    explicit WrapperProperties(QWidget *parent = nullptr);
    // One constructor will deal
    // for both Add sound dialog and Edit sound dialog
    WrapperProperties(int mainOutput,int VACOutput,int pttScanCode,int pttVirtualKey,SoundWrapper * sound = nullptr,QWidget *parent = nullptr);
    // WrapperProperties(int mainOutput,int VACOutput,int pttScanCode,int pttVirtualKey,QWidget *parent = nullptr);
private:
    // Pointer to the main window
    QWidget     * _mainWidget;
    // Display for sounds
    // QListWidget *_soundListDisplay;
    //  QGroupBox        * _soundListGroup;
    CustomListWidget * _soundListDisplay;
    // QVBoxLayout      * _soundLayout;

    QLabel * _soundListHint;

    // Buttons to add and delete
    QPushButton *_btnAdd;
    QPushButton *_btnAddFromURL;
    QPushButton *_btnDelete;
    CustomListWidgetItem *_selectedItem;

    // Our layouts
    QVBoxLayout *_vLayout;
    QGridLayout *_gLayout;



    // Playbackmode
    QGroupBox    *_radioGroupBox;
    QButtonGroup *_radioGroup;
    QHBoxLayout  *_radioLayout;
    QRadioButton *_radioSingleton;
    QRadioButton *_radioSequential;
    QRadioButton *_radioAuto;
    QRadioButton *_radioCancer;

    QLabel       *_radioToolTip;
    // Hotkey
    QGroupBox   *_shortcutGroup;
    QGridLayout *_shortcutLayout;

    CustomShortcutEdit *_shortcutEdit;
    QKeySequence   *_shortcutSequence;
    QPushButton    *_shortcutResetBtn;
    QLabel         *_shortcutHint;
    QLabel         *_shortcutWarning;

 //   int            _shortcutVirtualKey;

    // Abort and done button
    QPushButton *_btnDone;
    QPushButton *_btnAbort;

    //reimplementation de l'event close
    void  closeEvent(QCloseEvent *event);
    LIDL::Playback  _playBackMode;
    // variables to store which output device and PTT are already set (if any)
    int _mainOutput;
    int _VACOutput;
    int _microphone;
    int _pttScanCode;
    int _pttVirtualKey;

    /*******************************************************
     *                                                     *
     *                     VOLUME SLIDERS                  *
     *                                                     *
     *******************************************************/
    Spoiler * _sliderSpoiler;

    QGroupBox* _sliderGroup;
    QGridLayout * _sliderLayout;
    QSpinBox    * _sliderMainSpin;
    QSpinBox    * _sliderVACSpin;
    QLabel  * _sliderLabelMain;
    QLabel  * _sliderLabelVAC;
    QLabel  * _sliderHint;
    QSlider *_sliderMain;
    QSlider *_sliderVAC;
    /*******************************************************
     *                                                     *
     *                    SOUND EFFECTS                    *
     *                                                     *
     *******************************************************/
    QButtonGroup *_sfxBtnGroup;
    Spoiler *_sfxSpoiler;
    QGridLayout *_sfxLayout;
    QTabWidget *_sfxTabWidget;
    // DISTORTION
        QWidget   * _sfxDistortionWidget;
        QGridLayout * _sfxDistortionLayout;
        QCheckBox *_sfxDistortionCheckBox;
        QVector <QLabel*> _sfxDistortionLabels;
        QVector <QSlider*> _sfxDistortionSliders;
        QVector <QSpinBox*> _sfxDistortionSpinboxes;
        QPushButton * _sfxDistortionDefault;
    // CHORUS
        QWidget * _sfxChorusWidget;
        QGridLayout * _sfxChorusLayout;
        QCheckBox *_sfxChorusCheckBox;
        QVector <QLabel*> _sfxChorusLabels;
        QVector <QSlider*> _sfxChorusSliders;
        QVector <QSpinBox*> _sfxChorusSpinboxes;
        QVector <QComboBox*> _sfxChorusBox;
        QVector <QPushButton*> _sfxChorusButton;
        QScrollArea *  _sfxChorusScrollArea;

//    QCheckBox *_sfxChorusEnabled;
//    QCheckBox *_sfxCompressorEnabled;
//    QCheckBox *_sfxEchoEnabled;
//    QCheckBox *_sfxFlangerEnabled;
//    QCheckBox *_sfxGargleEnabled;
//    QCheckBox *_sfxReverbEnabled;

/*
 *
BASS_FX_DX8_CHORUS	DX8 Chorus. Use BASS_DX8_CHORUS structure to set/get parameters.
BASS_FX_DX8_COMPRESSOR	DX8 Compression. Use BASS_DX8_COMPRESSOR structure to set/get parameters.
BASS_FX_DX8_DISTORTION	DX8 Distortion. Use BASS_DX8_DISTORTION structure to set/get parameters.
BASS_FX_DX8_ECHO	DX8 Echo. Use BASS_DX8_ECHO structure to set/get parameters.
BASS_FX_DX8_FLANGER	DX8 Flanger. Use BASS_DX8_FLANGER structure to set/get parameters.
BASS_FX_DX8_GARGLE	DX8 Gargle. Use BASS_DX8_GARGLE structure to set/get parameters.
BASS_FX_DX8_I3DL2REVERB	DX8 I3DL2 (Interactive 3D Audio Level 2) reverb. Use BASS_DX8_I3DL2REVERB structure to set/get parameters.
BASS_FX_DX8_PARAMEQ	DX8 Parametric equalizer. Use BASS_DX8_PARAMEQ structure to set/get parameters.
BASS_FX_DX8_REVERB */


// TODO ajouter volume
signals:
    void signalAddDone(SoundWrapper *sound);
    void signalEditDone(SoundWrapper *sound);

public slots:
    // Qlist Widget slots
    void AddSound();
    void AddSoundFromUrl();
    void AddSoundFromDrop(QString);
    void ItemWasClicked(QListWidgetItem* item);
    void DeleteSelectedSound(); //will call DeleteSound()


    // Radio Slots
    void RadioPressed(int id);
    // Btn slots
    void AbortMission();
    // Shorcut slots
    void SetKeySequence(QKeySequence);
    void ShortcutReset();
    void editingStarted();
    void editingDone();
    // Communicating with main ui slots: CreateWrapper and Edited wrapper
    void CreateWrapper();
    void SendEditedWrapper();

    // Slots for the item as the items aren't Q_OBJECT they can't have slots of their own forsenT
    void SetItemMainVolume(int);
    void SetItemVACVolume(int);


  //  void Test(SoundWrapper*);
};

#endif // WRAPPERPROPERTIES_H
