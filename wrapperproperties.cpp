#include "wrapperproperties.h"




WrapperProperties::WrapperProperties(QWidget *parent) //: QWidget(parent)
{


    this->setFocusPolicy(Qt::StrongFocus);

    this->setFixedWidth(380);

    this->_mainWidget = parent;
    // Ajout d'un layout vertical pour afficher les sons
    this->setWindowTitle("Lidl Sounboard Entry Editor");
    this->setWindowIcon(QIcon(":/icon/resources/forsenAim.png"));
    _vLayout = new QVBoxLayout(this);
    // Adding viewer to view sound list
    _soundListDisplay = new QListWidget(this);

   // _itemAdd = new QListWidgetItem("Double click to add a sound");
    // _soundListDisplay->addItem(_itemAdd);
    //Enabling drag&drop reordering
    _soundListDisplay->setDragDropMode(QAbstractItemView::InternalMove);

    _vLayout->addWidget(_soundListDisplay);

    _gLayout = new QGridLayout;
    // Add and delete
    _vLayout->addLayout(_gLayout);
    _btnAdd = new QPushButton("Add");
    _btnDelete= new QPushButton("Delete");
    _btnDelete->setEnabled(false);
    _gLayout->addWidget(_btnAdd,0,0,1,2);
    _gLayout->addWidget(_btnDelete,0,2,1,2);



    //Radiobox
    _radioGroupBox      = new QGroupBox("Playback mode",this);
    _radioGroup     = new QButtonGroup(this);
    _radioLayout     = new QHBoxLayout(_radioGroupBox);

     // Creating buttons
     _radioSequential = new QRadioButton("Sequential",this);
     _radioAuto       = new QRadioButton("Sequential (auto)",this);
     _radioSingleton  = new QRadioButton("Singleton",this);
     // Tooltip hint
     _radioToolTip    = new QLabel("       ❔");
     _radioToolTip->setToolTip("Singleton: A Single Sound\nSequential: A Sound Collection. Next item will play after pressing play or the shortcut.\nSequential(Auto): Same as sequential, but automated.");

     // setting 1 has default value for playblack
     _playBackMode    = 1;

    // Adding them to layout, and setting singleton checked by default
    _radioLayout->addWidget(_radioSingleton);
    _radioSingleton->setChecked(true);
    _radioLayout->addWidget(_radioSequential);
    _radioLayout->addWidget(_radioAuto);
    _radioLayout->addWidget(_radioToolTip);
    // Adding them to the group
    _radioGroup->addButton(_radioSingleton,1);
    _radioGroup->addButton(_radioSequential,2);
    _radioGroup->addButton(_radioAuto,3);

     // Displaying widget
    _gLayout->addWidget(_radioGroupBox,1,0,1,4);



    //_gLayout->addWidget(_btnAdd,2,0,1,4);
    //_gLayout->addWidget(_btnDelete,2,1,1,4);


    /*******************************************************
     *                                                     *
     *                     HOTKEY EDITION                  *
     *                                                     *
     *******************************************************/
    _shortcutGroup      = new QGroupBox("Shortcut",this);
    _shortcutLayout     = new QGridLayout(_shortcutGroup);
    _shortcutSequence   = new QKeySequence;
    //_shortcutEdit       = new QKeySequenceEdit();
    _shortcutEdit       = new CustomShortcutEdit();
    _shortcutResetBtn   = new QPushButton("Reset");
     _shortcutHint      = new QLabel("Set a shortcut to play the sound.\nWait one second after last released key to apply.");
    _shortcutResetBtn->setFixedHeight(22);
    _shortcutLayout->addWidget(_shortcutEdit,1,0,1,2);
    _shortcutLayout->addWidget(_shortcutResetBtn,1,2,1,1);
    _shortcutLayout->addWidget(_shortcutHint,0,0,1,3);
    _shortcutWarning = new QLabel("⚠️ Don't use the same shortcut twice, it will not work.");
    _shortcutLayout->addWidget(_shortcutWarning,2,0,1,3);

    _gLayout->addWidget(_shortcutGroup,2,0,1,4);


    //Done button
    _btnDone = new QPushButton("Done!");
    _gLayout->addWidget(_btnDone,3,0,1,3);
    //Abort button
    _btnAbort = new QPushButton("Abort mission");
    _gLayout->addWidget(_btnAbort,3,3,1,1);

    /*******************************************************
     *                                                     *
     *                     CONNECTIONS                     *
     *                                                     *
     *******************************************************/

    /*******************BUTTON ADD AND DELETE***************/

    // Button add (new method)
    connect(_btnAdd,SIGNAL(clicked()),this,SLOT(AddSound()));
    // Delete button needs to know which item we are deleting, this method will deal with it
    connect(_soundListDisplay,SIGNAL(itemClicked(QListWidgetItem*)),this,SLOT(ItemWasClicked(QListWidgetItem*)));
    // Delete
    connect(_btnDelete,SIGNAL(clicked()),this,SLOT(DeleteSelectedSound()));

    /*******************RADIO BUTTONS************************/
    connect(_radioGroup, SIGNAL(buttonClicked(int)), this, SLOT(RadioPressed(int)));

    /*******************HOTKEY EDITION***********************/
    connect(_shortcutResetBtn,SIGNAL(clicked()), this, SLOT(ShortcutReset()));
    connect(_shortcutEdit,SIGNAL(keySequenceChanged(QKeySequence)),this,SLOT(SetKeySequence(QKeySequence)));
    // connect two signals to deal with the one sec delay
    connect(_shortcutEdit,SIGNAL(keyPressed()),this,SLOT(editingStarted()));
    connect(_shortcutEdit,SIGNAL(editingFinished()),this,SLOT(editingDone()));

    /*******************BUTTON DONE AND ABORT*****************/
    connect(_btnDone, SIGNAL(clicked()), this, SLOT(CreateWrapper()));
    connect(this,SIGNAL(signalAddDone(SoundWrapper*)),_mainWidget,SLOT(soundAdded(SoundWrapper*)));
    connect(_btnAbort, SIGNAL(clicked()), this, SLOT(AbortMission()));
}

