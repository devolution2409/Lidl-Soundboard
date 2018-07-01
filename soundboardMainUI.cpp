#include "soundboardMainUI.h"

SoundboardMainUI::SoundboardMainUI(QWidget *parent) : QMainWindow(parent)
{
    //    QFile css_dark(":/css/resources/darkorange.css");
    //    css_dark.open(QFile::ReadOnly);
    //    this->setStyleSheet(css_dark.readAll());
    //    this->menuBar()->setStyle(QStyleFactory::create("plastique"));
    //    css_dark.close();
    //   this->setWindowFlag( Qt::FramelessWindowHint);
    this->resize(400,600);
    this->setWindowTitle( "LIDL Sounboard " + QString(VER_STRING));
    this->setWindowIcon(QIcon(":/icon/resources/forsenAim.png"));
    this->setAcceptDrops(true);
    /***************************************************
                      SETTING UP MENU BAR
    ****************************************************/
    this->setUpMenu();
    _updateScheduled = false;
    // Setting up the layouts
    // vLayout = new QVBoxLayout();
    // not need since adding a parent in the constructor set layout by itself
    //  _nameEdit = new QLineEdit(this);

    // Adding the grid layout to the vertical now, so that's it's under it
    _gLayout = new QGridLayout();




    // since we QMainWindow now we need to set central widget forsenT
    this->setCentralWidget(new QWidget(this));
    this->centralWidget()->setLayout(_gLayout);


    // we use MVC architecture. This is the declaration of the _model
    // each case of the view is a model forsenT
    //_model = new QStandardItemModel(0,3,this);
    _model = new CustomTableModel(0,4,this);
    //Adding the headers
    // Headers will be set by the ClearAll() method which is called regardless

    // Also works
    // _model->setHeaderData(0,Qt::Horizontal, QString("Sound File"));
    // _model->setHeaderData(1,Qt::Horizontal, tr("Shortcut"));


    //Creating the Viewer
    resultView = new CustomTableView(this);

    //Applying the 1 line 2 column _model
    resultView->setModel(_model);

    connect(_model,SIGNAL(draggedOnRow(int)),this,SLOT(DealDragAndDrop(int)));
    _gLayout->addWidget(resultView,0,0,1,6);
    _gLayout->setRowStretch(0,90);
    _gLayout->setColumnStretch(0,100);
    //QStandardItem item;
    // Adding the viewer to the layout
    //     vLayout->addWidget(resultView);
    //     vLayout->addLayout(_gLayout);

    // Set up the QTableView to fill the layout
    resultView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    // Set up the column of said QTableView to stretch evenly
    resultView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    resultView->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);

    resultView->show();



    //Creating buttons
    _btnAdd = new QPushButton("Add",this);
    _btnDelete = new QPushButton("Delete",this);
    _btnEdit = new QPushButton("Edit",this);
    _btnPlay = new QPushButton("Play",this);
    _btnStop = new QPushButton("Stop All",this);

    // Disabling the edit and delete button so that the soundboard doesn't crash
    // if no items are selected
    this->_btnDelete->setEnabled(false);
    this->_btnEdit->setEnabled(false);

    // Adding them to grid layout
    _gLayout->addWidget(_btnAdd,1,0,1,1);
    _gLayout->addWidget(_btnDelete,1,1,1,1);
    _gLayout->addWidget(_btnEdit,1,2,1,1);
    _gLayout->addWidget(_btnPlay,1,4,1,1);
    _gLayout->addWidget(_btnStop,1,5,1,1);

    // Adding label to vlayout
    _label1 = new QLabel("1st Output (e.g. your speakers)",this);
    _gLayout->addWidget(_label1,2,0,1,6);

    // Adding output list
    _deviceListOutput = new QComboBox(this);
    _gLayout->addWidget(_deviceListOutput,3,0,1,6 );
    // Second label
    _label2 = new QLabel("Virtual Audio Cable output (optional)",this);
    _gLayout->addWidget(_label2,4,0,1,6);

    // Combo box to select VAC
    _deviceListVAC = new QComboBox(this);
    _gLayout->addWidget(_deviceListVAC,5,0,1,6);

    /***************************************************
                        MIC INJECTION SECTION
      ****************************************************/
    _label3 = new QLabel("Setup microphone injection (optional)",this);
    _gLayout->addWidget(_label3,6,0,1,6);
    //_deviceListInjector = new QComboBox(this);
    _btnMicInjection = new QPushButton("Open sound configuration",this);
    _gLayout->addWidget(_btnMicInjection,7,0,1,6);

    connect(this->_btnMicInjection,QPushButton::clicked,this, [=]{
        WinExec("control mmsys.cpl sounds",8);
        });

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

    _gLayout->addWidget(_label4,8,0,1,3);
    _gLayout->addWidget(_shortcutEditPTT,8,4,1,1);
    _gLayout->addWidget(_btnClearPTT,8,5,1,1);

    connect(this->_btnClearPTT,QPushButton::clicked,this,[=]{
            // Clearing the thing and setting the PTTScanCode and the PTTVirtualKey to -1
            _shortcutEditPTT->clear();
            for (auto &i: _sounds)
            {
                i->PTTVirtualKeyChanged(-1);
                i->PTTScanCodeChanged(-1);
            }
        });
    /***************************************************
                         STOP SOUND BIND
      ****************************************************/
    _label5 = new QLabel("Stop ALL sound shortcut:");
    _shortcutEditStop= new CustomShortcutEdit();
    _btnClearStop = new QPushButton("Clear");

    _gLayout->addWidget(_label5,9,0,1,3);
    _gLayout->addWidget(_shortcutEditStop,9,4,1,1);
    _gLayout->addWidget(_btnClearStop,9,5,1,1);

    connect(this->_btnClearStop,QPushButton::clicked,this,[=]{
        UnregisterHotKey(NULL,2147483647);
        _shortcutEditStop->clear();
    });

    connect(this->_shortcutEditStop,SIGNAL(virtualKeyChanged(int)),this,SLOT(setStopShortcut(int)));
    // WE ALSO NEED THOSE BUTTONS TO SEND -1 when reset forsenT

    /***************************************************
                         STATUS BAR
      ****************************************************/

    _statusEdit = new QTextEdit();
    _statusEdit->setMaximumHeight(20);
    _statusEdit->setAcceptDrops(false);
    _statusEdit->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    _statusEdit->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    _statusEdit->setLineWrapMode(QTextEdit::NoWrap);
    _statusEdit->setReadOnly(true);
    _statusEdit->setCursorWidth(0);
    _statusEdit->setEnabled(false);
    _statusEdit->setStyleSheet("border: none; color: #b1b1b1;");

    this->statusBar()->show();
    this->statusBar()->setMaximumHeight(20);
    this->statusBar()->setStyleSheet( "background: #727272; border: 1px solid black" );
    this->statusBar()->setSizeGripEnabled(false);
    this->statusBar()->addPermanentWidget(_statusEdit,1);
    connect(this->statusBar(),SIGNAL(messageChanged(QString)),this,SLOT(SetStatusTextEditText(QString)));
    /***************************************************
               TODO: PUT IN RELEVANT SECTION forsenT
      ****************************************************/

    // Lambda forsenE
    connect(this->_btnAdd, QPushButton::clicked, this, [=]{
        this->setEnabled(false);

        _propertiesWindow = new WrapperProperties(
                    this->_deviceListOutput->currentIndex(),
                    this->_deviceListVAC->currentIndex(),
                    this->_shortcutEditPTT->getScanCode(),
                    this->_shortcutEditPTT->getVirtualKey(),
                    nullptr,
                    this);
        // Connection of the done button to mainUI slots is dealt in the contructor
        // to account for edit or add mode
        _propertiesWindow->show();

    });
    // Lambda
    //connect(this->resultView,SIGNAL(clicked(QModelIndex)),this,SLOT(onCellClicked(QModelIndex)));

    connect(this->resultView, CustomTableView::clicked, this, [=](QModelIndex index){
        // disconnect the play button
        disconnect(_btnPlay,0,0,0);
        lastSelectedRow = index.row();
        this->_btnEdit->setEnabled(true);
        this->_btnDelete->setEnabled(true);
        this->_btnPlay->setEnabled(true);
        // qDebug() << "User clicked a cell on row number: " << index.row();
        // connect it to the selected cell
        // ONLY if soundlist isn't empty
        if ( ! _sounds.at(index.row())->getSoundList().isEmpty())
            connect(_btnPlay,SIGNAL(clicked()),_sounds.at(index.row()),SLOT(Play()));


    });

    // Lambda
    connect(this->resultView, CustomTableView::doubleClicked,this, [=](QModelIndex index){
        // but we update it regardless
        disconnect(_btnPlay,0,0,0);
        lastSelectedRow = index.row();
        this->_btnEdit->setEnabled(true);
        this->_btnDelete->setEnabled(true);
        this->_btnPlay->setEnabled(true);
        if ( ! _sounds.at(index.row())->getSoundList().isEmpty())
            connect(_btnPlay,SIGNAL(clicked()),_sounds.at(index.row()),SLOT(Play()));
        // And we call the edit sound dialog
        this->editSoundDialog();
    });

    connect(this->_btnEdit, SIGNAL(clicked()), this, SLOT(editSoundDialog()));
    connect(this->_btnDelete, QPushButton::clicked, this, [=]{    // check if selected sound is inside the array
        if (this->lastSelectedRow <= this->_sounds.size())
        {
            this->SetStatusTextEditText("Deleted selected sound");
            // disconnect anything connected to the sound
            disconnect(_sounds.at(lastSelectedRow));

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
        resultView->clearSelection();});


    connect(this->resultView, CustomTableView::enableButtons  ,this, [=]{
        _btnEdit->setEnabled(true);
        _btnDelete->setEnabled(true);});
    connect(this->resultView,CustomTableView::disableButtons,this, [=]{
        _btnEdit->setEnabled(false);
        _btnDelete->setEnabled(false);
    });

    // Declaring savename empty string so save doesn't work forsenE
    this->_saveName = "";

    /* Calling OpenSetting will created the instance of the SettingsController we can after wards access it everywhere.
       * If the settings file was found, this function will return true
       * => we can automatically open last opened file */

    // Ensure LIDL::SettingsController exist so we can connect it
    if (LIDL::SettingsController::GetInstance() != nullptr)
        connect(this,&SoundboardMainUI::lidlJsonDetected,
                LIDL::SettingsController::GetInstance(),&LIDL::SettingsController::addFile);

    // We are required to do this trick else
    // the window isn't existing when the AddSound method attempts to resize columns

    this->setMinimumSize(400,600);
    this->setMaximumSize(16777215,16777215);
    this->show();
    /***************************************************
                   CONNECTING TO OPEN SOUNDBOARD
      ****************************************************/
    // QueudConnection so that it doesn't fucks up the view
    connect(this,&SoundboardMainUI::OnConstructionDone,this,&SoundboardMainUI::PostConstruction,Qt::QueuedConnection);


    connect(LIDL::SettingsController::GetInstance(),
            LIDL::SettingsController::RecentFilesChanged,
            this,
            SoundboardMainUI::SetUpRecentMenu,
            Qt::QueuedConnection
            );
    connect(this,
            SoundboardMainUI::SaveSoundboardState,
            [=]
    {

        LIDL::SettingsController::GetInstance()->SaveState(*(this->GenerateSaveFile()));
    });
    connect(LIDL::SettingsController::GetInstance(),
            LIDL::SettingsController::SettingsChanged,
            this, SoundboardMainUI::SetUpRecentMenu,Qt::QueuedConnection);

    // connected modified soundboard with a lambda to call the savestate function
    // that way we can know if soundboard was modified or not Pog

    emit OnConstructionDone();


    /* CONNECTING THE SETTINGS CONTROLLER TO THE PTT KEY THINGS */
    connect(this->_shortcutEditPTT,SIGNAL(scanCodeChanged(int)),
            LIDL::SettingsController::GetInstance(),SLOT(SetPTTScanCode(int)));

    connect(this->_shortcutEditPTT,SIGNAL(virtualKeyChanged(int)),
            LIDL::SettingsController::GetInstance(),SLOT(SetPTTVirtualKey(int)));
    connect(this->_btnStop,QPushButton::clicked, [=]{
        LIDL::SettingsController::GetInstance()->unHoldPTT();});


}

