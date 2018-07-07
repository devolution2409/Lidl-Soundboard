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
#include <QRegExp>              // RegExp to find out the MIMEType of the file so user can't add retarded stuff from the internet

#include "SliderSpin.h"
#include "SfxSettingsWidget.h"

/*! \class WrapperProperties
  * \brief Inherits QWidget
  *
  * WrapperProperties class deals with displaying a soundwrapper's data in order to edit them.
  * It displays a UI that allows to add local and remote files to a wrapper as well as delete them.
  * Each sound can be assigned several sound effects, main output volume, and VAC output volume.
  *
  * Removed pointers to main UI and replaced several functions and connections by lambdas in both
  * this file and soundboardmainUI.
  * \author Devolution
  * \version 1.7.0
  */
class WrapperProperties : public QWidget
{
    Q_OBJECT
public:
    /*!
     * \brief Constructor of the "Sound Entry Editor" UI.
     *
     * Will construct a blank window, to be populated, or not, by a wrapper, by calling the private default constructor
     *
     * The UI will either be blank or populated by the SoundWrapper to be edited.
     * The function will check for SoundWrapper being nullptr.
     *  If it is, nothing is to be done, the Entry Editor will created a new wrapper.
     *  If it isn't, then we opened a wrapper. Several steps are then done:
     *      Iterating through the wrapper to display the files inside it, with their SFX and volume.
     *      Disconnect the "Done!" button from its connection, and connect it
     *      to the relevant function. (SendEditedWrapper)
     * \param mainOutput The index of the main output device.
     * \param VACOutput The index of the VAC output device.
     * \param sound A pointer to the sound being edited.
     *
     */
    WrapperProperties(int mainOutput,int VACOutput,SoundWrapper * sound = nullptr,QWidget *parent = nullptr);

private:
    /*!
     * \brief Default constructor, private because we only need the populated one.
     *
     * Will construct a blank window, to be populated, or not, by a wrapper.
     * This constructor is only called by the initialization list of the public constructor.
     * \param parent A pointer to the parent (default is nullptr);
     */
    explicit WrapperProperties(QWidget *parent = nullptr);
    /*!
     * \brief Reimplemented to send a closed signal to the mainUI, so we can re-enable the widgets.
     * \param event The QCloseEvent (used internally).
     */
    void  closeEvent(QCloseEvent *event);

    CustomListWidget * _soundListDisplay; /*!< The widget that will display the sound list. (check CustomListWidget class). */
    QLabel * _soundListHint; /*!< The instructions/hints under the sound list.*/

    // Buttons to add and delete
    QPushButton *_btnAdd; /*!< The add sound button. */
    QPushButton *_btnAddFromURL;/*!< The add sound from URL button.*/
    QPushButton *_btnDelete; /*!< The delete sound button.*/
    CustomListWidgetItem *_selectedItem; /*!< A pointer to the selected item.*/

    // Our layouts
    QVBoxLayout *_vLayout; /*!< Layout. */
    QGridLayout *_gLayout; /*!< Layout.*/



    // Playbackmode
    QGroupBox    *_radioGroupBox; /*!< The groupbox for playback mode.*/
    QButtonGroup *_radioGroup; /*!< The button holding together each radio button.*/
    QHBoxLayout  *_radioLayout; /*!< The horizontal layout containing the radio buttons.*/
    QRadioButton *_radioSingleton; /*!< The Singleton radio button.*/
    QRadioButton *_radioSequential; /*!< The Sequential radio button.*/
    QRadioButton *_radioAuto; /*!< The Sequential (auto) radio button.*/
    QRadioButton *_radioCancer; /*!< The Singleton (cancer) radio button.*/

    // Hotkey
    QGroupBox   *_shortcutGroup; /*!< The groupbox for the shortcut settings.*/
    QGridLayout *_shortcutLayout; /*!< The layout of the shortcut groupbox.*/

    CustomShortcutEdit *_shortcutEdit; /*!< The specialized QKeySequenceEdit we use to get the shortcut.*/
    QKeySequence   *_shortcutSequence; /*!< The variable that will contain the sequence.*/
    QPushButton    *_shortcutResetBtn; /*!< The reset button.*/
    QLabel         *_shortcutHint;     /*!< The instructions text.*/
    QLabel         *_shortcutWarning;  /*!< The warning about using the same key twice.*/

 //   int            _shortcutVirtualKey;

    // Abort and done button
    QPushButton *_btnDone; /*!< The done button.*/
    QPushButton *_btnAbort; /*!< The abort button.*/



    LIDL::Playback  _playBackMode; /*!< The variable that holds the playbackmode.*/
    // variables to store which output device and PTT are already set (if any)
    int _mainOutput; /*!< The index of the main output device.*/
    int _VACOutput; /*!< The index of the VAC output device.*/

