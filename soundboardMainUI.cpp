#include "soundboardMainUI.h"

SoundboardMainUI::SoundboardMainUI(QWidget *parent) : QWidget(parent)

{

    // Setting up the layouts
    vLayout = new QVBoxLayout(this);
    // not need since adding a parent in the constructor set layout by itself
    // this->setLayout(vLayout);
    //Adding Menu bar
    this->setUpMenu();


    // we use MVC architecture. This is the declaration of the _model
    // each case of the view is a model forsenT
    _model = new QStandardItemModel(0,3,this);
    //Adding the headers
    _model->setHorizontalHeaderLabels(
        (QStringList() << "Sound"
                       << "Shortcut")
                       << "Mode");
   // Also works
   // _model->setHeaderData(0,Qt::Horizontal, QString("Sound File"));
   // _model->setHeaderData(1,Qt::Horizontal, tr("Shortcut"));


    //Creating the Viewer
    //resultView = new QTableView(this);
    resultView = new CustomTableView(this);

    //Applying the 1 line 2 column _model
    resultView->setModel(_model);
    // hiding the vertical headers on the left side
    resultView->verticalHeader()->hide();
    // Setting so the user can only select row forsenE
    resultView->setSelectionBehavior(QAbstractItemView::SelectRows);
    resultView->setSelectionMode(QAbstractItemView::SingleSelection);
    //QStandardItem item;
    // Adding the viewer to the layout
     vLayout->addWidget(resultView);
    // Set up the QTableView to fill the layout
     resultView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    // Set up the column of said QTableView to stretch evenly
     resultView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
     resultView->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);
     resultView->show();

     // Adding the grid layout to the vertical now, so that's it's under it
     _gLayout = new QGridLayout();

     vLayout->addLayout(_gLayout);

     //Creating buttons
     _btnAdd = new QPushButton("Add",this);
     _btnDelete = new QPushButton("Delete",this);
     _btnEdit = new QPushButton("Edit",this);
     _btnPlay = new QPushButton("Play",this);
     _btnStop = new QPushButton("Stop",this);

      // Disabling the edit and delete button so that the soundboard doesn't crash
      // if no items are selected
      this->_btnDelete->setEnabled(false);
      this->_btnEdit->setEnabled(false);

      // Adding them to grid layout
      _gLayout->addWidget(_btnAdd,0,0,1,1);
      _gLayout->addWidget(_btnDelete,0,1,1,1);
      _gLayout->addWidget(_btnEdit,0,2,1,1);
      _gLayout->addWidget(_btnPlay,0,4,1,1);
      _gLayout->addWidget(_btnStop,0,5,1,1);

      // Adding label to vlayout
      _label1 = new QLabel("1st Output (e.g. your speakers)",this);
      _gLayout->addWidget(_label1,1,0,1,6);

      // Adding output list
      _deviceListOutput = new QComboBox(this);
      _gLayout->addWidget(_deviceListOutput,2,0,1,6 );
      // Second label
      _label2 = new QLabel("Virtual Audio Cable output (optional)",this);
      _gLayout->addWidget(_label2,3,0,1,6);

      // Combo box to select VAC
      _deviceListVAC = new QComboBox(this);
      _gLayout->addWidget(_deviceListVAC,4,0,1,6);

      /***************************************************
                        MIC INJECTION SECTION
      ****************************************************/
      _label3 = new QLabel("Setup microphone injection (optional)",this);
      _gLayout->addWidget(_label3,5,0,1,6);
      //_deviceListInjector = new QComboBox(this);
      _btnMicInjection = new QPushButton("Open sound configuration",this);
      _gLayout->addWidget(_btnMicInjection,6,0,1,6);

       connect(this->_btnMicInjection,SIGNAL(clicked()),this,SLOT(openAudioSettings()));

         // _gLayout->addWidget(_deviceListInjector,6,0,1,6);

      // Fetching the devices
      this->fetchDeviceList(_deviceListOutput,QAudio::AudioOutput);
      this->fetchDeviceList(_deviceListVAC,QAudio::AudioOutput);
  //    this->fetchDeviceList(_deviceListInjector,QAudio::AudioInput);

      /***************************************************
                            PTT BIND
      ****************************************************/
      _label4 = new QLabel("Push to talk key to auto-hold:");
      _shortcutEditPTT = new CustomShortcutEdit();
      _btnClearPTT = new QPushButton("Clear");

      _gLayout->addWidget(_label4,7,0,1,3);
      _gLayout->addWidget(_shortcutEditPTT,7,4,1,1);
      _gLayout->addWidget(_btnClearPTT,7,5,1,1);

      connect(this->_btnClearPTT,SIGNAL(clicked()),this,SLOT(resetPushToTalkEdit()));
      /***************************************************
                         STOP SOUND BIND
      ****************************************************/
      _label5 = new QLabel("Stop ALL sound shortcut:");
      _shortcutEditStop= new CustomShortcutEdit();
      _btnClearStop = new QPushButton("Clear");

      _gLayout->addWidget(_label5,8,0,1,3);
      _gLayout->addWidget(_shortcutEditStop,8,4,1,1);
      _gLayout->addWidget(_btnClearStop,8,5,1,1);

      connect(this->_btnClearStop,SIGNAL(clicked()),this,SLOT(resetStopAllEdit()));
      connect(this->_shortcutEditStop,SIGNAL(virtualKeyChanged(int)),this,SLOT(setStopShortcut(int)));
      // WE ALSO NEED THOSE BUTTONS TO SEND -1 when reset forsenT


      /***************************************************
               TODO: PUT IN RELEVANT SECTION forsenT
      ****************************************************/
      connect(this->_btnAdd, SIGNAL(clicked()), this, SLOT(addSoundDialog()));
      connect(this->resultView,SIGNAL(clicked(QModelIndex)),this,SLOT(onCellClicked(QModelIndex)));

      connect(this->_btnEdit, SIGNAL(clicked()), this, SLOT(editSoundDialog()));
      connect(this->_btnDelete, SIGNAL(clicked()), this, SLOT(deleteSound()));


      connect(this->resultView,SIGNAL(enableButtons()),this,SLOT(enableButtons()));
      connect(this->resultView,SIGNAL(disableButtons()),this,SLOT(disableButtons()));



}