void SoundboardMainUI::PostConstruction()
{
    // Take a snapshot right now to avoid lidl errors when loading soundboard automatically
    LIDL::SettingsController::GetInstance()->SaveState(*this->GenerateSaveFile() );
    // Open the soundboard post-construction
    if (LIDL::SettingsController::GetInstance()->OpenSettings())
    {
        if(!(LIDL::SettingsController::GetInstance()->GetLastOpenedSoundboard().isEmpty()))
            QTimer::singleShot(0, [=]{   this->Open(LIDL::SettingsController::GetInstance()->GetLastOpenedSoundboard());});
        else // show firt use dialog
            this->ClearAll();
    }
    // If this is the first time the user uses soundboard
    if (LIDL::SettingsController::GetInstance()->IsThisFirstTimeUser())
        this->HelpShowFirstUserDialog();


    // Testing the thing for chatterino

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

// Add a sound if whereToInsert isn't
void SoundboardMainUI::addSound(SoundWrapper * modifiedSound, int whereToInsert, LIDL::Shortcut generationMode)
{
    qDebug() << "forsenWut";
    //connecting the wrappper to the combo box for devices
    connect(this->_deviceListOutput,SIGNAL(currentIndexChanged(int)),modifiedSound,SLOT(OutputDeviceChanged(int)));
    connect(this->_deviceListVAC,SIGNAL(currentIndexChanged(int)),modifiedSound,SLOT(VACDeviceChanged(int)));

    // connecting the pushtotalk key thing
    //connect(this->_shortcutEditPTT,SIGNAL(scanCodeChanged(int)),modifiedSound,SLOT(PTTScanCodeChanged(int)));
    //connect(this->_shortcutEditPTT,SIGNAL(virtualKeyChanged(int)),modifiedSound,SLOT( PTTVirtualKeyChanged(int)));



    // connecting the stop btn
    connect(this->_btnStop,SIGNAL(clicked()),modifiedSound,SLOT(Stop()));


    // connecting the status bar signal for unexistant files (reading json)
    connect(modifiedSound,SoundWrapper::UnexistantFile,this, [=]{
            this->SetStatusTextEditText("The files marked with ⚠️ aren't present on disk.");
        });
    // connecting the wrapper proxy signal for player NowPlaying
    connect(modifiedSound,SoundWrapper::NowPlaying,this,[=](QString name){
            this->SetStatusTextEditText("<b>Now playing: </b>\"" + name +"\"");
        });
    // connecting the wrapper proxy signal for player ErrorPlaying
    connect(modifiedSound,SoundWrapper::ErrorPlaying,this, [=](QString name){
            this->SetStatusTextEditText("<b>Error playing file: </b>\"" + name + "\"");
         });
    // connect the clear button to the clear shortcut slot
    connect(this->_actions.at(11),SIGNAL(triggered()),modifiedSound,SLOT(clearShorcut()));

    connect(modifiedSound, SoundWrapper::holdPTTProxy, [=] (int duration){
        LIDL::SettingsController::GetInstance()->holdPTT(duration);
    } );

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
        // Adding actual row the view is done by addind the data
        // to the model forsenT
        //_model->appendRow(_data.last());
        // addind the key sequence to the shortcut list
        _winShorcutHandle.append(_winShorcutHandle.size());
        _keySequence.append(modifiedSound->getKeySequence());
        _keyVirtualKey.append(modifiedSound->getShortcutVirtualKey());
    }
    // else it was modified, need to swap  the previous item by the new, and than delete the item
    else
    {
        //we stop it if its playing because else we destroy it and can't stop it
        _sounds.at(whereToInsert)->Stop();
        // forsenT
        _sounds.removeAt(whereToInsert);
        _sounds.insert(whereToInsert,modifiedSound);
        // change data too
        _data.removeAt(whereToInsert);
        _data.insert(whereToInsert,tempList);
        //we need to update model accordingly
        //_model->removeRow(whereToInsert);
        //_model->insertRow(whereToInsert,_data.at(whereToInsert));
        // updating the shortcuts table
        _keySequence.removeAt(whereToInsert);
        _keySequence.insert(whereToInsert,modifiedSound->getKeySequence());
        // updating k   eyscancode table
        _keyVirtualKey.removeAt(whereToInsert);
        _keyVirtualKey.insert(whereToInsert,modifiedSound->getShortcutVirtualKey());

    }
    if (generationMode == LIDL::Shortcut::GENERATE)
        this->GenerateGlobalShortcuts();

    // we resize
    this->resultView->resizeRowsToContents();
    this->resultView->clearSelection();
    // this->resultView->setWordWrap(false);
    //this->resultView->setTextElideMode(Qt::ElideLeft);
    this->refreshView();
}

// Sort of delegate. Re-implemented here cause i don't know how the fuck
// to do it in the delegate.
void SoundboardMainUI::refreshView()
{
    // re-fetching data
    // data will always contain all the data available
    // [0]: Song names
    // [1]: Number of remote sound files
    // [2]: SFXs
    // [3]: Shortcut
    // [4]: Playback
    // DATA of the model:
    // QVector<QList< QStandardItem* >> _data;

    _displayedData.clear();
    for (auto i: _data)
    {
        QList<QStandardItem*> item;
        for (auto &j: i)
        {
            item.append(new QStandardItem(j->text()));
            item.last()->setEditable(false);
        }

        // Editing the text so that it warps "properly"
        QFontMetrics fm = resultView->fontMetrics();
        int width = fm.width(item.at(0)->text());
        //        qDebug() << "Size of text is: " << width;
        //        qDebug() << "Size of area is:" << resultView->columnWidth(0);
        // if the text is too big for the view, we wrap it or we only show the number depending
        // on the option provided
        if (width > resultView->columnWidth(0))
        {
            QString tempText = item.at(0)->text();

            // If we DONT wanna show the full list
            // we wrap the stuff
            if ( LIDL::SettingsController::GetInstance()->checkShowFlags(LIDL::SHOW_SETTINGS::WRAP_SONG_LIST))
            {
                QStringList tmpText = tempText.split("\n");
                QString tempAgain = tmpText.at(0);
                // if we have more than 1 song
                // we need to count the .. since we will add them.. POTENTIALLY POTENTIALLY
                if (tmpText.size() > 1 && fm.width(tmpText.at(0)) > resultView->columnWidth(0))
                {

                    tempAgain.append(".. + " + QString::number(tmpText.size() - 1)  + " more" ) ;
                    while( fm.width(tempAgain)  > resultView->columnWidth(0))
                    {
                        tempAgain.remove(tempAgain.size() - 12,1);
                    }
                    tempAgain.remove(tempAgain.size() - 12,1);
                    tempAgain.remove(tempAgain.size() - 12,1);
                }
                // if first item is lesser than the column but we have more item we need to remove the ..
                else if(tmpText.size() > 1 && fm.width(tmpText.at(0) + ".. + " + QString::number(tmpText.size() - 1)  + " more" )  < resultView->columnWidth(0))
                {
                    tempAgain.append(" + " + QString::number(tmpText.size() - 1)  + " more" ) ;
                }
                // else we are in between those two cases
                // but we still need to check that it has more than 1 sound
                else if (tmpText.size() > 1)
                {
                    tempAgain.append(".. + " + QString::number(tmpText.size() - 1)  + " more" );
                    while( fm.width(tempAgain)  > resultView->columnWidth(0))
                    {
                        tempAgain.remove(tempAgain.size() - 12,1);
                    }
                    tempAgain.remove(tempAgain.size() - 12,1);
                    tempAgain.remove(tempAgain.size() - 12,1);
                }
                item.removeAt(0);
                item.prepend(new QStandardItem(tempAgain));
                // if we only have one sound we show ..
                // actually we don't need to do it since Qt will do it by itself cause we set line maximum height
                item.at(0)->setToolTip( i.at(0)->text() );
            }



        } // end if text width > column width
        else // well if it's smaller than the col but contains more than one file we need to wrap it anyway
        {
            QString tempText = item.at(0)->text();
            QStringList tmpText = tempText.split("\n");
            if (tmpText.size() > 1)
            {
                item.removeAt(0);
                item.prepend(new QStandardItem( tmpText.first() + " + " + QString::number(tmpText.size() - 1)  + " more"   ));
            }

        }   // maybe this whole algorithm could use a revamp

        // If we do not want SFX to be shown we remove the entry from the list
        // i.e. if the flag isn't set:
        if (! LIDL::SettingsController::GetInstance()->checkShowFlags(LIDL::SHOW_SETTINGS::SHOW_SFX))
            item.removeAt(2);
        /* Else: we deal with multiline SFX. Which will always be the case if we have more than 1 sound
         * This else won't be called if the SFX are set to none for all the sounds in a collection
         * Because the QStringList doesn't contain \n */
        else if (item.at(1)->text().contains("\n"))
        {

            QStringList tempText = item.at(1)->text().split("\n");
            /* In case we ever support translations (pepeLaugh)
             we need to check if all the items are the same, if they are
             we can write EFFECT * number of sounds
             else we write several or something */

            auto sameStuff = [=](){
                for (auto i: tempText)
                    // if we find a single difference we return false, else there are none
                    if( i != tempText.at(0)  )
                        return false;
                return true;
            };

            if (sameStuff())
            {
                item.removeAt(1);
                item.insert(1,new QStandardItem(tempText.at(0) + "*" + QString::number(tempText.size()) ));
            }
            else
            {
                item.removeAt(1);
                item.insert(1,new QStandardItem(tr("≠")));
            }
        }


        _displayedData.append(item);
    } // end for auto i: data

    // resetting model with the new parameters
    _model->clear();

    // Setting headers
    QStringList tempZulul;
    tempZulul << tr("Sound Collections");
    tempZulul << tr("Remotes");
    if (LIDL::SettingsController::GetInstance()->checkShowFlags(LIDL::SHOW_SETTINGS::SHOW_SFX))
        tempZulul << tr("SFX");

    tempZulul  << tr("Shortcut") << tr("Mode");
    // qDebug() << tempZulul;

    _model->setHorizontalHeaderLabels(tempZulul);
    for (auto i: _displayedData )
    {
        for (int j = 0; j == i.size(); j++)
            static_cast<QStandardItem*>(i.at(j))->setEditable(false);
        _model->appendRow(i);
    }
    // if we don't want to show the full list, we force the size to be 22px
    if ( LIDL::SettingsController::GetInstance()->checkShowFlags(LIDL::SHOW_SETTINGS::WRAP_SONG_LIST))
        for (int i = 0; i < _displayedData.size(); ++i)
            resultView->setRowHeight(i,22);
    // else we resize
    else
        resultView->resizeRowsToContents();

    // We resize the SFX row if it exists
    if (LIDL::SettingsController::GetInstance()->checkShowFlags(LIDL::SHOW_SETTINGS::SHOW_SFX))
    {
        //  resultView->horizontalHeader()->setSectionResizeMode(2,QHeaderView::ResizeToContents);
        resultView->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);
    }

    // Centering SFX and playback mode
    // OMEGALUL NE LINERS
    for (auto i: _displayedData)
        for (int j = 0; j == i.size(); j++)
            if (j!=0 && j !=2)
                static_cast<QStandardItem*>(i.at(j))->setTextAlignment(Qt::AlignCenter);
    for (long i = 1; i < resultView->horizontalHeader()->count(); ++i)
    {
        resultView->horizontalHeader()->setSectionResizeMode(i,QHeaderView::ResizeToContents);
    }
}