    /*******************************************************
     *                                                     *
     *                     VOLUME SLIDERS                  *
     *                                                     *
     *******************************************************/
    Spoiler * _sliderSpoiler; /*!< The spoiler group containing the volume sliders*/

    QGroupBox* _sliderGroup; /*!< The groupbox containing the volume sliders.*/
    QGridLayout * _sliderLayout; /*!< The layout of the slider groupbox.*/
    QSlider *_sliderMain; /*!< The main volume slider.*/
    QSpinBox    * _sliderMainSpin; /*!< The main volume spinbox.*/
    QLabel  * _sliderLabelMain; /*!< Label for the main volume.*/

    QSlider *_sliderVAC; /*!< The VAC volume slider.*/
    QSpinBox    * _sliderVACSpin; /*!< The VAC volume spinbox.*/
    QLabel  * _sliderLabelVAC; /*!< Label for the VAC volume.*/
//    QLabel  * _sliderHint; /*!< Hint.*/


    /*******************************************************
     *                                                     *
     *                    SOUND EFFECTS                    *
     *                                                     *
     *******************************************************/
    QButtonGroup *_sfxBtnGroup; /*!< */
    Spoiler *_sfxSpoiler; /*!<The spoiler group containing the SFX settings.*/
    QGridLayout *_sfxLayout; /*!< The layout of the SFX spoiler group.*/
    QTabWidget *_sfxTabWidget; /*!< Tab widget to sort all the available SFX.*/
    // DISTORTION
        QWidget   * _sfxDistortionWidget; /*!< The widget holding the distortion settings.*/
        QGridLayout * _sfxDistortionLayout; /*!< Layout of the distortion widget.*/
        QCheckBox *_sfxDistortionCheckBox; /*!< Checkbox to activate the distortion.*/
        QVector <QLabel*> _sfxDistortionLabels; /*!< Vector containing the labels.*/
        QVector <QSlider*> _sfxDistortionSliders; /*!< Vector containing the sliders.*/
        QVector <QSpinBox*> _sfxDistortionSpinboxes; /*!< Vector containg the spinbox.*/
        QPushButton * _sfxDistortionDefault; /*!< Button to reset SFX to default.*/
    // CHORUS
        SfxSettingsWidget* _chorusWidget; /*!< The widget responsible of the tab construction*/
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
     /*!
     * \brief The signal that is sent when adding a new wrapper.
     *
     * This signal is connected to a lambda function in the mainUI when the New Wrapper button is clicked.
     * \param sound The new soundwrapper to add.
     */

    void signalAddDone(SoundWrapper *sound);
    /*!
     * \brief The signal that is sent after editing an existing wrapper.
     *
     * This signal is connected to a lambda function in the mainUI when the Edit Wrapper button is clicked.
     * \param sound The edited wrapper that will replace the old one.
     */
    void signalEditDone(SoundWrapper *sound);
    /*!
     * \brief This signal is emitted whenever the Sound Entry Editor window is closed.
     */
    void closed();

public slots:
    /*!
     * \brief AddSound method.
     *
     * Connected to the add button, it will open a prompt to select a file to add to the wrapper.
     */
    void AddSound();
    /*!
     * \brief AddSoundFromUrl method.
     *
     * Connected to the Add from URL button, it will open a prompt to input a url pointing to a remote file.
     * If the file has a valid MIME type, we add it to the soundboard.
     */
    void AddSoundFromUrl();
    /*!
     * \brief AddSoundFromDrop
     *
     * This method will deal with drag and dropped files.
     * If they have the correct MIME type, it will add them.
     */
    void AddSoundFromDrop(QString);

    /*!
     * \brief ItemWasClicked deals with connecting every slider and spinbox to the selected sound.
     * \param item The QListWidgetItem that was clicked (dynamic_cast from CustomListWidgetItem).
     */
    void ItemWasClicked(QListWidgetItem* item);




    /*!
     * \brief RadioPressed sets the wrapper playback mode whenever radio is pressed.
     * \param id The ID of the pressed button.
     */
    void RadioPressed(int id);


    void SetKeySequence(QKeySequence);

    // Communicating with main ui slots: CreateWrapper and Edited wrapper
    // void CreateWrapper();


    /*!
     * \brief SetItemMainVolume Sets a sound item main volume.
     *
     * (CustomListWidgetItem isn't a Q_OBJECT and can't have slots of its own).
     */
    void SetItemMainVolume(int);
    /*!
     * \brief SetItemVACVolume Sets a sound item VAC volume.
     *
     * (CustomListWidgetItem isn't a Q_OBJECT and can't have slots of its own).
     */
    void SetItemVACVolume(int);


  //  void Test(SoundWrapper*);
};

#endif // WRAPPERPROPERTIES_H