// overload to ADD sound
WrapperProperties::WrapperProperties(int mainOutput,int VACOutput,int microphone,int pttScanCode,int pttVirtualKey,QWidget *parent) : WrapperProperties(parent)
{
    this->_mainOutput = mainOutput;
    this->_VACOutput = VACOutput;
    this->_microphone = microphone;
    this->_pttScanCode = pttScanCode;
    this->_pttVirtualKey = pttVirtualKey;
}

// Overloaded contructor to show properties of already built SoundWrapper object
// we call the other constructor so we don't have to to this twice forsenE

//Send infos about all the devices and the push to talk key to hold
//Basically, first 4 things are
// the main audio output
// the VAC output
// the microphone to inject
// and the PTT key to autohold (as both Scan Code and Virtual Key)
// last item is the soundwrapper
WrapperProperties::WrapperProperties(int mainOutput,int VACOutput,int microphone,int pttScanCode,int pttVirtualKey,SoundWrapper *sound, QWidget *parent)
    : WrapperProperties(mainOutput,VACOutput, microphone,pttScanCode,pttVirtualKey,parent) //: QWidget(parent)
{
    // Disconnect the done button because else it will create another wrapper
    disconnect(_btnDone, SIGNAL(clicked()), this, SLOT(CreateWrapper()));
    // Add the files contained in the wrapper to the list


    for (auto &i: sound->getSoundList())
    {
        _soundListDisplay->insertItem(_soundListDisplay->count(),i->fileName());

    }
    // Set the mode to the according one
    _playBackMode = sound->getPlayMode();
    switch(_playBackMode)
    {
        case 1: this->_radioSingleton->setChecked(true); ; break;
        case 2: _radioSequential->setChecked(true); break;
        case 3: _radioAuto->setChecked(true); break;
    }
    // set the shortcut
    this->_shortcutEdit->setKeySequence(sound->getKeySequence());


    connect(_btnDone, SIGNAL(clicked()), this, SLOT(SendEditedWrapper()));
    connect(this,SIGNAL(signalEditDone(SoundWrapper*)),_mainWidget,SLOT(soundModified(SoundWrapper *)));
}



void WrapperProperties::closeEvent(QCloseEvent *event)
{

            this->_mainWidget->setEnabled(true);
    //  qDebug() << "LEAYOUHOE";
            event->accept();
    //         qDebug() << "forsenRope FeelsBadMan";
}

/*******************************************************
 *                                                     *
 *                     SLOTS                           *
 *                                                     *
 *******************************************************/

void WrapperProperties::AddSound()
{
    // if the playback is singleton and we already have one we don't do nuffin
    if ( !(_soundListDisplay->count()>=1 && _radioGroup->checkedId() == 1 ))
    {
        QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),"", tr("Sounds (*.wav *.mp3 *.ogg)"));
        // if the fileName isn't empty, the user selected a file, so we add it.
        if (!fileName.isEmpty())
        {
            //QListWidgetItem * tempItem = new QListWidgetItem(fileName);
            //qDebug() << _soundListDisplay->row(item);
            _soundListDisplay->insertItem(_soundListDisplay->count() ,fileName);
            // Disabling the double click event if singleton
            //qDebug() << _radioGroup->checkedId();
            // If singleton we disable the add sound option
            if (_radioGroup->checkedId() == 1)
                this->_btnAdd->setEnabled(false);
        }
    }

}