//// open the dialog to edit sound
void SoundboardMainUI::editSoundDialog()
{
    this->setEnabled(false);
    //if lastSelectedRow is valid (ie we didn't delete this entry)
    if (this->lastSelectedRow <= this->_sounds.size())
    {
        //Send infos about all the devices and the push to talk key to hold
        // and the sound wrapper
        _propertiesWindow= new WrapperProperties(
                    this->_deviceListOutput->currentIndex(),
                    this->_deviceListVAC->currentIndex(),
                    this->_shortcutEditPTT->getScanCode(),
                    this->_shortcutEditPTT->getVirtualKey(),
                    this->_sounds.at(this->lastSelectedRow)  ,
                    this);
        _propertiesWindow->show();
    }
    // Connection of the done button to mainUI slots is dealt in the contructor
    // to account for edit or add mode
}
// deal with modified sound
void SoundboardMainUI::soundModified(SoundWrapper *modifiedSound)
{
    this->addSound(modifiedSound,lastSelectedRow);
}


void SoundboardMainUI::GenerateGlobalShortcuts()
{
    // _keySequence and _winShortcutHandle should always be same size
    if (_keySequence.size() != _winShorcutHandle.size())
    {
        this->statusBar()->showMessage("Error attempting to generate the shortcuts.");
        return;
    }
    //need to UNREGISTER ALL hotkeys and rebind them
    //https://msdn.microsoft.com/fr-fr/library/windows/desktop/ms646327(v=vs.85).aspx
    // qDebug() << "Unregistering all hotkeys";
    for (auto i: _winShorcutHandle)
        UnregisterHotKey(NULL,i);


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
            int tempFlags = 0;//
            //int tempFlags = MOD_NOREPEAT;
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

    this->statusBar()->showMessage("Shortcuts generated successfully.");


}


// Method to run the sound via hotkeys
void SoundboardMainUI::winHotKeyPressed(int handle)
{
    // qDebug() << "Pressed hotkey handle: " << handle;

    // If this is the STOP hotkey then we stop all sounds
    if (handle == 2147483647)
    {
        LIDL::SettingsController::GetInstance()->unHoldPTT();
        for (auto &i: _sounds)
            i->Stop();
    }
    // else this a sound hotkey handle
    // We check if the soundwrapper at this location has one file else it will play nullptr and crash
    else if ( ! _sounds.at(handle)->getSoundList().isEmpty() )
        _sounds.at(handle)->Play();
}


void SoundboardMainUI::setUpMenu()
{

    /***************************************************
                            FILE
    ****************************************************/

    //menuBar = new QMenuBar(this);
    QMenuBar * menuBar= this->menuBar();
    menuBar->setFixedHeight(20);
    //vLayout->addWidget(menuBar);
    QMenu * fileMenu = menuBar->addMenu(tr("File"));
    _actions.append(   new QAction("New",this)); //0
    _actions.append(   new QAction("Open",this)); //1


    _actions.append(   new QAction("Open EXP soundboard file",this)); //2

    _actions.append(   new QAction("Save",this)); //3
    _actions.append(   new QAction("Save as..",this)); //4
    fileMenu->addSeparator();
    _actions.append(   new QAction("Exit",this)); // 5

    fileMenu->addAction(_actions.at(0)); // new
    fileMenu->addSeparator();
    fileMenu->addAction(_actions.at(1)); // open
    _openRecentMenu = fileMenu->addMenu("Open Recent"); // open recent (duh)
    fileMenu->addSeparator();
    fileMenu->addAction(_actions.at(2)); // open exp

    fileMenu->addSeparator();
    fileMenu->addAction(_actions.at(3));

    fileMenu->addSeparator();
    fileMenu->addAction(_actions.at(4));
    fileMenu->addSeparator();
    fileMenu->addAction(_actions.at(5));

    QMenu* toolMenu = menuBar->addMenu(tr("Tools"));
    QMenu * viewMenu = menuBar->addMenu(tr("View"));


    /***************************************************
                            Help
    ****************************************************/
    QMenu * helpMenu = menuBar->addMenu(tr("Help"));
    _actions.append(   new QAction("Guide",this));  //6
    _actions.append(   new QAction("Check for update..",this)); //7
    _actions.append(   new QAction("Report a bug or request a feature",this)); //8
    _actions.append(   new QAction("About LIDL Soundboard",this)); //9
    _actions.append(   new QAction("Welcome message",this)); //10

    helpMenu->addAction(_actions.at(6));
    helpMenu->addAction(_actions.at(10));
    helpMenu->addSeparator();
    // helpMenu->addAction(_actions.at(7));
    helpMenu->addAction(_actions.at(8));
    helpMenu->addSeparator();
    helpMenu->addAction(_actions.at(9));


    /***************************************************
                            Tools
    ****************************************************/
    _actions.append(new QAction("Regenerate shortcuts",this));  //11);
    toolMenu->addAction(_actions.at(11));
    _actions.at(10)->setToolTip( ("Regenerate shortcuts in case they get glitched."));
    _actions.append(new QAction("Clear sounds shortcuts",this));  //12
    toolMenu->addAction(_actions.at(12));
    toolMenu->addSeparator();
    _actions.append(new QAction("Settings",this)); //13
    toolMenu->addAction(_actions.at(13));
    toolMenu->addSeparator();
    _actions.append(new QAction("Check for updates",this));
    toolMenu->addAction(_actions.at(14));

    /***************************************************
                              View
    ****************************************************/
    // sound collection menu
    QMenu * scMenu =  viewMenu->addMenu(tr("Sound Collection"));
    _actions.append(new QAction("Show SFX"));
    _actions.append(new QAction("Wrap sound list"));
    scMenu->addAction(_actions.at(15));
    scMenu->addAction(_actions.at(16));

    // SFX
    connect(_actions.at(15),QAction::triggered, [=]{
        // if the show flag is already there we invert it
        // and show the checkmark

        if  (LIDL::SettingsController::GetInstance()->checkShowFlags(LIDL::SHOW_SETTINGS::SHOW_SFX))
        {
            LIDL::SettingsController::GetInstance()->removeShowFlag(LIDL::SHOW_SETTINGS::SHOW_SFX);
            this->_actions.at(15)->setIcon(QIcon(""));
        }
        else // if it's not present we set it
        {
            LIDL::SettingsController::GetInstance()->addShowFlag(LIDL::SHOW_SETTINGS::SHOW_SFX);
            this->_actions.at(15)->setIcon(QIcon(":/icon/resources/checkmark.png"));
        }
        this->refreshView();
    });
    // SHOW FULL LIST OF SOUNDS OR (n)
    connect(_actions.at(16),QAction::triggered, [=]{
        // if the show flag is already there we invert it
        // and show the checkmark
        if  (LIDL::SettingsController::GetInstance()->checkShowFlags(LIDL::SHOW_SETTINGS::WRAP_SONG_LIST))
        {
            LIDL::SettingsController::GetInstance()->removeShowFlag(LIDL::SHOW_SETTINGS::WRAP_SONG_LIST);
            this->_actions.at(16)->setIcon(QIcon(""));
        }
        else // if it's not present we set it
        {
            LIDL::SettingsController::GetInstance()->addShowFlag(LIDL::SHOW_SETTINGS::WRAP_SONG_LIST);
            this->_actions.at(16)->setIcon(QIcon(":/icon/resources/checkmark.png"));
        }
        this->refreshView();
    });
    /***************************************************
                            ?
    ****************************************************/
    //    _actions.append(new QAction("?",this));
    //    menuBar->addAction(_actions.at(15));


    /***************************************************
                           CONNECTIONS
    ****************************************************/
    connect(this->_actions.at(4),SIGNAL(triggered()),this,SLOT(SaveAs()));
    connect(this->_actions.at(1),SIGNAL(triggered()),this,SLOT(OpenSlot()));
    // New action will call save soundboard state aswell, however we only want
    // to call it if it is done through the new button to avoid calling it two times when
    // opening a soundboard weSmart
    //  connect(this->_actions.at(0),SIGNAL(triggered()),this,SLOT(ClearAll()));
    connect(this->_actions.at(0),QAction::triggered,
            [=]{
        // Saving Soundboard state in the SettingsController object
        this->ClearAll();
        //emit SaveSoundboardState();
        this->SetStatusTextEditText(QString("Creating new empty soundboard"));
    });


    connect(this->_actions.at(5),SIGNAL(triggered()),this,SLOT(close()));
    connect(this->_actions.at(2),SIGNAL(triggered()),this,SLOT(OpenEXPSounboard()));
    connect(this->_actions.at(3),SIGNAL(triggered()),this,SLOT(Save()));
    connect(this->_actions.at(6),SIGNAL(triggered()),this,SLOT(HelpGuide()));
    // connect(this->_actions.at(7),SIGNAL(triggered()),this,SLOT(HelpCheckForUpdate()));
    connect(this->_actions.at(8),QAction::triggered,this,[=]{
         QDesktopServices::openUrl(QUrl(QString("https://github.com/devolution2409/Lidl-Soundboard/issues")));
    });


    connect(this->_actions.at(9),SIGNAL(triggered()),this,SLOT(HelpAbout()));
    connect(this->_actions.at(10), QAction::triggered, this, SoundboardMainUI::HelpShowFirstUserDialog);

    connect(this->_actions.at(11),SIGNAL(triggered()),this,SLOT(GenerateGlobalShortcuts()));
    connect(this->_actions.at(12),SIGNAL(triggered()),this,SLOT(ToolClearShortcut()));
    connect(this->_actions.at(13),SIGNAL(triggered()),LIDL::SettingsController::GetInstance(),SLOT(ShowSettingsWindow()));
    connect(this->_actions.at(14),QAction::triggered,this,SoundboardMainUI::CheckForUpdates);
}