void SoundboardMainUI::fetchDeviceList(QComboBox *comboBox, QAudio::Mode mode)
{
    // Adding null device
    comboBox->addItem("<No device selected>",Qt::DisplayRole);

//    for (auto &deviceInfo: QAudioDeviceInfo::availableDevices(mode))
//        comboBox->addItem(deviceInfo.deviceName(),Qt::DisplayRole);
   // Modifying this so it is the same order as BASS library
    //http://www.un4seen.com/doc/#bass/BASS_GetDeviceInfo.html
  //device 0 is always the "no sound" device, so you should start at device 1 if you only want to list real output devices.

   if (mode == QAudio::AudioOutput)
   {
        BASS_DEVICEINFO info;
        for (long i=1; BASS_GetDeviceInfo(i, &info); i++)
            if (info.flags&BASS_DEVICE_ENABLED) // device is enabled
                comboBox->addItem(info.name, Qt::DisplayRole);
   }
   else if (mode == QAudio::AudioInput)
   {
        BASS_DEVICEINFO info;
        for (int n=0; BASS_RecordGetDeviceInfo(n, &info); n++)
            comboBox->addItem(info.name, Qt::DisplayRole);

   }

}
// The dialogue to be opened when the Add button is pressed
void SoundboardMainUI::addSoundDialog()
{
    this->setEnabled(false);

    _propertiesWindow = new WrapperProperties(
                this->_deviceListOutput->currentIndex(),
                this->_deviceListVAC->currentIndex(),
                -1,
                this->_shortcutEditPTT->getScanCode(),
                this->_shortcutEditPTT->getVirtualKey(),
                this);
    _propertiesWindow->show();

}

void SoundboardMainUI::deleteSound()
{
    // check if selected sound is inside the array
    if (this->lastSelectedRow <= this->_sounds.size())
    {
        //Schedule deletion just in case
        this->_sounds.at(lastSelectedRow)->deleteLater();
        this->_sounds.removeAt(lastSelectedRow);
        this->_data.removeAt(lastSelectedRow);
        this->_model->removeRow(lastSelectedRow);
        // Unregistering the hotkey
        UnregisterHotKey(NULL,_winShorcutHandle.at(lastSelectedRow));
        /* Deleting the associated handle and keysquence
         *  (we don't really need keysequence since
         * we use virtual keys now but oh well) */
        this->_winShorcutHandle.removeAt(lastSelectedRow);
        this->_keySequence.removeAt(lastSelectedRow);
        this->_keyVirtualKey.removeAt(lastSelectedRow);
       // qDebug() << "sounds size:" << _sounds.size() << "shortcut size" << _winShorcutHandle.size();


        // Regenerate shortcuts so that it doesn't go OOB when trying to play a non-existing handle
        GenerateGlobalShortcuts();
    }
    // If we have no more sounds in the soundboard, disable delete and edit button
    if (this->_sounds.size() == 0)
    {
        this->_btnDelete->setEnabled(false);
        this->_btnEdit->setEnabled(false);
    }
    // in any case we clear the focus

    resultView->clearFocus();
    resultView->clearSelection();
}


