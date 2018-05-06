/*  Class containing the main window for the application:
 *      Members:
 *          Attributes:
 *             Type                     Name                Description
 *
 *              QVector<SoundWrapper>    _sounds            array of SoundWrapper objects:
 *              QVBoxLayout              _vLayout           main vertical layout, that contains the display and the grid layout
 *              QGridLayout              _gLayout           grid layout
 *              QStandardItem_model       __model             _model for the MVC architecture
 *              QTableView               _resultView        viewer in the MVC architecture
 *              QPushButton              _btnXYZ            add delete edit play stop boutons
 *              QLabel                   _labelX            labels
 *              QComboBox                _deviceListOutput  Combo box to display the Output device
 *              QComboBox                _deviceListVAC                                 VAC device
 *              QComboBox                _deviceListInjector                     microphone device
 *          Methods:
 *              void fetchDeviceList(QComboBox*,QAudio::Mode );
 *                  Fetch the device list (input or ouput mode) on the computer, and display them in the combo box
 *
 * TODO: replace int _playMode by an enumeration
 */



#ifndef SOUNDBOARDMAINUI_H
#define SOUNDBOARDMAINUI_H

#include <QWidget>
#include "soundwrapper.h"
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
//#include <QListWidget>
#include <QComboBox>

// Media management
#include <QAudioDeviceInfo>
// Property window
#include "wrapperproperties.h"
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

//#include <QInputMethod>
//#include <QLocale>;
#include "Utility.h"

#define MOD_NOREPEAT    0x4000
#define MOD_ALT         0x0001
#define MOD_CONTROL     0x0002

#include <QCloseEvent>
// mod_shift already defined for some reason
//#define MOD_SHIFT       0x0003


class SoundboardMainUI : public QWidget
{
    Q_OBJECT

private:
    // All pointers will be deleted if their parents is killed. => parenting everything to this
    // sound list
    QVector<SoundWrapper*> _sounds;
    // shorcut list
    QVector<QKeySequence> _keySequence;
    // Windows Shorcut HANDLE
    QVector<int> _winShorcutHandle;

    // vertical layout
    QVBoxLayout *vLayout;
    // grid layout for btns
    QGridLayout *_gLayout;
    // Model displayed by the viewer
    QStandardItemModel *_model;
    // DATA of the model:
    QVector<QList< QStandardItem* >> _data;

    QMenuBar *_menuBar;
    QVector<QAction*> _actions;

    // view
  //  QTableView *resultView;
    CustomTableView *resultView;

    // buttons
    QPushButton  * _btnAdd;
    QPushButton  * _btnDelete;
    QPushButton  * _btnEdit;
    QPushButton  * _btnPlay;
    QPushButton  * _btnStop;

    // Device selection
    QLabel       * _label1;
    QComboBox  * _deviceListOutput;

    QLabel       * _label2;
    QComboBox  * _deviceListVAC;

    QLabel      * _label3;
    QComboBox   * _deviceListInjector;

    // The property window
    WrapperProperties *_propertiesWindow;



    void fetchDeviceList(QComboBox*,QAudio::Mode);
    void setUpMenu();
    void closeEvent (QCloseEvent *event);
    // keep track of selection
    int lastSelectedRow;



public:
    explicit SoundboardMainUI(QWidget *parent = nullptr);
signals:

public slots:
     //This slot will allow us to add a sound, opens a file explorer dialogue
      void addSoundDialog();
      void soundAdded(SoundWrapper * modifiedSound, int whereToInsert = -1);
      void soundModified(SoundWrapper * modifiedSound);
      void onCellClicked(QModelIndex index);
      void deleteSound();
      void editSoundDialog();

      void enableButtons();
      void disableButtons();
      void winHotKeyPressed(int);
      void GenerateGlobalShortcuts();




};

#endif // SOUNDBOARDMAINUI_H
