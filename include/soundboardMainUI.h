
#ifndef SOUNDBOARDMAINUI_H
#define SOUNDBOARDMAINUI_H

/*!
 * \file soundboardMainUI.h
 * \brief Soundboard Main UI file
 * \author Devolution
 * \version 1.7.0
 */


#include <QWidget>
#include <QVector>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QStandardItemModel>
#include <QTableView>
// QHeaderView is needed to hide the vertical headers forsenT
#include <QHeaderView>
#include <QPushButton>
#include <QGridLayout>
#include <QLabel>

#include <QComboBox>


// Property window

#include <QVector>
#include <QModelIndex>
// custom table view to reimplement focus out
#include "CustomTableView.h"
#include <QShortcut>
#include <QKeySequence>
#include <string>

#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QAudio>


//#include "Utility.h"

#define MOD_NOREPEAT    0x4000
#define MOD_ALT         0x0001
#define MOD_CONTROL     0x0002

#include <QCloseEvent>
#include "CustomShortcutEdit.h"

#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QTextStream>
#include <QDesktopServices>

//#include <QUrl>
// need for gachiBASS animation
#include <QMovie>
// mod_shift already defined for some reason
//#define MOD_SHIFT       0x0003
#define VER 1.6.1
#define VER_STRING "1.6.1"

//#include "QSimpleUpdater.h"
#include <QMainWindow>
#include <QStatusBar>
#include "CustomTableModel.h"
#include <QFontMetrics>
#include <QTextEdit>
#include <QTimer>
#include "SettingsController.h"
#include "soundwrapper.h"
#include "wrapperproperties.h"
#include <QFileInfo>
#include <deque>
#include "updater.h"

#include <QProcess>
#include <ui_guideUI.h>
#include <QDesktopWidget>
#include "OverlayWidget.h"
#include "EnumsAndStructs.h"
#include <QFontMetrics>
#include <QHeaderView>
#include <QStyleFactory>

/*! \class SoundboardMainUI
  * \brief Inherits QMainWindow.
  *
  *  Deals with displaying and intercepting shortcuts
  */
class SoundboardMainUI : public QMainWindow
{
    Q_OBJECT

private:
    // All pointers will be deleted if their parents is killed. => parenting everything to this

    // sound list
    QVector<SoundWrapper*> _sounds; /*!< SoundWrapper array*/


    QVector<QKeySequence> _keySequence; /*!< keySequence array: contains the shortcuts to the sounds (mostly for displaying).*/
    QVector<int>          _keyVirtualKey; /*!< keyVirtualKey array: contains virtual code of the shortcuts to play (to register them).*/
    // Windows Shorcut HANDLE for the sound shortcuts
    QVector<int> _winShorcutHandle; /*!< Handles to the registered shortcuts.*/


    QGridLayout *_gLayout; /*!< A grid layout: everything is inside it.*/
    // Model displayed by the viewer
    //QStandardItemModel *_model;
    CustomTableModel *_model; /*!< Custom model that contains the data being displayed.*/
    // DATA of the model:
    QVector<QList< QStandardItem* >> _data; /*!< ALL the data recieved by using GetSoundAsItem method. */
    QVector<QList< QStandardItem* >> _displayedData; /*!< Processed data according to the user settings. */

    /***************************************************
                            MENU
    ****************************************************/
    //QMenuBar *_menuBar;
    // We already have this->MenuBar() since we QMainWindow now
    QVector<QAction*> _actions; /*!< List of the actions in the MenuBar.*/
    /***************************************************
                       NAME DISPLAYING
    ****************************************************/
    // QGroupBox *_nameGroupBox;
    // QLineEdit *_nameEdit;

    // view
  //  QTableView *resultView;
    CustomTableView *resultView; /*!< Custom table view (inherists from QTableView) to display the data in the model.*/

    // buttons
    QPushButton  * _btnAdd; /*!< Add SoundWrapper button.*/
    QPushButton  * _btnDelete; /*!< Delete SoundWrapper button.*/
    QPushButton  * _btnEdit; /*!< Edit SoundWrapper button/*/
    QPushButton  * _btnPlay; /*!< Play the SoundWrapper according to its playback mode button.*/
    QPushButton  * _btnStop; /*!< Delete SoundWrapper button.*/

    // Device selection
    QLabel       * _label1; /*!<Label for the main output device list.*/
    QComboBox  * _deviceListOutput; /*!<Contains the actual output device list.*/

    QLabel       * _label2; /*!<Label for the VAC output device list.*/
    QComboBox  * _deviceListVAC; /*!<Contains the actual VAC output device list.*/

    // Open windows settings button
    QLabel      * _label3; /*!<Label for microphone injection.*/
    QPushButton *_btnMicInjection;/*!<Button that opens the windows sound settings.*/

    // Auto-hold ptt
    QLabel      * _label4; /*!<Label for the Auto-Hold PTT feature.*/
    CustomShortcutEdit *_shortcutEditPTT; /*!<CustomShortcutEdit field to set the microphone injection hotkey.*/
    QPushButton *_btnClearPTT; /*!<Button to unregister and clear the auto-PTT hotkey.*/
    // Stop button
    QLabel      * _label5; /*!<Label for the stop all feature.*/
    CustomShortcutEdit *_shortcutEditStop; /*!<CustomShortcutEdit field to set the stop all hotkey.*/
    QPushButton *_btnClearStop; /*!<Button to unregister and clear the Stop All hotkey.*/
    // The property window
    WrapperProperties *_propertiesWindow; /*!<Window that handle wrapper edition.*/