//Reimplementing to kill all shortcuts
void SoundboardMainUI::closeEvent (QCloseEvent *event)
{

    // Compare saved soundboard state with the one we have now
    if ( LIDL::SettingsController::GetInstance()->CompareSaves( * this->GenerateSaveFile()) == 0)
        this->Save();
    else if ( LIDL::SettingsController::GetInstance()->CompareSaves( * this->GenerateSaveFile()) == 2)
         return;
    else
    {




    for (auto i: _winShorcutHandle)
        UnregisterHotKey(NULL,i);

    if (_updateScheduled)
    {
#ifdef QT_DEBUG
        QProcess::startDetached("C:/Program Files (x86)/LIDL Soundboard/SDKMaintenanceTool.exe",QStringList("--updater"));
        qDebug() << "path is set to Program Files SDK";
#endif
#ifndef QT_DEBUG
        QString path = qApp->applicationDirPath();
        path.append("/SDKMaintenanceTool.exe");
        bool success = QProcess::startDetached(path,QStringList("--updater"));
#endif
    }
    // send message to stop the listening loop L OMEGALUL OMEGALUL P
    PostQuitMessage(0);
    QWidget::closeEvent(event);

    }

}



void SoundboardMainUI::setStopShortcut(int virtualKey)
{
    //qDebug() << "virtual key" << virtualKey;
    UnregisterHotKey(NULL,2147483647);
    RegisterHotKey(NULL,2147483647,0, virtualKey);

}





//This thing will create a txt file for the soundboard to store *.lidljson locations
// so that they can be added in the c OMEGALUL mbo box
//void GenerateLidlLocations()
//{



//}






/***************************************************
                    FILE MENU SLOTS
****************************************************/


// CLEAR ALL aka New
void SoundboardMainUI::ClearAll()
{
    /***************************************************
                        SOUNDS
    ****************************************************/
    for (auto &i: this->_sounds)
    {
        i->Stop();
        delete i;
    }
    //    clearing just in case
    _sounds.clear();

    /***************************************************
                        SHORTCUTS
    ****************************************************/
    this->_keySequence.clear();
    this->_keyVirtualKey.clear();

    // Unregistering shortcuts
    for (auto i: _winShorcutHandle)
        UnregisterHotKey(NULL,i);
    // clearing the table
    _winShorcutHandle.clear();

    this->_shortcutEditPTT->clear();
    this->_shortcutEditStop->clear();
    // Unregistering stop hotkey
    UnregisterHotKey(NULL,2147483647);

    /***************************************************
                          MODEL
    ****************************************************/
    _model->clear();
    //resultView->setWordWrap(false);

    /***************************************************
                          DATA
    ****************************************************/
    // clearing data table as well
    // the objects SHOULD have been deleted when clearing the model
    //TODO: check khow to delete later those items, just in case forsenT
    this->_data.clear();
    this->_displayedData.clear();
    /***************************************************
                       COMBO BOXES
    ****************************************************/
    this->_deviceListOutput->setCurrentIndex(0);
    this->_deviceListVAC->setCurrentIndex(0);
    this->_saveName.clear();

    this->refreshView();
}

// Open slot
void SoundboardMainUI::OpenSlot()
{
    QString fileName = QFileDialog::getOpenFileName(this,tr("Open file"), LIDL::SettingsController::GetInstance()->GetDefaultSoundboardFolder() ,tr("LIDL JSON file(*.lidljson)"));
    this->Open(fileName);
    // forsenT
}

