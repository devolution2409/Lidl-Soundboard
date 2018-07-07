
#ifndef SOUNDBOARDMAINUI_H
#define SOUNDBOARDMAINUI_H

/*!
 * \file soundboardMainUI.h
 * \brief Soundboard Main UI file
 *
 *  Replaced by lambdas expression in 1.7.0 update:
 *      void onCellClicked(QModelIndex index);
 *      void onCellDoubleClicked(QModelIndex index);
 *      void deleteSound();
 *      void enableButtons();
 *      void disableButtons();
 *      void resetPushToTalkEdit();
 *      void addSoundDialog();
 *      void resetStopAllEdit();
 *      void openAudioSettings();
 *      void HelpReportBug();
 *      void StatusErrorUnexistant();
 *      void StatusPlaying(QString);
 *      void StatusErrorPlaying(QString);
 *      void soundModified(SoundWrapper * modifiedSound);
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
#define VER 1.7.0
#define VER_STRING "1.7.0"

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



#include <QRegExp>
#include <QMimeData>
#include <QMimeDatabase>
#include <QProgressBar>
#include <ui_loadingJson.h>

/*! \class SoundboardMainUI
  * \brief Inherits QMainWindow.
  *
  *  Deals with displaying sounds (main UUI) and intercepting shortcuts.
  */
class SoundboardMainUI : public QMainWindow
{
    Q_OBJECT

private:
    // All pointers will be deleted if their parents is killed. => parenting everything to this

    QVector<SoundWrapper*> _sounds; /*!< SoundWrapper array: contains the sounds*/


    QVector<QKeySequence> _keySequence; /*!< keySequence array: contains the shortcuts to the sounds (mostly for displaying).*/
    QVector<int>          _keyVirtualKey; /*!< keyVirtualKey array: contains virtual code of the shortcuts to play (to register them).*/
    // Windows Shorcut HANDLE for the sound shortcuts
    QVector<int> _winShorcutHandle; /*!< Handles to the registered shortcuts.*/


    QGridLayout *_gLayout; /*!< A grid layout: everything is inside it.*/

    CustomTableModel *_model; /*!< Custom model that contains the data being displayed.*/

    QVector<QList< QStandardItem* >> _data; /*!< ALL the data recieved by using GetSoundAsItem method. */
    QVector<QList< QStandardItem* >> _displayedData; /*!< Processed data according to the user settings. */

    /***************************************************
                            MENU
    ****************************************************/
    //QMenuBar *_menuBar;
    // We already have this->MenuBar() since we QMainWindow now
    QVector<QAction*> _actions; /*!< List of the actions in the MenuBar.*/
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

    //void ScrollStatusText(int howMuch);
    QMenu * _openRecentMenu; /*!<QMenu containing the recently opened lidljson*/

    /**
     * \brief This function construct and display the first-time user dialog.
     */
    void HelpShowFirstUserDialog();

    bool _updateScheduled; /*!<Boolean to know if the user wanna update the soundboard after quitting.*/

    QWidget * _guideWidget; /*!<Widget for the guide/app tour.*/
    Ui::Guide *_guideUI; /*!<UI for the guide/app tour.*/

    /*!
     * \brief This function deals with editing the data that will be displayed according to user settings. (Read: kind of delegate).
     */
    void refreshView();

public:
    explicit SoundboardMainUI(QWidget *parent = nullptr);

signals:
    /*!
     * \brief Signal to be emitted whenever a file is saved or opened. So that it appears in the recent menu.
     */
    void lidlJsonDetected(QFileInfo); // forsenBee (to deal with recent saved or opened files)*

    // We save soundboard state once we open a file or when we save it,
    // Than we can compare it when closing it to tell the user it hasn't been saved

    /*!
     * \brief Signal to be emitted whenever a file is saved or opened. So that its save can be saved and compared to the "ending" state, so modifications can be detected.
     */
    void SaveSoundboardState();

    void OnConstructionDone();

public slots:
    /*!
     * \brief This function add a soundwrapper to the main UI display, and adds/modify the entries in the private members so that the said wrapper can be played.
     * \param modifiedSound The added sounded/modified sound.
     * \param whereToInsert The spot where to insert the sound (only used when it's a sound being edited. Else if it's -1 we insert it at the bottom.
     * \param generationMode Should the shortcut be registered or not. Useful when adding a bunch of soundswrapper (opening a soundboard).
     */
    void addSound(SoundWrapper * modifiedSound, int whereToInsert = -1, LIDL::Shortcut generationMode = LIDL::Shortcut::GENERATE);