// Event dealing with the colors if mode is changed
void WrapperProperties::RadioPressed(int id)
{
    this->_playBackMode = id;
    // If we have 0 item we don't really care
    if (_soundListDisplay->count()<1)
        return;
    switch(id){
        case 1:
            this->_btnAdd->setEnabled(false);
        break;
        // 2 and 3 are sequential or sequential auto
        default:
            this->_btnAdd->setEnabled(true);
        break;

    }
}
void WrapperProperties::AbortMission()
{
    this->close();
}


//Clear the input box for the shortcut but also clears
//the actual shortcut since it emits the signal that calls the SetKeySequenceSlot
void WrapperProperties::ShortcutReset()
{
    //qDebug() << this->_shortcutSequence->toString();
    this->_shortcutEdit->clear();
    //qDebug() << this->_shortcutSequence->toString();
}

void WrapperProperties::SetKeySequence(QKeySequence shortcut)
{
    this->_shortcutSequence->swap(shortcut);
}

void WrapperProperties::CreateWrapper()
{
    // we check for the sound being singleton or not, if it is, we don't accept if we have more than 1 sound

    if( (this->_playBackMode == 1) && (this->_soundListDisplay->count()  >1))
    {
        QMessageBox::critical(this, "Error", "Singleton cannot contain more than one sound file.");
        return;
    }

    SoundWrapper *tmpSound = new SoundWrapper(this->_soundListDisplay,
                                              this->_playBackMode,
                                              this->_shortcutSequence,
                                              this->_shortcutEdit->getVirtualKey(),
                                              nullptr);
    // we emit the signal so that the main window knows
    // IT KNOWS forsenKek
    //tmpSound->setPTTKeySequence( _mainWidget->getPTTKeySequence());
    tmpSound->setPlayerPTTScanCode(_pttScanCode);
    tmpSound->setPlayerPTTVirtualKey(_pttVirtualKey);
    tmpSound->setPlayerMainOutput(_mainOutput);
    tmpSound->setPlayerVACOutput(_VACOutput);

    emit signalAddDone(tmpSound);
    this->close();
}

void WrapperProperties::SendEditedWrapper()
{
    if( (this->_playBackMode == 1) && (this->_soundListDisplay->count()  >1))
    {
        QMessageBox::critical(this, "Error", "Singleton cannot contain more than one sound file.");
        return;
    }
    //SoundWrapper::SoundWrapper(QListWidget* soundList, int playbackMode,QKeySequence * shortcut, QObject * parent)
    // We construct the item first and than set the player parameters.
    SoundWrapper *tmpSound = new SoundWrapper(this->_soundListDisplay,
                                              this->_playBackMode,
                                              this->_shortcutSequence,
                                              this->_shortcutEdit->getVirtualKey() ,
                                              nullptr);
    // we emit the signal so that the main window knows
    // IT KNOWS forsenKek
    tmpSound->setPlayerPTTScanCode(_pttScanCode);
    tmpSound->setPlayerPTTVirtualKey(_pttVirtualKey);
    tmpSound->setPlayerMainOutput(_mainOutput);
    tmpSound->setPlayerVACOutput(_VACOutput);


    emit signalEditDone(tmpSound);
    //qDebug() << "forsenT";
    this->close();
}

void WrapperProperties::editingDone()
{
    this->_btnDone->setEnabled(true);
}

void WrapperProperties::editingStarted()
{
    this->_btnDone->setEnabled(false);
}


void WrapperProperties::ItemWasClicked(QListWidgetItem * item)
{
    _btnDelete->setEnabled(true);
    _selectedItem = item;
}

void WrapperProperties::DeleteSelectedSound()
{
    delete this->_selectedItem;
    this->_btnDelete->setEnabled(false);
    // need to renable add button if we are in singleton song and this was the last sound
    if (_soundListDisplay->count() == 0)
        this->_btnAdd->setEnabled(true);
}




//void WrapperProperties::ContextDelete(const QPoint &pos)
//{
//    qDebug() << this->childAt(pos);
//}
//void WrapperProperties::Test(SoundWrapper * lul)
//{
//    qDebug() << "Signal emitted";

//}