void SoundboardMainUI::Open(QString fileName)
{

    if (LIDL::SettingsController::GetInstance()->CompareSaves(* this->GenerateSaveFile()) == 0) //yes
        this->Save();
    else if  (LIDL::SettingsController::GetInstance()->CompareSaves(* this->GenerateSaveFile()) == 2) // cancel or X button
        return;
    else if  (LIDL::SettingsController::GetInstance()->CompareSaves(* this->GenerateSaveFile()) == 1 ||
              LIDL::SettingsController::GetInstance()->CompareSaves(* this->GenerateSaveFile()) == -1 ) //no or file up to date
    {
        QFile file(fileName);
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)   )
        {
            // Declare the temp variables we are going to use to construct our objects
            QString mainOutputDevice, vacOutputDevice;
            QString pttName;
            int pttScanCode=-1, pttVirtualKey =-1;
            QString stopName; int stopVirtualKey =-1;
            QString jsonAsString = file.readAll();

            //QJsonParseError error;
            QJsonDocument cdOMEGALUL = QJsonDocument::fromJson(jsonAsString.toUtf8());
            if (cdOMEGALUL.isNull())
            {
                file.close();
                QString errorMsg( "\""  +  fileName +  "\" isn't a valid .lidljson file.");
                this->statusBar()->showMessage(errorMsg);
                return;
            }

        _model->clear();
        _data.clear();
        this->ClearAll();

        this->_saveName = fileName;
        emit lidlJsonDetected(QFileInfo(file)); // forsenBee
        file.close();
        QJsonObject json = cdOMEGALUL.object();

        //  QVector<SoundWrapper*> sounds;
        // if it has a setting block we read it
        if (json.contains("Settings"))
        {
            QJsonObject settings = json.value("Settings").toObject();
            if (settings.contains("Main Output Device"))
                mainOutputDevice = settings.value("Main Output Device").toString();
            if (settings.contains("VAC Output Device"))
                vacOutputDevice = settings.value("VAC Output Device").toString();
            if (settings.contains("Push To Talk Key"))
            {
                QJsonObject settingsPTT = settings.value("Push To Talk Key").toObject();
                if (settingsPTT.contains("Key Name"))
                    pttName = settingsPTT.value("Key Name").toString();
                if (settingsPTT.contains("ScanCode"))
                    pttScanCode = settingsPTT.value("ScanCode").toInt();
                if (settingsPTT.contains("VirtualKey"))
                    pttVirtualKey = settingsPTT.value("VirtualKey").toInt();
            }
            if (settings.contains("Stop Sound Key"))
            {
                QJsonObject settingsStop = settings.value("Stop Sound Key").toObject();
                if (settingsStop.contains("Key Name"))
                    stopName = settingsStop.value("Key Name").toString();
                if (settingsStop.contains("VirtualKey"))
                    stopVirtualKey = settingsStop.value("VirtualKey").toInt();
            }
            if (settings.contains("Show Flags"))
            {
                LIDL::SHOW_SETTINGS flags = static_cast<LIDL::SHOW_SETTINGS>(settings.value("Show Flags").toInt());

                if (flags & LIDL::SHOW_SETTINGS::SHOW_SFX){

                    LIDL::SettingsController::GetInstance()->addShowFlag(LIDL::SHOW_SETTINGS::SHOW_SFX);
                    this->_actions.at(15)->setIcon(QIcon(":/icon/resources/checkmark.png"));
                }
                if (flags &  LIDL::SHOW_SETTINGS::WRAP_SONG_LIST){
                    LIDL::SettingsController::GetInstance()->addShowFlag(LIDL::SHOW_SETTINGS::WRAP_SONG_LIST);
                    this->_actions.at(16)->setIcon(QIcon(":/icon/resources/checkmark.png"));
                }
            }
        }// end if it contains settings

        /***************************************************
                          SETTING COMBO BOX
        ****************************************************/

        // we have to set the devices before
        this->_deviceListOutput->setCurrentIndex(this->_deviceListOutput->findData(mainOutputDevice, Qt::DisplayRole));
        this->_deviceListVAC->setCurrentIndex(this->_deviceListVAC->findData(vacOutputDevice,Qt::DisplayRole));



        /***************************************************
                 SETTING LOCALL DEVICES INDEX TO -1
                 just kidding we don't need to
        ****************************************************/
        int indexMainOutputDevice = this->_deviceListOutput->findData(mainOutputDevice, Qt::DisplayRole);// == 0) ? -1 : (this->_deviceListOutput->findData(mainOutputDevice, Qt::DisplayRole)) ;
        int indexVACOutputDevice  = this->_deviceListOutput->findData(vacOutputDevice, Qt::DisplayRole);//  == 0)? -1 : (this->_deviceListOutput->findData(vacOutputDevice, Qt::DisplayRole));
        /***************************************************
                          SETTING KEY SEQUENCES
        ****************************************************/
        this->_shortcutEditPTT->setKeySequence(QKeySequence(pttName));
        // need to update VK and SC else it will be -1 if we save right away
        this->_shortcutEditPTT->setScanCode(pttScanCode);
        this->_shortcutEditPTT->setVirtualKey(pttVirtualKey);

        this->_shortcutEditStop->setKeySequence(QKeySequence(stopName));
        this->_shortcutEditStop->setVirtualKey(stopVirtualKey);
        this->setStopShortcut(stopVirtualKey);
        // need to update the VirtualKey aswell in case we save afterwards, else it will be -1

        // The wrapper stuff



        if (json.contains("SoundWrappers"))
        {


            //progressWidget->move(0,this->resultView->size().height() - progressWidget->height() + 20);
            QJsonArray wrappersArray = json.value("SoundWrappers").toArray();

            // we iterate over wrappers
            for (auto i:wrappersArray)
            {
                QJsonObject item = i.toObject();
                LIDL::Playback playbackmode;
                QString shortcutString;
                int shortcutVirtualKey;
                QVector<LIDL::SoundFile*> fileArray;
                // Playback
                if (item.contains("Playback Mode"))
                {
                    playbackmode = static_cast<LIDL::Playback>(item.value("Playback Mode").toInt());
                    //qDebug() << "playbackmode here:" << item.value("Playback Mode").toInt();
                }
                // Shortcut info
                if (item.contains("Shortcut"))
                {
                    QJsonObject shortcut = item.value("Shortcut").toObject();
                    if (shortcut.contains("Key"))
                        shortcutString = shortcut.value("Key").toString();
                    if (shortcut.contains("VirtualKey"))
                        shortcutVirtualKey = shortcut.value("VirtualKey").toInt();
                }

                // Sound collection
                if (item.contains("Sound Collection"))
                {
                    if (item.value("Sound Collection").isObject())
                    {
                        //qDebug() << "this is an object forsenE, so new file format forsenE";
                        QJsonObject soundCollection = item.value("Sound Collection").toObject();
                        // Have to use traditional iterators because auto doesn't allow to use key DansGame

                        for (QJsonObject::iterator it = soundCollection.begin(); it!= soundCollection.end(); it++)
                        {
                            QString fileName = it.key();
                            float mainVolume = 1.0;
                            float vacVolume = 1.0;
                            QJsonObject settings = it.value().toObject();
                            if (settings.contains("Main Volume"))
                                mainVolume = static_cast<float>(settings.value("Main Volume").toInt()/100.0);
                            if (settings.contains("VAC Volume"))
                                vacVolume  = static_cast<float>(settings.value("VAC Volume").toInt()/100.0);
                            // SFX
                            LIDL::SFX sfx;

                            if (settings.contains("SFX Flags"))
                                sfx.flags = static_cast<LIDL::SFX_TYPE>(settings.value("SFX Flags").toInt());

                            if (settings.contains("SFX"))
                            {
                                QJsonObject sfx_obj =  settings.value("SFX").toObject();
                                if (sfx_obj.contains("Distortion"))
                                {
                                    QJsonObject distObj = sfx_obj.value("Distortion").toObject();
                                    for (QJsonObject::iterator l = distObj.begin(); l!= distObj.end();l++)
                                    {
                                        if (l.key() == "Cutoff")
                                            sfx.distortion.fPreLowpassCutoff = static_cast<float>(l.value().toInt());
                                        if (l.key() =="EQBandwidth")
                                            sfx.distortion.fPostEQBandwidth = static_cast<float>(l.value().toInt());
                                        if (l.key() =="EQCenterFrequency")
                                            sfx.distortion.fPostEQCenterFrequency = static_cast<float>(l.value().toInt());
                                        if (l.key() =="Edge")
                                            sfx.distortion.fEdge = static_cast<float>(l.value().toInt());
                                        if (l.key() =="Gain")
                                            sfx.distortion.fGain=  static_cast<float>(l.value().toInt());
                                    }
                                }
                                if (sfx_obj.contains("Chorus"))
                                {
                                    QJsonObject chorusObj = sfx_obj.value("Chorus").toObject();
                                    for (QJsonObject::iterator l = chorusObj.begin(); l!= chorusObj.end();l++)
                                    {
                                        if (l.key() == "Delay")
                                            sfx.chorus.fDelay = static_cast<float>(l.value().toInt());
                                        if (l.key() == "Depth")
                                            sfx.chorus.fDepth = static_cast<float>(l.value().toInt());
                                        if (l.key() == "Feedback")
                                            sfx.chorus.fFeedback = static_cast<float>(l.value().toInt());
                                        if (l.key() == "Frequency")
                                            sfx.chorus.fFrequency = static_cast<float>(l.value().toInt());
                                        if (l.key() == "WetDryMix")
                                            sfx.chorus.fWetDryMix = static_cast<float>(l.value().toInt());
                                        if (l.key() == "Phase")
                                            sfx.chorus.lPhase = static_cast<int>(l.value().toInt());
                                        if (l.key() == "Waveform")
                                            sfx.chorus.lWaveform =  static_cast<int>(l.value().toInt());
                                    }

                                }
                            }
                            else
                            {
                                // set default values here
                                //sfx.distortionEnabled = false;
                                sfx.distortion.fGain =  -18;
                                sfx.distortion.fEdge = 15;
                                sfx.distortion.fPostEQCenterFrequency = 2400;
                                sfx.distortion.fPostEQBandwidth = 2400;
                                sfx.distortion.fPreLowpassCutoff = 8000;

                            }
                            fileArray.append(new LIDL::SoundFile(fileName,
                                                                 mainVolume,
                                                                 vacVolume,sfx));

                        }
                    }

                    //Else If this is an array than we are on the old save system without the volumes:
                    else if (item.value("Sound Collection").isArray() )
                    {
                        //qDebug() << "Old file detected forsenBee";
                        QJsonArray soundArray = item.value("Sound Collection").toArray();
                        // We iterate over the sound files and add them to the array
                        // (default volume is 100%).
                        for (auto j: soundArray)
                            fileArray.append( new LIDL::SoundFile(j.toString())  );

                    }


                }
                //                    qDebug() << "Wrapper: \n\tPlayback Mode:" << playbackmode
                //                             << "\n\tShortcut String:" << shortcutString
                //                             << "\n\tVirtualKey:"      << shortcutVirtualKey
                //                             << "\n\tSoundlist:";
                //                    for (auto j: fileArray)
                //                        qDebug() << j;


                /***************************************************
                                   CREATING THE WRAPPERS
                    ****************************************************/
                //                    qDebug() << this->_deviceListOutput->findData(mainOutputDevice, Qt::DisplayRole);

                this->addSound(new SoundWrapper(fileArray,
                                                playbackmode,
                                                QKeySequence(shortcutString),
                                                shortcutVirtualKey,
                                                indexMainOutputDevice,
                                                indexVACOutputDevice,
                                                nullptr));


            } // end for auto wrapper
        } // end if json contains wrapper
        //qDebug() << mainOutputDevice << "\t" << vacOutputDevice << "\t" << pttName << "\t" << pttScanCode << "\t" << pttVirtualKey << "\t" << stopName << "\t" << stopVirtualKey;
        // Saving Soundboard state in the SettingsController object
        //qDebug() << "Keys here: " << _shortcutEditStop->getText() << "vk:" << _shortcutEditStop->getVirtualKey();
        emit SaveSoundboardState();
        }//endif file was opened

    }
}
// Open EXP
void SoundboardMainUI::OpenEXPSounboard()
{
    QString fileName = QFileDialog::getOpenFileName(this,tr("Open file"),"",tr("EXP Sounboard JSON  (*.json)"));
    QFile file(fileName);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)   )
    {   // We clear the soundboard
        this->ClearAll();
        QString jsonAsString = file.readAll();
        file.close();
        QJsonDocument cdOMEGALUL = QJsonDocument::fromJson(jsonAsString.toUtf8());
        QJsonObject json = cdOMEGALUL.object();


        if (json.contains("soundboardEntries"))
        {

            QJsonArray soundArray = json.value("soundboardEntries").toArray();
            for (auto i: soundArray)
            {
                QString fileName="";
                //int modifiers=0;
                //QString sequenceString;
                //int sequenceVK;

                QJsonObject singleSound = i.toObject();
                if (singleSound.contains("file"))
                    fileName = singleSound.value("file").toString();
                fileName.replace(QString("\\"), QString("/"));
                qDebug() << fileName;


                QVector<QString> fileList;
                fileList.append(fileName);
                // Calling the constructor designed for exp jsons (V)
                this->addSound(new SoundWrapper(fileList,
                                                LIDL::Playback::Singleton,
                                                LIDL::SettingsController::GetInstance()->GetDefaultMainVolume(),
                                                LIDL::SettingsController::GetInstance()->GetDefaultVacVolume(),
                                                this->_deviceListOutput->currentIndex(),
                                                this->_deviceListVAC->currentIndex()));


            } //end for sound array
        }
    }
}