// Add a sound if whereToInsert isn't
void SoundboardMainUI::soundAdded(SoundWrapper * modifiedSound, int whereToInsert)
{
    //connecting the wrappper to the combo box for devices
    connect(this->_deviceListOutput,SIGNAL(currentIndexChanged(int)),modifiedSound,SLOT(OutputDeviceChanged(int)));
    connect(this->_deviceListVAC,SIGNAL(currentIndexChanged(int)),modifiedSound,SLOT(VACDeviceChanged(int)));

    // connecting the pushtotalk key thing
    connect(this->_shortcutEditPTT,SIGNAL(scanCodeChanged(int)),modifiedSound,SLOT(PTTScanCodeChanged(int)));
    connect(this->_shortcutEditPTT,SIGNAL(virtualKeyChanged(int)),modifiedSound,SLOT( PTTVirtualKeyChanged(int)));

    // connecting the stop btn
    connect(this->_btnStop,SIGNAL(clicked()),modifiedSound,SLOT(Stop()));

    QList<QStandardItem*> tempList;
    tempList = modifiedSound->getSoundAsItem();

    //setting the flags so that it can be clicked on
    for (auto &i:tempList)
        i->setFlags(Qt::ItemIsSelectable |Qt::ItemIsEnabled);

    // if sound was added we append it
    if (whereToInsert == -1)
    {
        _sounds.append(modifiedSound);
        _data.append(tempList);
        _model->appendRow(_data.last());
        // addind the key sequence to the shortcut list
        _winShorcutHandle.append(_winShorcutHandle.size());
        _keySequence.append(modifiedSound->getKeySequence());
        _keyVirtualKey.append(modifiedSound->getShortcutVirtualKey());
    }
    // else it was modified, need to swap  the previous item by the new, and than delete the item
    else
    {
        _sounds.removeAt(lastSelectedRow);
        _sounds.insert(lastSelectedRow,modifiedSound);
        // change data too
        _data.removeAt(lastSelectedRow);
        _data.insert(lastSelectedRow,tempList);
        //we need to update model accordingly
        _model->removeRow(lastSelectedRow);
        _model->insertRow(lastSelectedRow,_data.at(lastSelectedRow));
        // updating the shortcuts table
        _keySequence.removeAt(lastSelectedRow);
        _keySequence.insert(lastSelectedRow,modifiedSound->getKeySequence());
        // updating keyscancode table
        _keyVirtualKey.removeAt(lastSelectedRow);
        _keyVirtualKey.insert(lastSelectedRow,modifiedSound->getShortcutVirtualKey());

    }

    this->GenerateGlobalShortcuts();

    // we resize
    this->resultView->resizeRowsToContents();

}


// Dealing with click on a row: update index
void SoundboardMainUI::onCellClicked(QModelIndex index)
{
    // disconnect the play button
    disconnect(_btnPlay,0,0,0);
    lastSelectedRow = index.row();
    this->_btnEdit->setEnabled(true);
    this->_btnDelete->setEnabled(true);
    this->_btnPlay->setEnabled(true);
    // qDebug() << "User clicked a cell on row number: " << index.row();
    // connect it to the selected cell

    connect(_btnPlay,SIGNAL(clicked()),_sounds.at(index.row()),SLOT(Play()));
}

// open the dialog to edit sound
void SoundboardMainUI::editSoundDialog()
{
    //if lastSelectedRow is valid (ie we didn't delete this entry)
    if (this->lastSelectedRow <= this->_sounds.size())
    {
        //Send infos about all the devices and the push to talk key to hold
        // and the sound wrapper
       _propertiesWindow= new WrapperProperties(
                   this->_deviceListOutput->currentIndex(),
                   this->_deviceListVAC->currentIndex(),
                   -1,
                   this->_shortcutEditPTT->getScanCode(),
                   this->_shortcutEditPTT->getVirtualKey(),
                   this->_sounds.at(this->lastSelectedRow)  ,
                   this);
       _propertiesWindow->show();
    }
}
// deal with modified sound
void SoundboardMainUI::soundModified(SoundWrapper *modifiedSound)
{
    this->soundAdded(modifiedSound,lastSelectedRow);
}

void SoundboardMainUI::enableButtons()
{
    _btnEdit->setEnabled(true);
    _btnDelete->setEnabled(true);

}


void SoundboardMainUI::disableButtons()
{
    _btnEdit->setEnabled(false);
    _btnDelete->setEnabled(false);

}