    /*!
     * \brief This function opens the Sound Entry Editor to open the selected sound.
     * It is used by both the EDIT button and the DoubleClicked signal in the view.
     */
    void editSoundDialog();
    /*!
     * \brief Function to deal with Shortcut being pressed.
     *
     * This function will check if the shortcut being processed is the stop key
     * If it is, it tells the controller to un-hold the ptt and stops all running sounds.
     * If it is not, it plays the sound associated with the shortcut.
     *
     * This function is called by the event loop declared in main.cpp.
     * The shortcut arrays and the soundswrapper array uses the same index.
     * \param handle The index of the shortcut being played.
     */
    void winHotKeyPressed(int handle);


    /*!
     * \brief Function to register all the Shortcuts being used by the LIDL Soundboard.
     *
     * It will unregister all existing Shortcuts, and register them again.
     */
    void GenerateGlobalShortcuts();

    /*!
     * \brief Function to register the Stop ALL Shortcut.
     * It is used by the associated CustomShortcutEdit and the Open soundboard function.
     */
    void setStopShortcut(int);






    /*!
     * \brief Function to generate the .lidljson save file.
     *
     * It is used to save the active soundboard.
     * It is also used to detect any modification by comparing the QJsonObject store inside the settings controller with a freshly generated one.
     *
     * \return QJSonObject* (pointer) containing the data.
     */
    QJsonObject * GenerateSaveFile();

    /*!
     * \brief Function save the active lidljson file. If no file is already set, it will open the Save As prompt.
     */
    void Save();

    /*!
     * \brief This function Opens a Save As prompt, and save the active soundboard as a .lidljson file.
     */
    void SaveAs();

    /*!
     * \brief This function Opens a Open prompt, and opens .lidljson file.
     *
     * It parses the Json contained the file, and add all the settings and wrappers found inside it.
     */
    void OpenSlot();

    /*!
     * \brief This function clears the soundboard, as if it was just opened without any file.
     *
     * Call a function from the controller settings to prompt for save, clears the soundwrapper array, unregisters the shortcuts and clear their array, clears the model, clears the data, clears the devices lists, and finally refreshes the view.
     */
    void ClearAll();

    /*!
     * \brief This function allows the opening of .json files written by EXP Soundboard
     */
    void OpenEXPSounboard();

    /*!
     * \brief This function will open the application tour guide.
     *
     * Adds a widget to the right of the view, guiding user through every element.
     */
    void HelpGuide();


    /*!
     * \brief This function will open the about window.
     *
     * Shows some text about the soundboard, version number, compilation time and compiler, and link to git.
     */
    void HelpAbout();







    /*!
     * \brief This function will clear all the shortcuts in the soundboard.
     *
     * Will unregister all shortcuts. Clears the _data array. Replaced the wrappers in _sounds array by freshly created one without shortcuts. It will alos update the view in the process.
     */
    void ToolClearShortcut();
    /*!
     * \brief This function deals with drag and drop in the view, or at least, attempts to. I'm not sure it works correctly forsenD
     *
     * Will unregister all shortcuts. Clears the _data array. Replaced the wrappers in _sounds array by freshly created one without shortcuts. It will alos update the view in the process.
     */
    void DealDragAndDrop(int);

    /*!
     * \brief This function swaps two soundwrappers in the view.
     *
     * Will unregister all shortcuts. Clears the _data array. Replaced the wrappers in _sounds array by freshly created one without shortcuts. It will alos update the view in the process.
     */
    void SwapWrappers(int firstRow, int secondRow);



    /*!
     * \brief This function set the text of the permanent widget in the status bar.
     *
     * Will probably be removed at some point.
     */
    void SetStatusTextEditText(QString);

    void PostConstruction();
    //void ErrorOnPlaying(QString);


    /*!
     * \brief This function sets up the recent menu.
     *
     * Just add the necessaries entry in the recent menu.
     */
    void SetUpRecentMenu();
    /*!
     *  \brief This function check for updates.
     *
     * Check for updates by launching the maintenance tool in the background. If no data => no updates.
     * If data => Ask for user if he wants to update now or later.
     * The updates is stored on a github repository.
     */
    void CheckForUpdates();

    /*!
     *  \brief This function is called whenever the window is resized, and just resizerowstocontent(). And call refreshView()
     */
    void resizeEvent ( QResizeEvent * event );

    /*!
     *  \brief This function checks for the MIME type of files being dragged onto the sounboard.
     *
     * If the mime types of the files being dropped are all contained in the settings controller mime types, we accept the event.
     * If one of them isn't, we refuse.
     * If it is a .lidljson file, we accept aswell.
     * Refusing an event will show the 🚫 icon instead of the + one.
     */
    void dragEnterEvent(QDragEnterEvent *e);

    /*!
     *  \brief This function checks for the MIME type of files being dragged onto the sounboard.
     *
     *  This function deals with accepted events from the dragEnterEvent.
     *  If it is sound file(s) it will add them to the soundboard based on the settings found inside the controller.
     *  If it is a lidljson file, it will open it.
     */
    void dropEvent(QDropEvent *e);

};




#endif // SOUNDBOARDMAINUI_H