// This function will generate the save file used by the save and SaveAs slots
QJsonObject * SoundboardMainUI::GenerateSaveFile()
{
    // creating the new JSon object
    QJsonObject * save = new QJsonObject();

    // Storing settings
    QJsonObject  settings;
    settings.insert("Main Output Device", this->_deviceListOutput->currentText());
    settings.insert("VAC Output Device",  this->_deviceListVAC->currentText());
    settings.insert("Show Flags", static_cast<int>(LIDL::SettingsController::GetInstance()->getShowFlags()));


    QJsonObject pttKey;
    pttKey.insert("Key Name",this->_shortcutEditPTT->getText());
    pttKey.insert("VirtualKey" ,this->_shortcutEditPTT->getVirtualKey());
    pttKey.insert("ScanCode"   ,this->_shortcutEditPTT->getScanCode());
    settings.insert("Push To Talk Key",pttKey);

    QJsonObject stopSoundKey;
    stopSoundKey.insert("Key Name",this->_shortcutEditStop->getText());
    stopSoundKey.insert("VirtualKey" ,this->_shortcutEditStop->getVirtualKey());
    settings.insert("Stop Sound Key",stopSoundKey);

    save->insert("Settings",settings);

    QJsonArray sounds;
    for (auto &i: _sounds)
    {
        // creating temp sound collection
        QJsonObject tempSound;
        tempSound.insert("Playback Mode",static_cast<int>(i->getPlayMode()));
        // qDebug() << i->getPlayMode();
        QJsonObject key;
        key.insert("Key",i->getKeySequence().toString());
        key.insert("VirtualKey", i->getShortcutVirtualKey());
        tempSound.insert("Shortcut",key);
        // The sound collection
        QJsonObject soundCollection;
        // Declaring temps sound collection object
        QVector<LIDL::SoundFile*> soundList = i->getSoundList();
        for (auto &j: soundList)
        {
            QJsonObject properties;
            properties.insert("Main Volume",static_cast<int>(j->getMainVolume() *100));
            properties.insert("VAC Volume" ,static_cast<int>(j->getVacVolume() *100));
            properties.insert("SFX Flags", static_cast<int>(j->getSFX().flags));
            QJsonObject soundEffects;

            //             if (j->getSFX().flags & LIDL::SFX_TYPE::DISTORTION)
            //             {
            QJsonObject distortion;
            BASS_DX8_DISTORTION tempDist;
            // check for trash values
            tempDist.fGain = static_cast<int>(j->getSFX().distortion.fGain);
            tempDist.fEdge = static_cast<int>(j->getSFX().distortion.fEdge);
            tempDist.fPostEQCenterFrequency =static_cast<int>(j->getSFX().distortion.fPostEQCenterFrequency);

            tempDist.fPostEQBandwidth = static_cast<int>(j->getSFX().distortion.fPostEQBandwidth);
            tempDist.fPreLowpassCutoff = static_cast<int>(j->getSFX().distortion.fPreLowpassCutoff);

            distortion.insert("Gain", tempDist.fGain     );
            distortion.insert("Edge",tempDist.fEdge);
            distortion.insert("EQCenterFrequency",tempDist.fPostEQCenterFrequency);
            distortion.insert("EQBandwidth",tempDist.fPostEQBandwidth);
            distortion.insert("Cutoff",tempDist.fPreLowpassCutoff);
            soundEffects.insert("Distortion",distortion);
            //               }
            //               if (j->getSFX().flags & LIDL::SFX_TYPE::CHORUS)
            //               {
            QJsonObject chorus;
            chorus.insert("Delay",static_cast<int>(j->getSFX().chorus.fDelay));
            chorus.insert("Depth",static_cast<int>(j->getSFX().chorus.fDepth));
            chorus.insert("Feedback",static_cast<int>(j->getSFX().chorus.fFeedback));
            chorus.insert("Frequency",static_cast<int>(j->getSFX().chorus.fFrequency));
            chorus.insert("WetDryMix",static_cast<int>(j->getSFX().chorus.fWetDryMix));
            chorus.insert("Phase",static_cast<int>(j->getSFX().chorus.lPhase));
            chorus.insert("Waveform",static_cast<int>(j->getSFX().chorus.lWaveform));
            soundEffects.insert("Chorus",chorus);
            //               }

            properties.insert("SFX",soundEffects);
            soundCollection.insert(  j->url(), properties);

        }

        tempSound.insert("Sound Collection",soundCollection);
        sounds.append(tempSound);
    }
    save->insert("SoundWrappers",sounds);
    return save;
}

// Save
void SoundboardMainUI::Save()
{
    // if file doesn't exist we throw the save as prompt
    if (this->_saveName.isEmpty())
    {
        this->SaveAs();
        return;
    }
    // else we save on it
    else
    {
        QJsonObject *save = GenerateSaveFile();
        QJsonDocument *cdOMEGALUL = new QJsonDocument(*save);
        QString jsonString = cdOMEGALUL->toJson(QJsonDocument::Indented);
        QFile file(_saveName);
        if (!file.open(QIODevice::WriteOnly))
        {
            QMessageBox::information(this, tr("Unable to open file"), file.errorString());
            return;
        }
        else
        {
            QTextStream out(&file);
            out.setCodec("UTF-8");
            out << jsonString.toUtf8();
            file.close();
            // Saving Soundboard state in the SettingsController object
            emit SaveSoundboardState();
            this->SetStatusTextEditText("Succesfully saved file: " + _saveName);
        }

    }
}

// Save as
void SoundboardMainUI::SaveAs()
{
    /*QFileDialog::getSaveFileName(QWidget *parent = Q_NULLPTR,
                                const QString &caption = QString(),
                                const QString &dir = QString(),
                                const QString &filter = QString(),
                                QString *selectedFilter = Q_NULLPTR,
                                 Options options = Options())
*/
    QString fileName  = QFileDialog::getSaveFileName(this,
                                                     tr("Save Soundboard As.."),
                                                     LIDL::SettingsController::GetInstance()->GetDefaultSoundboardFolder() ,
                                                     tr("LIDL JSON file(*.lidljson)"));
    //  fileName.append(".lidljson");
    QJsonObject *save = GenerateSaveFile();
    QJsonDocument *doc = new QJsonDocument(*save);
    QString jsonString = doc->toJson(QJsonDocument::Indented);
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly))
    {

        QMessageBox::information(this, tr("Unable to open file"), file.errorString());
        return;
    }
    else
    {
        this->_saveName = fileName;
        QTextStream out(&file);
        out.setCodec("UTF-8");
        out << jsonString.toUtf8();
        // lidljson detected is used to add the file in the recent file thing
        emit lidlJsonDetected(QFileInfo(file));
        file.close();
        // Saving Soundboard state in the SettingsController object
        emit SaveSoundboardState();
        this->SetStatusTextEditText("Succesfully saved file: " + fileName);
    }
}





/***************************************************
                    HELP MENU SLOTS
****************************************************/

void SoundboardMainUI::HelpGuide()
{
    // prepare overlay widget

    //this->setGraphicsEffect( nullptr );

    QSize previousSize = this->size();
    this->resize(1170,632);
    _guideUI = new Ui::Guide();
    _guideWidget = new QWidget();
    Ui::Guide * ui = _guideUI;
    this->setGeometry(
                QStyle::alignedRect(
                    Qt::LeftToRight,
                    Qt::AlignCenter,
                    this->size(),
                    qApp->desktop()->availableGeometry()
                    ));
    // reseting selection
    resultView->clearSelection();
    //    this->setMaximumSize(1170,632);
    this->setMinimumSize(771,664);

    // _guideOverlay = new QWidget(this);

    //_guideOverlay->setSizePolicy(QSizePolicy::Maximum,QSizePolicy::Maximum);
    //qDebug() << "size is:" << resultView->width();
    //_guideOverlay->resize( this->size());




    // Getting the state of the widget and disablin
    // EVERY LAST OF THEM forsenC forsenGun
    QVector<bool> state;
    for (int i = 0; i < _gLayout->count(); ++i)
    {
        QWidget *widget = _gLayout->itemAt(i)->widget();
        if (widget != nullptr)
        {
            state.append(widget->isEnabled());
            widget->setEnabled(false);
        }
    }
    _gLayout->addWidget(_guideWidget,0,8,9,4);
    _gLayout->setColumnStretch(8,100);
    _guideUI->setupUi(_guideWidget);
    _guideWidget->show();
    _guideWidget->setMaximumWidth(515);
    this->_gLayout->update();
    ui->titleLabel->setText("LIDL Helper");
    ui->pageLabel->setText("1/14");

    QStringList helpText;
    helpText.reserve(14);
    helpText.append(tr("The Menu Bar is where you will find the following actions:<br>"
                       "File:<br>"
                       "     New<br>"
                       "     Open<br>"
                       "     Open Recent<br>"
                       "     Open EXP Soundboard<br>"
                       "     Save<br>"
                       "     Save As<br>"
                       "     Exit<br>"
                       "Tools:<br>"
                       "     Regenerate shortcuts<br>"
                       "     Clear sounds shortcuts<br>"
                       "     Settings<br>"
                       "     Check for updates<br>"
                       "Help:<br>"
                       "     Guide:<br>"
                       "     Welcome Message:<br>"
                       "     Report a bug or request a feature<br>"
                       "     About LIDL Soundboard<br>"
                       ));
    helpText.append(tr("<h1>Sound wrapper list</h1><br>"
                       "Each line represent a wrapper, you can see:<br>"
                       "Which sounds it contains on the first column.<br>"
                       "The number of remote files contained in the wrapper.<br>"
                       "The shortcut it is assigned to.<br> "
                       "The playback mode.<br>"
                       "The list of sound effects assigned to each sound in the wrapper.<br><br>"
                       "You can toggle some display option in the view window."
                       ));

    helpText.append(tr(	"<h1>Adding sounds</h1><br>"
                        "Opens the LIDL Soundboard Entry Editor, allowing you to add sound files to the sound collection. <br>"
                        "You can either drag and drop sound files at the window, <br>"
                        "or open the file browser to choose a file to import. <br><br>"
                        "In this window, you can also edit the volume of each sound file, <br>"
                        "as well as add additional special effects. <br><br>"
                        "The playback mode and the shortcut key sequence to play the sounds in the wrappers <br>"
                        "can also be selected here."
                        ));

    helpText.append(tr(	"<h1>Deleting sounds</h1><br>"
                        "Press this button to remove the selected song from the sound collection. :)"
                        ));

    helpText.append(tr(	"<h1>Edit sounds</h1><br>"
                        "Brings up a menu to edit the settings for the selected sound file. <br>"
                        "(Will open the LIDL Soundboard Entry Editor.)"
                        ));

    helpText.append(tr(	"<h1>Playing sounds</h1><br>"
                        "Press this button to play the selected sound file through the <br>"
                        "selected audio devices."
                        ));

    helpText.append(tr(	"<h1>Stop playback</h1><br>"
                        "This button cures cancer."
                        ));

    helpText.append(tr(	"<h1>First output device</h1><br>"
                        "Pick an audio device to output audio to.<br>"
                        "Select your primary playback device (so that you can hear the sound too)."
                        ));

    helpText.append(tr( "<h1>Virtual audio cable output</h1><br>"
                        "Select your virtual audio cable."
                        ));

    helpText.append(tr(	"<h1>Microphone injection</h1><br>"
                        "Opens Windows Sound menu to help set up microphone injection.<br>"
                        "First, open the properties for your microphone in the Recording tab. <br>"
                        "Then in the 'Listen' tab, tick the 'Listen to this device' checkbox, <br>"
                        "and select your virtual audio cable in the 'Playback through this device' menu.<br>"
                        "Finally, set the virtual audio cable as your default input device, whether in windows or in your game."
                        ));

    helpText.append(tr(	"<h1>Key to Auto-Hold (push to talk users)</h1><br>"
                        "Click on this box followed by the desired key to set up a push-to-talk key that the LIDL soundboard will auto-hold.<br>"
                        "When playing a file, whether through its shortcut or by clicking play, the soundboard will emulate both a physical (through Scan Code) and virtual (through Virtual Key) key press<br>"
                        "This way, you don't have to hold the push to talk key yourself when propagating ebola. <br>"
                        "Leave blank if you are using voice activation."
                        ));
    helpText.append(tr(	"<h1>Clear push-to-talk key</h1><br>"
                        "Clears the previously set push-to-talk key."
                        ));

    helpText.append(tr(	"<h1>Stop playback key</h1><br>"
                        "Click on this box followed by the desired key to set up a stop hotkey. <br>"
                        "When this hotkey is pressed, all currently playing sounds will be stopped."
                        ));


    helpText.append(tr(	"<h1>Clear stop playback key</h1><br>"
                        "Clears the previously set stop hotkey."
                        ));


    for (int i=0;i<14;i++)
        helpText.append("12");

    QString buttonCSS =  _btnAdd->styleSheet();
    QString comboCSS = _deviceListOutput->styleSheet();
    QString lineEditCSS = _shortcutEditPTT->styleSheet();
    QString highlightedCSS = "border: 2px solid QLinearGradient( x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #ffa02f, stop: 1 #d7801a);color: #b1b1b1;" ;
    QString menuCSS = this->menuBar()->styleSheet();
    QString viewCSS = this->resultView->styleSheet();
    ui->mainTextLabel->setText(helpText.first());
    connect(ui->btnNext, QPushButton::clicked, [=] {
        if (ui->pageSlider->value() < 14)
            ui->pageSlider->setValue( ui->pageSlider->value() + 1  );
    });
    connect(ui->btnPrevious, QPushButton::clicked, [=] {
        if (ui->pageSlider->value() > 1)
            ui->pageSlider->setValue( ui->pageSlider->value() - 1  );
    });


    // The close button
    connect(ui->okButton, QPushButton::clicked, [=] {
        _guideWidget->close();
        _gLayout->removeWidget(_guideWidget);
        delete _guideWidget;
        // resetting widgets to their previous states
        for (int i = 0; i < _gLayout->count(); ++i)
        {
            QWidget *widget = _gLayout->itemAt(i)->widget();
            if (widget != nullptr)
                widget->setEnabled(state.at(i));
        }
        this->resize(previousSize);
        _guideUI = new Ui::Guide();
        _guideWidget = new QWidget();
        Ui::Guide * ui = _guideUI;
        this->setGeometry(
                    QStyle::alignedRect(
                        Qt::LeftToRight,
                        Qt::AlignCenter,
                        this->size(),
                        qApp->desktop()->availableGeometry()
                        ));
        _gLayout->setColumnStretch(8,0);
        // resetting style
        this->menuBar()->setStyleSheet(menuCSS);
        _btnAdd->setStyleSheet(buttonCSS);
        _btnDelete->setStyleSheet(buttonCSS);
        _btnEdit->setStyleSheet(buttonCSS);
        _btnPlay->setStyleSheet(buttonCSS);
        _btnStop->setStyleSheet(buttonCSS);
        _deviceListOutput->setStyleSheet(comboCSS);
        _deviceListVAC->setStyleSheet(comboCSS);
        _btnMicInjection->setStyleSheet(buttonCSS);
        _shortcutEditPTT->setStyleSheet(lineEditCSS);
        _shortcutEditStop->setStyleSheet(lineEditCSS);
        _btnClearPTT->setStyleSheet(buttonCSS);
        _btnClearStop->setStyleSheet(buttonCSS);
        resultView->setStyleSheet(viewCSS);
    });

    // the lambda we connect the slider to forsenE
    connect(ui->pageSlider,QSlider::valueChanged, [=] (int value){
        if (value == 14)
        {
            ui->btnNext->setEnabled(false);
        }
        else if (value == 1)
        {
            ui->btnPrevious->setEnabled(false);
        }
        else
        {
            ui->btnNext->setEnabled(true);
            ui->btnPrevious->setEnabled(true);
        }
        ui->mainTextLabel->setText(helpText.at(value-1));
        ui->pageLabel->setText(QString::number(value) + "/14");
        // Reset every thing to its previous CSS
        this->menuBar()->setStyleSheet(menuCSS);
        _btnAdd->setStyleSheet(buttonCSS);
        _btnDelete->setStyleSheet(buttonCSS);
        _btnEdit->setStyleSheet(buttonCSS);
        _btnPlay->setStyleSheet(buttonCSS);
        _btnStop->setStyleSheet(buttonCSS);
        _deviceListOutput->setStyleSheet(comboCSS);
        _deviceListVAC->setStyleSheet(comboCSS);
        _btnMicInjection->setStyleSheet(buttonCSS);
        _shortcutEditPTT->setStyleSheet(lineEditCSS);
        _shortcutEditStop->setStyleSheet(lineEditCSS);
        _btnClearPTT->setStyleSheet(buttonCSS);
        _btnClearStop->setStyleSheet(buttonCSS);
        resultView->setStyleSheet(viewCSS);

        switch (value) {
        case 1:
            //this->menuBar()->setStyleSheet("QMenuBar:item { border-bottom: 2px solid QLinearGradient( x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #ffa02f, stop: 1 #d7801a);}" );
            break;
        case 2:
            resultView->setStyleSheet(highlightedCSS);
            break;
        case 3:
            _btnAdd->setStyleSheet(highlightedCSS);
            break;
        case 4:
            _btnDelete->setStyleSheet(highlightedCSS);
            break;
        case 5:
            _btnEdit->setStyleSheet(highlightedCSS);
            break;
        case 6:
            _btnPlay->setStyleSheet(highlightedCSS);
            break;
        case 7:
            _btnStop->setStyleSheet(highlightedCSS);
            break;
        case 8:
            _deviceListOutput->setStyleSheet(highlightedCSS);
            break;
        case 9:
            _deviceListVAC->setStyleSheet(highlightedCSS);
            break;
        case 10:
            _btnMicInjection->setStyleSheet(highlightedCSS);
            break;
        case 11:
            _shortcutEditPTT->setStyleSheet(highlightedCSS);
            break;
        case 12:
            _btnClearPTT->setStyleSheet(highlightedCSS);
            break;
        case 13:
            _shortcutEditStop->setStyleSheet(highlightedCSS);
            break;
        case 14:
            _btnClearStop->setStyleSheet(highlightedCSS);
            break;
        default:
            break;
        }
        //  connect(ui->okButton, QButton::clicked);


    });
    //  QDesktopServices::openUrl(QUrl(QString("https://github.com/devolution2409/Lidl-Soundboard/blob/master/Lidl_manual.pdf")));
}