void SoundboardMainUI::GenerateGlobalShortcuts()
{
    // _keySequence and _winShortcutHandle should always be same size
    if (_keySequence.size() != _winShorcutHandle.size())
    {
        //qDebug() <<"DansGame";
        return;
    }
    //need to UNREGISTER ALL hotkeys and rebind them
    //https://msdn.microsoft.com/fr-fr/library/windows/desktop/ms646327(v=vs.85).aspx
   // qDebug() << "Unregistering all hotkeys";
    for (auto i: _winShorcutHandle)
    {
        UnregisterHotKey(NULL,i);
    }

    // now we register the hotkeys
    // https://msdn.microsoft.com/en-us/library/windows/desktop/ms646309(v=vs.85).aspx
    int count = 0;
    for (auto i: _keySequence)
    {
        // if they key is empty we skip it duh, but we still increment count so that table matches
        if (!i.isEmpty())
        {
            // since alt and ctrl CANNOT be binded alone
            // first argument will either be a modifier (Ctrl, alt) or the key
        //    qDebug() << i.toString();

            // Looking for the flags and setting no repeat as default. to check for spamming sound forsenT
            int tempFlags = MOD_NOREPEAT;
            for (auto j: i.toString().split("+"))
            {
                if (j=="Ctrl")
                    tempFlags = tempFlags | MOD_CONTROL;
                else if (j=="Alt")
                    tempFlags = tempFlags | MOD_ALT;
                else if (j=="Shift")
                    tempFlags = tempFlags | MOD_SHIFT;
            }
       //   qDebug() << "Attempting to register the shortcut:" <<i.toString() ;

            // Testing the new stuff

           RegisterHotKey(NULL, count,tempFlags, _keyVirtualKey.at(count) );
       //     qDebug() << "Registering hotkey handle number:" << count;
       //     qDebug() << _sounds.at(0)->getSoundListAsQString();
        }
        count++;
    }




}


// Method to run the sound via hotkeys
void SoundboardMainUI::winHotKeyPressed(int handle)
{
      qDebug() << "Pressed hotkey handle: " << handle;

    // If this is the STOP hotkey then we stop all sounds
    if (handle == 2147483647)
        for (auto &i: _sounds)
            i->Stop();

    else
        _sounds.at(handle)->Play();
}


void SoundboardMainUI::setUpMenu()
{

    /***************************************************
                            FILE
    ****************************************************/
    _menuBar = new QMenuBar(this);
    _menuBar->setFixedHeight(20);
    vLayout->addWidget(_menuBar);
    QMenu * fileMenu = _menuBar->addMenu(tr("File"));
    _actions.append(   new QAction("New",this));
    _actions.append(   new QAction("Open",this));
    _actions.append(   new QAction("Open EXP soundboard file",this));

    _actions.append(   new QAction("Save",this));
    _actions.append(   new QAction("Save as..",this));
    fileMenu->addSeparator();
    _actions.append(   new QAction("Exit",this));

    fileMenu->addAction(_actions.at(0));
    fileMenu->addAction(_actions.at(1));
    fileMenu->addAction(_actions.at(2));

    fileMenu->addSeparator();
    fileMenu->addAction(_actions.at(3));
    fileMenu->addAction(_actions.at(4));
    fileMenu->addSeparator();
    fileMenu->addAction(_actions.at(5));

    /***************************************************
                            Help
    ****************************************************/
    QMenu * helpMenu = _menuBar->addMenu(tr("Help"));
    _actions.append(   new QAction("Guide",this));
    _actions.append(   new QAction("Report a bug..",this));
    _actions.append(   new QAction("About LIDL Soundboard",this));
    helpMenu->addAction(_actions.at(6));
    helpMenu->addAction(_actions.at(7));
    helpMenu->addAction(_actions.at(8));
    /***************************************************
                            HELP
    ****************************************************/


}

//Reimplementing to kill all shortcuts
void SoundboardMainUI::closeEvent (QCloseEvent *event)
{

    for (auto i: _winShorcutHandle)
    {
        UnregisterHotKey(NULL,i);
    }

    // send message to stop the listening loop L OMEGALUL OMEGALUL P
    PostQuitMessage(0);
    QWidget::closeEvent(event);
}


void SoundboardMainUI::resetPushToTalkEdit()
{
    // Clearing the thing and setting the PTTScanCode and the PTTVirtualKey to -1
    _shortcutEditPTT->clear();

    for (auto &i: _sounds)
    {
        i->PTTVirtualKeyChanged(-1);
        i->PTTScanCodeChanged(-1);
    }


}

void SoundboardMainUI::resetStopAllEdit()
{
    UnregisterHotKey(NULL,2147483647);
    _shortcutEditStop->clear();
}

void SoundboardMainUI::setStopShortcut(int virtualKey)
{
    UnregisterHotKey(NULL,2147483647);
    RegisterHotKey(NULL,2147483647,0, virtualKey);

}

void SoundboardMainUI::openAudioSettings()
{

    //system("control mmsys.cpl sounds");
    WinExec("control mmsys.cpl sounds",8);
}