    /**
     * \brief This function fetches the devices list, and sets up the given combo box.
     *
     * \param comboBox Pointer to the combo box that will display devices.
     * \param mode Deprecated. Defaults to QAudio::AudioOutput.
     */
    void fetchDeviceList(QComboBox* comboBox,QAudio::Mode mode = QAudio::AudioOutput);
    /**
     * \brief This function adds all the entries of the QMenuBar.
     */
    void setUpMenu();
    /**
     * \brief This function adds all the entries of the QMenuBar.(Reimplementation of the QMainWindow close event).
     * \param event Pointer to the event (automatically called).
     */
    void closeEvent (QCloseEvent *event);



   // bool checkFileExistence(QString fileName);

    // Will return true if update is available
    // bool IsUpdateAvailable(); //

    // keep track of selection
    int lastSelectedRow; /*!<Keeps tracks of last selected rows. (Could probably use a method of QTableView.*/


    QTextEdit * _statusEdit; /*!<Main widget of the QStatusBar so we have more control over it.*/
    /***************************************************
                            SAVE
    ****************************************************/
    QString _saveName; /*!<Keeps tracks the .lidljson file being read/written.*/

    /**
     * \brief This function Open a saved soundboard and add all the entries in the CustomTableView.
     * \param fileName String containing the file path.
     */
    void Open(QString fileName);

    /* LIDL::SettingsController is a singleton and calling
     * GetInstance() will either create it or give a pointer
     * towards the already existing instance
     */

    //LIDL::SettingsController* settingsObj;

    //void ScrollStatusText(int howMuch);
    QMenu * _openRecentMenu; /*!<QMenu containing the recently opened lidljson*/

    /**
     * \brief This function construct and display the first-time user dialog.
     */
    void HelpShowFirstUserDialog();

    bool _updateScheduled; /*!<Boolean to know if the user wanna update the soundboard after quitting.*/

    QWidget * _guideWidget; /*!<Widget for the guide/app tour.*/
    Ui::Guide *_guideUI; /*!<UI for the guide/app tour.*/

    /**
     * \brief This function deals with editing the data that will be displayed according to user settings. (Read: kind of delegate).
     */
    void refreshView();

public:
    explicit SoundboardMainUI(QWidget *parent = nullptr);

signals:
    /**
     * \brief Signal to be emitted whenever a file is saved or opened. So that it appears in the recent menu.
     */
    void lidlJsonDetected(QFileInfo); // forsenBee (to deal with recent saved or opened files)*

    // We save soundboard state once we open a file or when we save it,
    // Than we can compare it when closing it to tell the user it hasn't been saved

    /**
     * \brief Signal to be emitted whenever a file is saved or opened. So that its save can be saved and compared to the "ending" state, so modifications can be detected.
     */
    void SaveSoundboardState();

    void OnConstructionDone();

public slots:
    //This slot will allow us to add a sound, opens a file explorer dialogue
    // void addSoundDialog(); replace by a lambda

    /**
     * \brief This function add a soundwrapper to the main UI display, and adds/modify the entries in the private members so that the said wrapper can be played.
     * \param modifiedSound The added sounded/modified sound.
     * \param whereToInsert The spot where to insert the sound (only used when it's a sound being edited. Else if it's -1 we insert it at the bottom.
     * \param generationMode Should the shortcut be registered or not. Useful when adding a bunch of soundswrapper (opening a soundboard).
     */
    void addSound(SoundWrapper * modifiedSound, int whereToInsert = -1, LIDL::Shortcut generationMode = LIDL::Shortcut::GENERATE);

    /**
     * \brief This function will call addSound with the modified sound and the correct whereToInsert param.
     * \param modifiedSound The modified sound.
     */
    void soundModified(SoundWrapper * modifiedSound);

    // Slots for where user click or double click a cell
    void onCellClicked(QModelIndex index);
    void onCellDoubleClicked(QModelIndex index);
    void deleteSound();
    void editSoundDialog();

    void enableButtons();
    void disableButtons();
    void winHotKeyPressed(int);
    void GenerateGlobalShortcuts();

    void resetPushToTalkEdit();
    void setStopShortcut(int);
    void resetStopAllEdit();


    void openAudioSettings();

    //Soundboard as JSON
    QJsonObject * GenerateSaveFile();
    //Json for saving the json path forsenT
    //void SaveSettings();

    // save slot
    void Save();
    // Save as slot
    void SaveAs();

    // open slot
    void OpenSlot();
    // Clear the soundboard
    void ClearAll();
    // EXP soundboard compatibility
    void OpenEXPSounboard();
    // help guide slot
    void HelpGuide();
    // help report bug
    void HelpReportBug();
    // About
    void HelpAbout();

    void StatusErrorUnexistant();
    void StatusPlaying(QString);
    void StatusErrorPlaying(QString);

    //void HelpCheckForUpdate();


    void ToolClearShortcut();

    void DealDragAndDrop(int);
    void SwapWrappers(int firstRow, int secondRow);

    void SetStatusTextEditText(QString);

    void PostConstruction();
    //void ErrorOnPlaying(QString);
    void SetUpRecentMenu();

    void CheckForUpdates();
    void resizeEvent ( QResizeEvent * event );

};




#endif // SOUNDBOARDMAINUI_H