// Just in case but we will set this shit to be non-resizable
void SoundboardMainUI::resizeEvent ( QResizeEvent * event )
{
    // if the widget exists
    //if (_guideOverlay != nullptr)
    //    _guideOverlay->resize(this->width()-_guideWidget->width(),this->height() - this->statusBar()->height());
    this->resultView->resizeRowsToContents();
    this->refreshView();
    event->accept();



}

void SoundboardMainUI::HelpAbout()
{
    QDialog * zulul = new QDialog(this);
    zulul->setFixedSize(485,254);
    zulul->setWindowIcon(QIcon(":/icon/resources/forsenAim.png"));
    zulul->setWindowTitle("About LIDL Soundboard");
    QGridLayout *layout = new QGridLayout(zulul);
    QLabel *text = new QLabel("LIDL Soundboard " + QString(VER_STRING));
    QFont f( "Arial", 24, QFont::Bold);
    text->setFont(f);
    QPixmap image(":/icon/resources/forsenAim.png");

    QLabel *imageLabel = new QLabel();
    imageLabel->setPixmap(image);

    QLabel *text2 = new QLabel("Based on Qt 5.10.1 (https://www.qt.io/) compiled with MinGW 5.3.0 32bit");
    QLabel *text3= new QLabel("With BASS 2.4.13.8 (http://www.un4seen.com/)" );
    QLabel *text4 = new QLabel("Built on: " + QString(__DATE__) + "at: " + QString(__TIME__));
    QLabel *gachiBASS = new QLabel();
    gachiBASS->setMaximumSize(32,32);
    gachiBASS->setScaledContents(true);
    QMovie* movie = new QMovie(":/icon/resources/gachiBASS.gif");
    gachiBASS->setMovie(movie);
    movie->start();
    movie->setSpeed(200);

    layout->addWidget(imageLabel,0,0,1,3);
    layout->addWidget(text,0,3,1,14);
    layout->addWidget(text2,1,0,1,17);
    layout->addWidget(text3,2,0,1,17);
    layout->addWidget(gachiBASS,2,8,1,2);
    layout->addWidget(text4,3,0,1,17);

    QLabel *text5 = new QLabel("Made with love, and for all the BAJS by Devolution");
    layout->addWidget(text5,4,0,1,12);

    QLabel *text6 = new QLabel("Remember to always stay transparent.");
    layout->addWidget(text6,5,0,1,5);

    QVector<QLabel *> forsenDVD;
    for (long i=0; i<12; i++)
    {
        forsenDVD.append(new QLabel());
        forsenDVD.last()->setPixmap(QPixmap(":/icon/resources/forsenCD.jpg"));
        forsenDVD.last()->setMaximumSize(16,16);
        forsenDVD.last()->setScaledContents(true);
        layout->addWidget(forsenDVD.last(),5,i+5,1,1);
    }


    zulul->show();
}



void SoundboardMainUI::HelpShowFirstUserDialog()
{
    QDialog * zulul = new QDialog(this);
    zulul->setFixedSize(492,312);
    zulul->setWindowIcon(QIcon(":/icon/resources/forsenAim.png"));
    zulul->setModal(true);
    zulul->setWindowTitle("Welcome to LIDL Soundboard " + QString(VER_STRING) + " !");
    QGridLayout *layout = new QGridLayout(zulul);
    QLabel *text = new QLabel(tr("New User? Here are some things to know before using lidl soundboard:\n"));
    QLabel *text2 = new QLabel(tr("• A shortcut will be captured by the soundboard and will NOT get transmitted to any other program.\n"
                                  "• A shortcut can use one, or any combination of those modifiers: CTRL, ALT, SHIFT.\n"
                                  "• A sound collection can contain one or several sound files, using one the following playback modes:\n"
                                  "\t ‣Singleton: a single sound file.\n"
                                  "\t ‣Singleton (Cancer): hold it to spam it.\n"
                                  "\t ‣Sequential: will play the next sound in the collection every time shortcut\n\t   or play button signal is received.\n"
                                  "\t ‣Sequential(auto): Same as sequential except it will play next sound automatically.\n"
                                  "• You can set default value for many things in Tools->Settings.\n"
                                  "This message can be found again in the Help->Welcome message."
                                  ));
    text->setWordWrap(true);
    text2->setWordWrap(true);
    QFont f( "Arial", 14, QFont::Bold);
    text->setFont(f);

    QLabel *gachiPls = new QLabel();
    gachiPls->setMaximumSize(32,32);
    gachiPls->setScaledContents(true);
    gachiPls->setMovie( new QMovie(":/icon/resources/GachiPls.gif"));
    gachiPls->movie()->start();


    QVector<QLabel *> polishing;
    for (long i=0; i<5; i++)
    {
        polishing.append(new QLabel());
        polishing.last()->setMovie(   new QMovie(":/icon/resources/GachiPls.gif") );
        polishing.last()->setMaximumSize(16,16);
        polishing.last()->setScaledContents(true);

    }


    layout->addWidget(text,0,0,1,12);
    layout->addWidget(text2,1,0,1,12);
    QLabel* labelLink = new QLabel(tr("Full patch note available on <a href=\"https://github.com/devolution2409/Lidl-Soundboard/releases\">git.</a>"));
    labelLink->setOpenExternalLinks(true);
    layout->addWidget(labelLink,2,0,1,12);
    int j = 0;
    for (auto &i: polishing)
    {
        i->movie()->start();
        layout->addWidget(i,3,j,1,1);
        layout->addWidget(new QLabel(tr("KEYLOGGER"),this),3,j+1,1,1);

        j+=2;
    }
    zulul->show();
}

void SoundboardMainUI::ToolClearShortcut()
{
    // Clearing KeySequence, else GenerateGlobalShortcut() will be able to pull info from there
    _keySequence.clear();
    for (auto i: _winShorcutHandle)
        UnregisterHotKey(NULL,i);
    // Clearing it too, else size  of keysequence and winshortcut handle wont match forsenE
    _winShorcutHandle.clear();
    QKeySequence emptySeq;

    QVector<SoundWrapper*> temp;
    for (auto &i: _sounds)
    {
        temp.append(new SoundWrapper(i->getSoundList(),i->getPlayMode(),emptySeq, -1,i->getMainDevice(),i->getVacDevice(),nullptr));
        delete i;
    }
    _sounds.clear();
    _model->clear();
    _model->setHorizontalHeaderLabels(
                (QStringList() << "Sound Collections"
                 << "SFX"
                 << "Shortcut"
                 << "Mode"));

    _data.clear();
    for (auto &i: temp)
        this->addSound(i,-1,LIDL::Shortcut::DONT_GENERATE);
    this->SetStatusTextEditText("Shortcuts cleared.");
}

void SoundboardMainUI::DealDragAndDrop(int newPlace)
{
    if (newPlace != -1)
        this->SwapWrappers(this->resultView->currentIndex().row(), newPlace );
}

void SoundboardMainUI::SwapWrappers(int firstRow, int secondRow)
{
    SoundWrapper * temp = _sounds.at(firstRow);

    SoundWrapper * firstPtr = new SoundWrapper(temp->getSoundList(),
                                               temp->getPlayMode(),
                                               temp->getKeySequence(),
                                               temp->getShortcutVirtualKey(),
                                               temp->getMainDevice(),
                                               temp->getVacDevice(),
                                               nullptr);


    temp = _sounds.at(secondRow);

    SoundWrapper * secondPtr = new SoundWrapper(temp->getSoundList(),
                                                temp->getPlayMode(),
                                                temp->getKeySequence(),
                                                temp->getShortcutVirtualKey(),
                                                temp->getMainDevice(),
                                                temp->getVacDevice(),
                                                nullptr);

    // Once we created those new items we can insert them (since it deletes the items we can't reuse them)
    this->addSound(firstPtr, secondRow);
    this->addSound(secondPtr, firstRow);
}



void SoundboardMainUI::SetStatusTextEditText(QString text)
{
    // Get size of text as pixels (width)
    QFontMetrics fm(this->statusBar()->font());
    int size = fm.width(text);
    int max  = this->statusBar()->width();
    if (!(text.isEmpty()))
    {
        // adding a . just in case
        if (text.at(text.size() -1) != ".")
            text += ".";
        if (size < max)
        {
            _statusEdit->setText(text);

        }
        else
        {
            _statusEdit->setText(text);
            // this->ScrollStatusText( max - size  );
        }
        // if the soundboard has been modified:
        QTimer::singleShot(1500, [=]
        {
            // if snapshot now is different from stored one
            if ( LIDL::SettingsController::GetInstance()->SaveIsDifferentFrom(*(this->GenerateSaveFile())))
            {
                if (_saveName.isEmpty() || _saveName.size() == 0)
                    _statusEdit->setText("Soundboard file not saved.");
                else
                    _statusEdit->setText("Soundboard file: " + this->_saveName   + " not saved."  );
            }
            else if (_saveName.isEmpty())
                _statusEdit->setText("Soundboard file not saved.");
            else //it is the same soundboard as before (ie it is saved)
                _statusEdit->setText("Soundboard file: \"" + this->_saveName  + "\".");
        });
    }
}


void SoundboardMainUI::SetUpRecentMenu()
{
    // Clearing actions
    _openRecentMenu->clear();
    QVector<QAction *> actions;
    // Creating actions (ie the names)
    int count = 0;
    for (auto i: LIDL::SettingsController::GetInstance()->GetRecentFiles() )
    {
        //qDebug() << "size of the array:" << LIDL::SettingsController::GetInstance()->GetRecentFiles().size();
        qDebug() << "LUL:" << i.fileName();
        actions.append(new QAction( i.fileName()));
        // using lambdas forsenE
        connect( actions.last(),
                 QAction::triggered,
                 [=] {
            this->Open( i.filePath() );
        });
        _openRecentMenu->addAction(actions.last());

        if (++count == LIDL::SettingsController::GetInstance()->GetRecentFilesCount())
            break;
    }
}


void SoundboardMainUI::CheckForUpdates()
{
    if (!_updateScheduled)
    {
        QProcess process;
#ifdef QT_DEBUG
        process.start("C:/Program Files (x86)/LIDL Soundboard/SDKMaintenanceTool.exe --checkupdates");
        qDebug() << "path is set to Program Files SDK";
#endif
#ifndef QT_DEBUG
        process.start(qApp->applicationDirPath() + "/SDKMaintenanceTool.exe --checkupdates");
#endif



        // Wait until the update tool is finished
        process.waitForFinished();

        connect(&process, &QProcess::errorOccurred, [=](QProcess::ProcessError error)
        {
            qDebug() << "Error while checking updates enum val = " << error;
        });

        // Read the output
        QByteArray data = process.readAllStandardOutput();

        qDebug() << data;
        // No output means no updates available
        // Note that the exit code will also be 1, but we don't use that
        // Also note that we should parse the output instead of just checking if it is empty if we want specific update info
        if(data.isEmpty())
        {
            QMessageBox msgBox;
            msgBox.setText(tr("Update Check Complete"));
            msgBox.setText(tr("No Updates Available."));
            msgBox.setIcon(QMessageBox::Information);
            msgBox.exec();
        }
        else
        {
            QMessageBox msgBox;
            msgBox.setWindowTitle("Update available");
            msgBox.setIcon(QMessageBox::Information);
            msgBox.setMinimumSize(300,75);
            msgBox.setText(tr("A New Update is available.\n"
                              "Do you wish to update?"));

            QPushButton *nowButton = msgBox.addButton(tr("Now"), QMessageBox::ActionRole);
            QPushButton *laterButton = msgBox.addButton(tr("On Exit"), QMessageBox::ActionRole);
            QPushButton *abortButton = msgBox.addButton(QMessageBox::Abort);

            msgBox.exec();
            QStringList args("--updater");
            if (msgBox.clickedButton() == nowButton)
            {
#ifdef QT_DEBUG
                bool success = QProcess::startDetached("C:/Program Files (x86)/LIDL Soundboard/SDKMaintenanceTool.exe", args);
                qDebug() << "path is set to Program Files SDK";
#endif
#ifndef QT_DEBUG
                bool success = QProcess::startDetached(qApp->applicationDirPath() + "/SDKMaintenanceTool.exe", args);
#endif
                _updateScheduled = false;
                qApp->closeAllWindows();
            }
            else if (msgBox.clickedButton() == laterButton)
            {
                _updateScheduled = true;
                emit this->SetStatusTextEditText(tr("Updater will run after LIDL Sounboard is closed."));
                _actions.at(14)->setText(tr("Cancel scheduled update"));
            }
        }
    }
    else
    {
        _updateScheduled = false;
        _actions.at(14)->setText(tr("Check for updates"));
        emit this->SetStatusTextEditText(tr("Update canceled."));
    }
}


void SoundboardMainUI::dragEnterEvent(QDragEnterEvent *e)
{
    if (e->mimeData()->hasUrls())
    {
        bool accept  = true;
        QList<QUrl> urlList = e->mimeData()->urls();

        QMimeDatabase db;

        for (auto i: urlList)
        {
          QMimeType type = db.mimeTypeForFile(i.path());
          qDebug() << type.name();
          if (!( LIDL::SettingsController::GetInstance()->GetSupportedMimeTypes().contains(type.name())))
            accept = false;
        }

        // check for lidljson file
        //qDebug() << e->mimeData()->urls().at(0).path();
        if (urlList.size() == 1 && e->mimeData()->urls().at(0).path().contains(".lidljson") )
            accept = true;


        if (accept)
            e->acceptProposedAction();
    }

}

void SoundboardMainUI::dropEvent(QDropEvent *e)
{
    // we can only have supported mime types or lidljson
    foreach (const QUrl &url, e->mimeData()->urls())
    {
        //QString fileName = url.toLocalFile();
        qDebug() << "Dropped file:" << url.toString();
        if (url.toString().contains(".lidljson"))
            this->Open(url.path().remove(0,1));
        else
        {
                // check anyway but it shouldn't be possible
              QMimeDatabase db;
              QMimeType type = db.mimeTypeForFile(url.toString());
              if (LIDL::SettingsController::GetInstance()->GetSupportedMimeTypes().contains(type.name()))
              {
                    // if the behaviour is to add one wrapper per sound
                    // OR we only have one sound
                    if (LIDL::SettingsController::GetInstance()->GetDragAndDropSeveralWrappers() ||  e->mimeData()->urls().size() == 1)
                    {
                        QVector<LIDL::SoundFile*> file;
                        file.append( new LIDL::SoundFile(url.toString(),
                                                         LIDL::SettingsController::GetInstance()->GetDefaultMainVolume(),
                                                         LIDL::SettingsController::GetInstance()->GetDefaultVacVolume()  ));
                        QKeySequence emptySeq;
                        // adding sound with empty shortcut and defaulting to singleton
                        this->addSound( new SoundWrapper(file,LIDL::Playback::Singleton,
                                                                 emptySeq, -1,
                                                                 this->_deviceListOutput->currentIndex(),
                                                                 this->_deviceListVAC->currentIndex()));

                    }
                    // if we have more than 1 sound and we want to add them  all in one thing
                    else if (LIDL::SettingsController::GetInstance()->GetDragAndDropSeveralWrappers()
                              &&  e->mimeData()->urls().size() > 1)
                    {
                        qDebug() << "test";
                    }
              }
        }
    }
}
