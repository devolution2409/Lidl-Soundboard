#include "soundboardMainUI.h"

SoundboardMainUI::SoundboardMainUI(QWidget *parent) : QMainWindow(parent)
{



    //set up hooks
    LIDL::Controller::HookController::GetInstance()->SetHooks();

    //    QFile css_dark(":/css/resources/darkorange.css");
    //    css_dark.open(QFile::ReadOnly);
    //    this->setStyleSheet(css_dark.readAll());
    //    this->menuBar()->setStyle(QStyleFactory::create("plastique"));
    //    css_dark.close();
    //   this->setWindowFlag( Qt::FramelessWindowHint);
    this->resize(514,741);
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
    // Headers will be set by the ClearAll() method which is called regardless<

    // Also works
    // _model->setHeaderData(0,Qt::Horizontal, QString("Sound File"));
    // _model->setHeaderData(1,Qt::Horizontal, tr("Shortcut"));

    // Creating the combox box and buttons for game detection


    //Creating the Viewer
    resultView = new CustomTableView(this);

    //Applying the 1 line 2 column _model
    resultView->setModel(_model);
    _gameSelector = new GameSelector();


    _gLayout->addWidget(_gameSelector,1,0,1,6);

    //connect(gameSelectorUi.comboBox,&QComboBox:);

    connect(_model,SIGNAL(draggedOnRow(int)),this,SLOT(DealDragAndDrop(int)));
    _gLayout->addWidget(resultView,2,0,1,6);
    _gLayout->setRowStretch(2,90);
    _gLayout->setColumnStretch(2,100);
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
    _gLayout->addWidget(_btnAdd,3,0,1,1);
    _gLayout->addWidget(_btnDelete,3,1,1,1);
    _gLayout->addWidget(_btnEdit,3,2,1,1);
    _gLayout->addWidget(_btnPlay,3,4,1,1);
    _gLayout->addWidget(_btnStop,3,5,1,1);

    // Adding label to vlayout
    _label1 = new QLabel("1st Output (e.g. your speakers)",this);
    _gLayout->addWidget(_label1,4,0,1,6);

    // Adding output list
    _deviceListOutput = new QComboBox(this);
    _gLayout->addWidget(_deviceListOutput,5,0,1,6 );
    // Second label
    _label2 = new QLabel("Virtual Audio Cable output (optional)",this);
    _gLayout->addWidget(_label2,6,0,1,6);

    // Combo box to select VAC
    _deviceListVAC = new QComboBox(this);
    _gLayout->addWidget(_deviceListVAC,7,0,1,6);

    /***************************************************
                        MIC INJECTION SECTION
      ****************************************************/
    _label3 = new QLabel("Setup microphone injection (optional)",this);
    _gLayout->addWidget(_label3,8,0,1,6);
    //_deviceListInjector = new QComboBox(this);
    _btnMicInjection = new QPushButton("Open sound configuration",this);
    _gLayout->addWidget(_btnMicInjection,9,0,1,6);

    connect(this->_btnMicInjection,&QPushButton::clicked,this, [=]{
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

    _gLayout->addWidget(_label4,10,0,1,3);
    _gLayout->addWidget(_shortcutEditPTT,10,4,1,1);
    _gLayout->addWidget(_btnClearPTT,10,5,1,1);

    connect(this->_btnClearPTT,&QPushButton::clicked,this,[=]{
            // Clearing the thing and setting the PTTScanCode and the PTTVirtualKey to -1
            _shortcutEditPTT->clear();
            LIDL::Controller::ProfileController::GetInstance()->GetActiveProfile()->SetPttScanCode(-1);
            LIDL::Controller::ProfileController::GetInstance()->GetActiveProfile()->SetPttVirtualKey(-1);
        });
    /***************************************************
                         STOP SOUND BIND
      ****************************************************/
    _label5 = new QLabel("Stop ALL sound shortcut:");
    _shortcutEditStop= new CustomShortcutEdit();
    _btnClearStop = new QPushButton("Clear");

    _gLayout->addWidget(_label5,11,0,1,3);
    _gLayout->addWidget(_shortcutEditStop,11,4,1,1);
    _gLayout->addWidget(_btnClearStop,11,5,1,1);

    connect(this->_btnClearStop,&QPushButton::clicked,this,[=]{
        UnregisterHotKey(nullptr,2147483647);
        _shortcutEditStop->clear();
    });

    connect(this->_shortcutEditStop, &CustomShortcutEdit::virtualKeyChanged,this,&SoundboardMainUI::setStopShortcut);
    // WE ALSO NEED THOSE BUTTONS TO SEND -1 when reset forsenT

     _btnRadialSettings = new QPushButton("Open Radial Menu Settings");
     _gLayout->addWidget(_btnRadialSettings,12,0,1,6);

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

    // Lamnda _btnAdd
    connect(this->_btnAdd, &QPushButton::clicked, this, [=]{
        this->setEnabled(false);

            _propertiesWindow = new WrapperProperties(
                    this->_deviceListOutput->currentIndex(),
                    this->_deviceListVAC->currentIndex(),
                    nullptr,
                    this);
        // Connection of the done button to mainUI slots is dealt in the contructor
        // to account for edit or add mode
        _propertiesWindow->show();
        connect(_propertiesWindow, &WrapperProperties::signalAddDone, this, [&](std::shared_ptr<SoundWrapper> sound){

            // member those shared_ptr are only destroyed when going out of scope.
            // the scope of the btnDone is still active so ref is +1
            qDebug() << "[215] use count in the lambda: " << sound.use_count();
            // lock() returns shared_ptr from weak ptr
            this->addSound(sound);
            this->setEnabled(true);
             qDebug() << "[215] use count end of lambda: " << sound.use_count();
          });
        connect(_propertiesWindow,&WrapperProperties::closed, this,[=] { this->setEnabled(true);});

    });
    // Lambda
    //connect(this->resultView,SIGNAL(clicked(QModelIndex)),this,SLOT(onCellClicked(QModelIndex)));

    connect(this->resultView, &CustomTableView::clicked, this, [&](QModelIndex index){
        // disconnect the play button
        disconnect(_btnPlay,nullptr,nullptr,nullptr);
        lastSelectedRow = index.row();
        this->_btnEdit->setEnabled(true);
        this->_btnDelete->setEnabled(true);
        this->_btnPlay->setEnabled(true);
        // qDebug() << "User clicked a cell on row number: " << index.row();
        // connect it to the selected cell
        // ONLY if soundlist isn't empty
        if ( ! _sounds.at(index.row())->getSoundList().isEmpty())
            connect(_btnPlay,&QPushButton::clicked,this,
                    [=]{

                qDebug() << "[255] Ref count before playing after connecting from simple click:" << _sounds.at(index.row()).use_count();

                qDebug() << "wtf am i looking at here:" << _sounds.at(index.row()).use_count();
                qDebug() << "playing sound: " << index.row() << "   " << &_sounds.at(index.row());

                _sounds.at(index.row())->Play();

            });


    });

    // Lambda
    connect(this->resultView, &CustomTableView::doubleClicked,this, [&](QModelIndex index){
        qDebug() << "DOUBLECLICKDETECTED";
        // but we update it regardless
        disconnect(_btnPlay,nullptr,nullptr,nullptr);
        lastSelectedRow = index.row();
        this->_btnEdit->setEnabled(true);
        this->_btnDelete->setEnabled(true);
        this->_btnPlay->setEnabled(true);
        if ( ! _sounds.at(index.row())->getSoundList().isEmpty())
            connect(_btnPlay,&QPushButton::clicked,this,
                    [=]{
                qDebug() << "[255] Ref count before playing after connecting from double click:" << _sounds.at(index.row()).use_count();
                _sounds.at(index.row())->Play();


            });
               //     SLOT(Play()));
        // And we call the edit sound dialog
        this->editSoundDialog();
    });

    connect(this->_btnEdit, SIGNAL(clicked()), this, SLOT(editSoundDialog()));
    connect(this->_btnDelete, &QPushButton::clicked, this, [&]{    // check if selected sound is inside the array
        if (this->lastSelectedRow <= this->_sounds.size())
        {
            this->SetStatusTextEditText("Deleted selected sound");


            // disconnect anything connected to the sound
            disconnect(_sounds.at(lastSelectedRow).get());
            qDebug() << "attempting to delete" << &_sounds.at(lastSelectedRow);

            std::weak_ptr<SoundWrapper> lul = _sounds.value(lastSelectedRow);
            qDebug() << "[263] use count before deletion of i.get():" << lul.use_count();


            qDebug() << "[_btnDelete Lambda] Now deleting sound from profile aswell";

            LIDL::Controller::ProfileController::GetInstance()->GetActiveProfile()->RemoveSoundFromSharedPtr(lul.lock()) ;


            //   this->_sounds.at(lastSelectedRow)->deleteLater();
            // testing unicity here then
            qDebug() << "what about unicity here?" << _sounds.value(lastSelectedRow).unique();
            _sounds.value(lastSelectedRow).reset();
            this->_sounds.removeAt(lastSelectedRow);
             qDebug() << "[263] use count after deletion of wrapper from main ui and profile:" << lul.use_count();
            this->_data.removeAt(lastSelectedRow);
            this->_model->removeRow(lastSelectedRow);
            // Unregistering the hotkey
            UnregisterHotKey(nullptr,_winShorcutHandle.at(lastSelectedRow));
            /* Deleting the associated handle and keysquence
             *  (we don't really need keysequence since
             * we use virtual keys now but oh well) */
            this->_winShorcutHandle.removeAt(lastSelectedRow);
            this->_keySequence.removeAt(lastSelectedRow);
            this->_keyVirtualKey.removeAt(lastSelectedRow);
            // qDebug() << "sounds size:" << _sounds.size() << "shortcut size" << _winShorcutHandle.size();

              qDebug() << "[_btnDelete Lambda] Use count after deletion: " << lul.use_count();
              qDebug() << "[_btnDelete Lambda] Is it expired?" << lul.expired();

            // disconnecting play btn
            disconnect(this->_btnPlay,nullptr,nullptr,nullptr);


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


    connect(this->resultView, &CustomTableView::enableButtons  ,this, [=]{
        _btnEdit->setEnabled(true);
        _btnDelete->setEnabled(true);});
    connect(this->resultView,&CustomTableView::disableButtons,this, [=]{
        _btnEdit->setEnabled(false);
        _btnDelete->setEnabled(false);
    });



    // We are required to do this trick else
    // the window isn't existing when the AddSound method attempts to resize columns

    this->setMinimumSize(400,600);
    this->setMaximumSize(16777215,16777215);

    /***************************************************
                   CONNECTING TO OPEN SOUNDBOARD
      ****************************************************/



    connect(LIDL::Controller::SettingsController::GetInstance(),
            &LIDL::Controller::SettingsController::RecentFilesChanged,
            this,
            &SoundboardMainUI::SetUpRecentMenu,
            Qt::QueuedConnection
            );

    connect(this,
            &SoundboardMainUI::SaveSoundboardState,
           LIDL::Controller::SaveController::GetInstance(),
            &LIDL::Controller::SaveController::SaveState);


    connect(LIDL::Controller::SettingsController::GetInstance(),
            &LIDL::Controller::SettingsController::SettingsChanged,
            this, &SoundboardMainUI::SetUpRecentMenu,Qt::QueuedConnection);



    connect(this->_shortcutEditPTT,&CustomShortcutEdit::scanCodeChanged, this, [=](int scancode){
        LIDL::Controller::ProfileController::GetInstance()->GetActiveProfile()->SetPttScanCode(scancode);
        LIDL::Controller::ProfileController::GetInstance()->GetActiveProfile()->SetPttKeySequence(this->_shortcutEditPTT->keySequence());
    });
    connect(this->_shortcutEditPTT,&CustomShortcutEdit::virtualKeyChanged, this, [=](int virtualkey){
        LIDL::Controller::ProfileController::GetInstance()->GetActiveProfile()->SetPttVirtualKey(virtualkey);
        LIDL::Controller::ProfileController::GetInstance()->GetActiveProfile()->SetPttKeySequence(this->_shortcutEditPTT->keySequence());
    });


    connect(this->_btnStop,&QPushButton::clicked, [=]{
        for (auto &i: _sounds)
            i->Stop();
        LIDL::Controller::SettingsController::GetInstance()->unHoldPTT();
    });

    //connecting the wrappper to the combo box for devices
    connect(this->_deviceListOutput,static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),this,
            [=](int newIndex)
            {
                for (auto &i: _sounds)
                    i->OutputDeviceChanged(newIndex);

                LIDL::Controller::SaveController::GetInstance()->SetMainOutputDevice( this->_deviceListOutput->currentText() );


    });
    connect(this->_deviceListVAC,static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),this,
            [=](int newIndex)
            {
                for (auto &i: _sounds)
                    i->VACDeviceChanged(newIndex);

                LIDL::Controller::SaveController::GetInstance()->SetVacOutputDevice(this->_deviceListVAC->currentText());
    });


    // QueudConnection so that it doesn't fucks up the view
    connect(this,&SoundboardMainUI::OnConstructionDone,this,&SoundboardMainUI::PostConstruction,Qt::QueuedConnection);




    connect(LIDL::Controller::ProfileController::GetInstance(), &LIDL::Controller::ProfileController::AddSoundsToMainUI,this,
            &SoundboardMainUI::ProfileSwitched);

    connect(_gameSelector, &GameSelector::RefreshWrappers, this,
            &SoundboardMainUI::ProfileSwitched);

    connect(LIDL::Controller::SaveController::GetInstance(),
            &LIDL::Controller::SaveController::Clear,
            this,
            &SoundboardMainUI::ClearAll);

    connect(LIDL::Controller::SaveController::GetInstance(),
            &LIDL::Controller::SaveController::SetDevices,
            this, [=] (QString main, QString vac){
        qDebug() << "gnnnn " << main << "   " << vac;
        if (main.isEmpty())
            this->_deviceListVAC->setCurrentText("<No device selected>");
        this->_deviceListVAC->setCurrentText( vac );
        this->_deviceListOutput->setCurrentText(main);

    });

    connect(LIDL::Controller::SaveController::GetInstance(),
               &LIDL::Controller::SaveController::SetStopShortcut,
            this, [=] (QKeySequence ks, int vk){
        this->_shortcutEditStop->setKeySequence(ks);
        this->_shortcutEditStop->setVirtualKey(vk);
        this->setStopShortcut(vk);
    });

    emit OnConstructionDone();

}



void SoundboardMainUI::ProfileSwitched(QVector<std::shared_ptr<SoundWrapper>> wrappers)
{
    qDebug() << "[SoundboardMainUI::ProfileSwitched]";
    QString mainOutput = this->_deviceListOutput->currentText();
    QString vacOutput = this->_deviceListVAC->currentText();

    QKeySequence stopSeq = this->_shortcutEditStop->keySequence();
    int stopSC = this->_shortcutEditStop->getScanCode();
    int stopVK = this->_shortcutEditStop->getVirtualKey();

    this->ClearAllSounds();

    if (wrappers.size() > 0)
    {
        qDebug() << "[SoundboardMainUI::ProfileSwitched] We coo";
        for (int i = 0; i < wrappers.size() - 1; i++)
        {
            this->addSound(wrappers.at(i),-1,false,false);
        }

        this->addSound(wrappers.last(), -1, true,true);


        //for (auto &i: wrappers)
            //this->addSound(i);
    }
    // setting the text of the QTextEdits here
    // Clear all will clear those

    // It will also unregister the stop button, and any wrappers and their shorcuts
    // It will however not touch the settings singleton FeelsOkayMan

    // the PTT comes from the profile
    this->_shortcutEditPTT->setKeySequence(LIDL::Controller::ProfileController::GetInstance()->GetActiveProfile()->GetPttKeySequence());
    this->_shortcutEditPTT->setScanCode(LIDL::Controller::ProfileController::GetInstance()->GetActiveProfile()->GetPttScanCode());
    this->_shortcutEditPTT->setVirtualKey(LIDL::Controller::ProfileController::GetInstance()->GetActiveProfile()->GetPttVirtualKey());

    // the stop one comes from the global config

    this->_shortcutEditStop->setKeySequence(stopSeq);
    this->_shortcutEditStop->setScanCode(stopSC);
    this->_shortcutEditStop->setVirtualKey(stopVK);
    this->setStopShortcut(static_cast<unsigned int>(stopVK));

    this->_deviceListVAC->setCurrentText( vacOutput );
    this->_deviceListOutput->setCurrentText(mainOutput);
}


void SoundboardMainUI::PostConstruction()
{
    this->ClearAll();
//    connect(LIDL::Controller::SaveController::GetInstance(),
//            &LIDL::Controller::SaveController::lidlJsonDetected,this, [=]{
//            this->show();
//    });
    // Take a snapshot right now to avoid lidl errors when loading soundboard automatically
    LIDL::Controller::SaveController::GetInstance()->SaveState();


    // Open the soundboard post-construction
    if (LIDL::Controller::SettingsController::GetInstance()->OpenSettings())
    {
        if(!(LIDL::Controller::SettingsController::GetInstance()->GetLastOpenedSoundboard().isEmpty()))
        {
            QTimer::singleShot(0, [=]{
                LIDL::Controller::SaveController::GetInstance()->OpenSaveFile(
                            LIDL::Controller::SettingsController::GetInstance()->GetLastOpenedSoundboard());
            });
        }

    }



    this->show();


    // If this is the first time the user uses soundboard
    if (LIDL::Controller::SettingsController::GetInstance()->IsThisFirstTimeUser())
        this->HelpShowFirstUserDialog();

/****************************** ZULOL ****************************/
    QString path = "C:/Windows/system32/WindowsPowerShell/v1.0/powershell.exe";
    //path = "cmd";
    QStringList commands;
    commands.append("-nologo");
    commands.append("/Command");


    commands.append(  R"~([Net.ServicePointManager]::SecurityProtocol = 'tls12','tls11','tls'; curl -H @{"Authorization"="bearer 7237a1a80c289981c31f07d53cbe9d31f2922577"} -Method POST -body '{"query":"{\n  repository(owner: \"devolution2409\", name: \"Lidl-Soundboard\") {\n    releases(last: 2) {\n      edges {\n        node {\n          description\n          isPrerelease\n          tag {\n            id\n            name\n          }\n        }\n      }\n    }\n  }\n}\n"}' https://api.github.com/graphql | Select -ExpandProperty Content ;exit)~"
                );

    QProcess *p = new QProcess();

    p->setReadChannel(QProcess::StandardOutput);
    connect(p, &QProcess::readyReadStandardOutput, this,
            [=](){

        qDebug().noquote() << p->readAllStandardOutput();

    });


    connect(p, &QProcess::readyReadStandardError,this,
            [=](){
        qDebug() << p->readAllStandardError();
    });
    connect(p, &QProcess::started, this , [=](){
           qDebug() << "istartedzulol";
    });

    p->start(path,commands);


}


void SoundboardMainUI::fetchDeviceList(QComboBox *comboBox, QAudio::Mode mode)
{
    // Adding nullptr device
    comboBox->addItem("<No device selected>",Qt::DisplayRole);

    //    for (auto &deviceInfo: QAudioDeviceInfo::availableDevices(mode))
    //        comboBox->addItem(deviceInfo.deviceName(),Qt::DisplayRole);
    // Modifying this so it is the same order as BASS library
    //http://www.un4seen.com/doc/#bass/BASS_GetDeviceInfo.html
    //device 0 is always the "no sound" device, so you should start at device 1 if you only want to list real output devices.

    if (mode == QAudio::AudioOutput)
    {
        BASS_DEVICEINFO info;
        for (unsigned long i=1; BASS_GetDeviceInfo(i, &info); i++)
            if (info.flags&BASS_DEVICE_ENABLED) // device is enabled
                comboBox->addItem(info.name, Qt::DisplayRole);
    }
    else if (mode == QAudio::AudioInput)
    {
        BASS_DEVICEINFO info;
        for (unsigned long n=0; BASS_RecordGetDeviceInfo(n, &info); n++)
            comboBox->addItem(info.name, Qt::DisplayRole);

    }

}

// Add a sound if whereToInsert isn't
void SoundboardMainUI::addSound(std::shared_ptr<SoundWrapper> modifiedSound, int whereToInsert, bool generateShortcuts, bool refreshView)
{

    qDebug() << "Bear in mind that the previous shared_ptr from the edit window isn't out of scope until the end of this block";
    qDebug() << "[SoundboardMainUI::addSound] ref count for modified sound before anything:" << modifiedSound.use_count();

    // connecting the status bar signal for unexistant files (reading json)
    connect(modifiedSound.get(),&SoundWrapper::UnexistantFile,this, [=]{
            this->SetStatusTextEditText("The files marked with ⚠️ aren't present on disk.");
        });

    // connecting the wrapper proxy signal for player NowPlaying
    connect(modifiedSound.get(),&SoundWrapper::NowPlaying,this,[=](QString name){
            this->SetStatusTextEditText("<b>Now playing: </b>\"" + name +"\"");
        });

    // connecting the wrapper proxy signal for player ErrorPlaying
    connect(modifiedSound.get(),&SoundWrapper::ErrorPlaying,this, [=](QString name){
            this->SetStatusTextEditText("<b>Error playing file: </b>\"" + name + "\"");
         });

    // connect the clear button to the clear shortcut slot
   //  connect(this->_actions.at(11),SIGNAL(triggered()),modifiedSound,SLOT(clearShorcut()));

    // needed for remotes files forsenE
    connect(modifiedSound.get(), &SoundWrapper::holdPTTProxy, [=] (int duration){
        LIDL::Controller::SettingsController::GetInstance()->holdPTT(duration);
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

        // we add it to the current profile
        qDebug() << "[SoundboardMainUI::addSound] Ref count before adding the sound wrapper to" <<LIDL::Controller::ProfileController::GetInstance()->GetActiveProfile()->GetName() << "profile" << modifiedSound.use_count();
        LIDL::Controller::ProfileController::GetInstance()->GetActiveProfile()->AddSound(modifiedSound);
        qDebug() << "[SoundboardMainUI::addSound] Ref count after adding the sound wrapper to" <<LIDL::Controller::ProfileController::GetInstance()->GetActiveProfile()->GetName() << "profile" << modifiedSound.use_count();


    }
    // else it was modified, need to swap  the previous item by the new, and than delete the item
    else
    {
        // we swap the sound in the current profile
        LIDL::Controller::ProfileController::GetInstance()->GetActiveProfile()->SwapSound(_sounds.at(whereToInsert),modifiedSound);

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


    if (generateShortcuts)
        this->GenerateGlobalShortcuts();
    if (refreshView)
    {
        // we resize
        this->resultView->resizeRowsToContents();
        this->resultView->clearSelection();
        // this->resultView->setWordWrap(false);
        //this->resultView->setTextElideMode(Qt::ElideLeft);
        this->refreshView();
    }

      qDebug() << "[429] AddSound: ref count for modified sound after:" << modifiedSound.use_count();
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
            item.append(new QStandardItem(j->text()));

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
            if ( LIDL::Controller::SettingsController::GetInstance()->checkShowFlags(LIDL::SHOW_SETTINGS::WRAP_SONG_LIST))
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
        if (! LIDL::Controller::SettingsController::GetInstance()->checkShowFlags(LIDL::SHOW_SETTINGS::SHOW_SFX))
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
    if (LIDL::Controller::SettingsController::GetInstance()->checkShowFlags(LIDL::SHOW_SETTINGS::SHOW_SFX))
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
    if ( LIDL::Controller::SettingsController::GetInstance()->checkShowFlags(LIDL::SHOW_SETTINGS::WRAP_SONG_LIST))
        for (int i = 0; i < _displayedData.size(); ++i)
            resultView->setRowHeight(i,22);
    // else we resize
    else
        resultView->resizeRowsToContents();

    // We resize the SFX row if it exists
    if (LIDL::Controller::SettingsController::GetInstance()->checkShowFlags(LIDL::SHOW_SETTINGS::SHOW_SFX))
    {
        //  resultView->horizontalHeader()->setSectionResizeMode(2,QHeaderView::ResizeToContents);
        resultView->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);
    }

    // Centering SFX and playback mode
    // OMEGALUL NE LINERS
    for (auto i: _displayedData)
        for (int j = 0; j == i.size(); j++)
            static_cast<QStandardItem*>(i.at(j))->setEditable(false);

    for (auto i: _displayedData)
        for (int j = 0; j == i.size(); j++)
            if (j!=0 && j !=2)
                static_cast<QStandardItem*>(i.at(j))->setTextAlignment(Qt::AlignCenter);
    for (long i = 1; i < resultView->horizontalHeader()->count(); ++i)
    {
        resultView->horizontalHeader()->setSectionResizeMode(i,QHeaderView::ResizeToContents);
    }
}



// NO lambda cause we call it twice :slight_smile:
void SoundboardMainUI::editSoundDialog()
{
    qDebug() << "mb make editSoundDialog constructor expected a shared_ptr instead of a wrapper pointer!";
    this->setEnabled(false);
    //if lastSelectedRow is valid (ie we didn't delete this entry)
    if (this->lastSelectedRow <= this->_sounds.size())
    {
        //Send infos about all the devices and the push to talk key to hold
        // and the sound wrapper
        _propertiesWindow= new WrapperProperties(
                    this->_deviceListOutput->currentIndex(),
                    this->_deviceListVAC->currentIndex(),
                    this->_sounds.at(this->lastSelectedRow).get()  ,
                    this);
        // Lambda to connect the edit signal to the addSound method
        connect(_propertiesWindow,&WrapperProperties::signalEditDone,this,[=](std::shared_ptr<SoundWrapper> modifiedSound){
            this->addSound(modifiedSound,lastSelectedRow);

        });
        connect(_propertiesWindow,&WrapperProperties::closed, this,[=] { this->setEnabled(true);});

        _propertiesWindow->show();


    }
    // Connection of the done button to mainUI slots is dealt in the contructor
    // to account for edit or add mode

    // NOT ANYMORE SoBayed. God bless lambdas.
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
        UnregisterHotKey(nullptr,i);


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
            unsigned int tempFlags = 0;//
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

            RegisterHotKey(nullptr, count,tempFlags, _keyVirtualKey.at(count) );
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
    // If we shouldn't process event, we return.
    //(a.k.a. bug fix for shortcut playing themselves if
    // they have the same shortcut as modifier + autohold PTT key.
    //qDebug() << "var:" << LIDL::Controller::SettingsController::GetInstance()->_eventProcessing;
    // should have commented this more thoroughly because i don't understand the logic now LULW
    if(!(LIDL::Controller::SettingsController::GetInstance()->getEventProcessing()))
    {
        qDebug() << "shouldn't process handle number:" << handle;
        return;
    }
    qDebug() << "Pressed hotkey handle: " << handle;


    // We want to allow the stop hotkey to be pressed, but not the others that would maybe play a sound


    // If this is the STOP hotkey then we stop all sounds
    if (handle == 2147483647)
    {
        LIDL::Controller::SettingsController::GetInstance()->unHoldPTT();
        // unHoldPTT also stops the timer :wave: forsenE
        for (auto &i: _sounds)
            i->Stop();
    }
    // else this a sound hotkey handle
    // We check if the soundwrapper at this location has one file else it will play nullptr and crash

    else if ( ! _sounds.value(handle)->getSoundList().isEmpty() && !LIDL::Controller::SettingsController::GetInstance()->isEditing())
    {
        qDebug() << "ref count here? pepeS" << _sounds.at(handle).use_count();
        _sounds.value(handle)->Play();

    }
}


void SoundboardMainUI::setUpMenu()
{

    /***************************************************
                            FILE
    ****************************************************/

    //menuBar = new QMenuBar(this);
    QMenuBar * menuBar= this->menuBar();
    menuBar->setMinimumHeight(30);
    //vLayout->addWidget(menuBar);
    QMenu * fileMenu = menuBar->addMenu(tr("File"));
    _actions.append(   new QAction(tr("New"),this)); //0
    // New action will call save soundboard state aswell, however we only want
    // to call it if it is done through the new button to avoid calling it two times when
    // opening a soundboard weSmart
    //  connect(this->_actions.at(0),SIGNAL(triggered()),this,SLOT(ClearAll()));
    connect(this->_actions.last(),&QAction::triggered,
            [=]{
        switch(LIDL::Controller::SaveController::GetInstance()->CheckAndPromptIfNeedsSaving())
        {
            case 0: this->Save(); break; // yes
            case 1: break; // no
            case 2: return; break; // cancel or x button

            case -1: break; // file up to date
        }
        // Saving Soundboard state in the SettingsController object
        this->ClearAll();
        //emit SaveSoundboardState();
        this->SetStatusTextEditText(QString(tr("Creating new empty soundboard")));
        emit SaveSoundboardState(); // == LIDL::Controller::SettingsController::GetInstance()->SaveSoundboardState(QJsonObject save);
    });

    _actions.append(   new QAction(tr("Open"),this)); //1
    connect(this->_actions.last(),&QAction::triggered,this,&SoundboardMainUI::OpenSlot);

    _actions.append(   new QAction(tr("Open EXP soundboard file"),this)); //2
    connect(this->_actions.last(),&QAction::triggered,this,&SoundboardMainUI::OpenEXPSounboard);

    _actions.append(   new QAction(tr("Save"),this)); //3
    connect(this->_actions.last(),&QAction::triggered,this,&SoundboardMainUI::Save);

    _actions.append(   new QAction(tr("Save as.."),this)); //4
    connect(this->_actions.last(),&QAction::triggered,this,&SoundboardMainUI::SaveAs);

    _actions.append(   new QAction(tr("Exit"),this)); // 5
    connect(this->_actions.last(),&QAction::triggered,this,&SoundboardMainUI::close);

    fileMenu->addSeparator();
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
    _actions.append(   new QAction(tr("Guide"),this));                                              //6
    connect(this->_actions.last(),&QAction::triggered,this,&SoundboardMainUI::HelpGuide);

    _actions.append(   new QAction(tr("Check for updates.."),this));                                //7
    connect(this->_actions.last(),&QAction::triggered,this,&SoundboardMainUI::CheckForUpdates);

    _actions.append(   new QAction(tr("About LIDL Soundboard"),this));                  //8
    connect(this->_actions.last(),&QAction::triggered,this,&SoundboardMainUI::HelpAbout);


    _actions.append(   new QAction(tr("Report a bug or request a feature"),this));                              //9
    connect(this->_actions.last(),&QAction::triggered,this,[=]{
         QDesktopServices::openUrl(QUrl(QString("https://github.com/devolution2409/Lidl-Soundboard/issues")));
    });

    _actions.append(   new QAction(tr("First user message"),this));                                 //10
    connect(this->_actions.last(),&QAction::triggered,this, &SoundboardMainUI::HelpShowFirstUserDialog);


    helpMenu->addAction(_actions.at(6));
    helpMenu->addAction(_actions.at(10));
    helpMenu->addSeparator();

    helpMenu->addAction(_actions.at(8));
    helpMenu->addSeparator();

    helpMenu->addAction(_actions.at(9));
    helpMenu->addAction(_actions.at(7));
    /***************************************************
                            Tools
    ****************************************************/
    _actions.append(new QAction(tr("Preset editor wizard.."),this));
    connect(this->_actions.last(),&QAction::triggered,this, [=]{
       PresetWizard * lul = new PresetWizard();
       lul->show();
    });
    toolMenu->addAction(_actions.last());
    toolMenu->addSeparator();

    _actions.append(new QAction(tr("Regenerate shortcuts"),this));  //11);
    toolMenu->addAction(_actions.last());
    connect(this->_actions.last(),SIGNAL(triggered()),this,SLOT(GenerateGlobalShortcuts()));


    _actions.at(10)->setToolTip( tr("Regenerate shortcuts in case they get glitched."));
    _actions.append(new QAction(tr("Clear sounds shortcuts"),this));  //12
    toolMenu->addAction(_actions.last());
    connect(this->_actions.last(),SIGNAL(triggered()),this,SLOT(ToolClearShortcut()));

    _actions.append(new QAction(tr("Refresh sound devices"),this)); // 13
    connect(_actions.last(), &QAction::triggered, [=]{
        // clearing the view
        this->_deviceListOutput->clear();
        this->_deviceListVAC->clear();
        // changing the wrapper playback device to 0 will be automatically done when the .clear() option happens since
        // they are all connected to the comboboxes weSmart

        // Fetching the devices
        this->fetchDeviceList(_deviceListOutput,QAudio::AudioOutput);
        this->fetchDeviceList(_deviceListVAC,QAudio::AudioOutput);
        });
    toolMenu->addAction(_actions.last());
    toolMenu->addSeparator();

    _actions.append(new QAction(tr("Settings"),this)); //14
    toolMenu->addAction(_actions.last());
    connect(this->_actions.last(),SIGNAL(triggered()),LIDL::Controller::SettingsController::GetInstance(),SLOT(ShowSettingsWindow()));
    _actions.last()->setIcon(QIcon(""));
    toolMenu->addSeparator();



    /***************************************************
                              View
    ****************************************************/
    // sound collection menu
    QMenu * scMenu =  viewMenu->addMenu(tr("Sound Collection"));

    _actions.append(new QAction(tr("Show SFX"))); // 15

    // needed else the lambda will go rogue when calling _actions.last()
    QAction* temp = _actions.last();
    // SFX
    connect(_actions.last(),&QAction::triggered, [=]{
        // if the show flag is already there we invert it
        // and show the checkmark
        if  (LIDL::Controller::SettingsController::GetInstance()->checkShowFlags(LIDL::SHOW_SETTINGS::SHOW_SFX))
        {
            LIDL::Controller::SettingsController::GetInstance()->removeShowFlag(LIDL::SHOW_SETTINGS::SHOW_SFX);
            temp->setIcon(QIcon(""));
        }
        else // if it's not present we set it
        {
            LIDL::Controller::SettingsController::GetInstance()->addShowFlag(LIDL::SHOW_SETTINGS::SHOW_SFX);
            temp->setIcon(QIcon(":/icon/resources/checkmark.png"));
        }
        this->refreshView();
    });
    scMenu->addAction(_actions.last());

    _actions.append(new QAction(tr("Wrap sound list"))); //17
    QAction *temp2 = _actions.last();
    // SHOW FULL LIST OF SOUNDS OR (n)
    connect(_actions.last(),&QAction::triggered, [=]{
        // if the show flag is already there we invert it
        // and show the checkmark
        if  (LIDL::Controller::SettingsController::GetInstance()->checkShowFlags(LIDL::SHOW_SETTINGS::WRAP_SONG_LIST))
        {
            LIDL::Controller::SettingsController::GetInstance()->removeShowFlag(LIDL::SHOW_SETTINGS::WRAP_SONG_LIST);
            temp2->setIcon(QIcon(""));
        }
        else // if it's not present we set it
        {
            LIDL::Controller::SettingsController::GetInstance()->addShowFlag(LIDL::SHOW_SETTINGS::WRAP_SONG_LIST);
            temp2->setIcon(QIcon(":/icon/resources/checkmark.png"));
        }
        this->refreshView();
    });

    scMenu->addAction(_actions.last());
}



//Reimplementing to kill all shortcuts
void SoundboardMainUI::closeEvent (QCloseEvent *event)
{
    //Silenced the QApplication missing for style
    // now we have a QPixmap must construuct a QGuiApplication error :feelsWeirdMan:
    //apparentely thats because of static QWidget
    LIDL::Controller::HookController::GetInstance()->UnSetHooks();


    // Compare saved soundboard state with the one we have now
    switch(LIDL::Controller::SaveController::GetInstance()->CheckAndPromptIfNeedsSaving())
    {
        case 0:
            break; // yes
        case 1:
            break; // no
        case 2:
            event->ignore(); // we cancel the close the event
            break; // cancel or x button
        case -1: break; // file up to date
    }

    // we generate the .lidlsettings file anyway
    LIDL::Controller::SettingsController::GetInstance()->SaveSettings();


    for (auto i: _winShorcutHandle)
        UnregisterHotKey(nullptr,i);

    // now deleting sounds from profile
    LIDL::Controller::ProfileController::GetInstance()->RemoveAllProfiles();


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
        Q_UNUSED(success)
#endif
    }
    // send message to stop the listening loop L OMEGALUL OMEGALUL P
    PostQuitMessage(0);
    QWidget::closeEvent(event);



}



void SoundboardMainUI::setStopShortcut(unsigned int virtualKey)
{

    //TODO refactor this shit pepeL
    // assigning it to save controller feelsweirdman
    LIDL::Controller::SaveController::GetInstance()->SetStopVirtualKey(virtualKey);
    LIDL::Controller::SaveController::GetInstance()->SetStopKeyName(
                this->_shortcutEditStop->getText());

    //qDebug() << "virtual key" << virtualKey;
    UnregisterHotKey(nullptr,2147483647);
    RegisterHotKey(nullptr,2147483647,0, virtualKey);

}



/***************************************************
                    FILE MENU SLOTS
****************************************************/
void SoundboardMainUI::ClearAll()
{
    //clear sounds
    this->ClearAllSounds();
    // clear profiles
    LIDL::Controller::ProfileController::GetInstance()->RemoveAllProfiles();

    // build default profile
    LIDL::Controller::ProfileController::GetInstance()->AddProfile( Profile::Builder().Build());

}


// CLEAR ALL sounds but wont remove profiles
// and ptt buttons forsenT
void SoundboardMainUI::ClearAllSounds()
{
    /***************************************************
                        SOUNDS
    ****************************************************/
    for (auto &i: this->_sounds)
    {
        i->Stop();
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
        UnregisterHotKey(nullptr,i);
    // clearing the table
    _winShorcutHandle.clear();

    this->_shortcutEditPTT->clear();
    this->_shortcutEditStop->clear();
    // Unregistering stop hotkey
    UnregisterHotKey(nullptr,2147483647);

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


    this->refreshView();
}

// Open slot
void SoundboardMainUI::OpenSlot()
{

    LIDL::Controller::SaveController::GetInstance()->OpenSaveFile();
    return;
    // forsenT
}

// Open EXP
void SoundboardMainUI::OpenEXPSounboard()
{
    switch(LIDL::Controller::SaveController::GetInstance()->CheckAndPromptIfNeedsSaving())
    {
    case 0: this->Save(); break; // yes
    case 1: break; // no
    case 2: return; break; // cancel or x button

    case -1: break; // file up to date
    }
    QString fileName = QFileDialog::getOpenFileName(this,tr("Open file"),"",tr("EXP Sounboard JSON  (*.json)"));
    QFile file(fileName);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)   )
    {
        QString jsonAsString = file.readAll();
        file.close();
        QJsonDocument cdOMEGALUL = QJsonDocument::fromJson(jsonAsString.toUtf8());
        QJsonObject json = cdOMEGALUL.object();


        if (json.contains("soundboardEntries"))
        {
            // we clear the soundboard only if it's a valid file forsenE
            // We clear the soundboard
            this->ClearAllSounds();

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
                this->addSound(std::make_shared<SoundWrapper>(fileList,
                                                LIDL::Playback::Sequential,
                                                //LIDL::Playback::Singleton // not a singleton anymore cause it's redundant with sequential with only one sound
                                                LIDL::Controller::SettingsController::GetInstance()->GetDefaultMainVolume(),
                                                LIDL::Controller::SettingsController::GetInstance()->GetDefaultVacVolume(),
                                                this->_deviceListOutput->currentIndex(),
                                                this->_deviceListVAC->currentIndex()));


            } //end for sound array
        }
        else
        {
            QMessageBox::warning(this,tr("Error"),tr("File \"%1\" is not a valid EXP Soundboard save.").arg(fileName));
            return;
        }
    }
    // in any case we set the profile now
    LIDL::Controller::ProfileController::GetInstance()->ManualGameConfigurationChanged("Default");
}



// This function will generate the save file used by the save and SaveAs slots
QJsonObject * SoundboardMainUI::GenerateSaveFile()
{
    // creating the new JSon object
    QJsonObject * save = new QJsonObject();
    save->insert("Version", VER_STRING  );

    // Storing settings
    QJsonObject  settings;
    settings.insert("Main Output Device", this->_deviceListOutput->currentText());
    settings.insert("VAC Output Device",  this->_deviceListVAC->currentText());
    settings.insert("Show Flags", static_cast<int>(LIDL::Controller::SettingsController::GetInstance()->getShowFlags()));


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
    int index = 0;
    for (auto &i: _sounds)
    {
        // creating temp sound collection
        QJsonObject tempSound;
        tempSound.insert("Index", index++); // We don't even use this, but for human readability it's cool
        tempSound.insert("Playback Mode",static_cast<int>(i->getPlayMode()));
        // qDebug() << i->getPlayMode();
        QJsonObject key;
        key.insert("Key",i->getKeySequence().toString());
        key.insert("VirtualKey", static_cast<int>(i->getShortcutVirtualKey()));
        tempSound.insert("Shortcut",key);
        // The sound collection
        QJsonObject soundCollection;
        // Declaring temps sound collection object
        QVector<LIDL::SoundFile*> soundList = i->getSoundList();
        int jIndex = 0;
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
            distortion.insert("Gain", static_cast<int>(j->getSFX().distortion.fGain));
            distortion.insert("Edge", static_cast<int>(j->getSFX().distortion.fEdge));
            distortion.insert("EQCenterFrequency",static_cast<int>(j->getSFX().distortion.fPostEQCenterFrequency));
            distortion.insert("EQBandwidth",static_cast<int>(j->getSFX().distortion.fPostEQBandwidth));
            distortion.insert("Cutoff",static_cast<int>(j->getSFX().distortion.fPreLowpassCutoff));
            soundEffects.insert("Distortion",distortion);

            QJsonObject chorus;
            chorus.insert("Delay",static_cast<int>(j->getSFX().chorus.fDelay));
            chorus.insert("Depth",static_cast<int>(j->getSFX().chorus.fDepth));
            chorus.insert("Feedback",static_cast<int>(j->getSFX().chorus.fFeedback));
            chorus.insert("Frequency",static_cast<int>(j->getSFX().chorus.fFrequency));
            chorus.insert("Wet Dry Mix",static_cast<int>(j->getSFX().chorus.fWetDryMix));
            chorus.insert("Phase",static_cast<int>(j->getSFX().chorus.lPhase));
            chorus.insert("Waveform",static_cast<int>(j->getSFX().chorus.lWaveform));
            soundEffects.insert("Chorus",chorus);

            QJsonObject echo;

            echo.insert("Feedback",static_cast<int>(j->getSFX().echo.fFeedback) );
            echo.insert("Left Delay",static_cast<int>(j->getSFX().echo.fLeftDelay) );
            echo.insert("Right Delay",static_cast<int>(j->getSFX().echo.fRightDelay) );
            echo.insert("Wet Dry Mix",static_cast<int>(j->getSFX().echo.fWetDryMix) );
            echo.insert("Swap",static_cast<int>(j->getSFX().echo.lPanDelay) );
            soundEffects.insert("Echo",echo);

            QJsonObject flanger;
            flanger.insert("Delay",static_cast<int>(j->getSFX().flanger.fDelay));
            flanger.insert("Depth",static_cast<int>(j->getSFX().flanger.fDepth));
            flanger.insert("Feedback",static_cast<int>(j->getSFX().flanger.fFeedback));
            flanger.insert("Frequency",static_cast<int>(j->getSFX().flanger.fFrequency));
            flanger.insert("Phase",static_cast<int>(j->getSFX().flanger.lPhase));
            flanger.insert("Waveform",static_cast<bool>(j->getSFX().flanger.lWaveform));
            soundEffects.insert("Flanger",flanger);


            QJsonObject compressor;
            compressor.insert("Attack",static_cast<int>(j->getSFX().compressor.fAttack));
            compressor.insert("Gain"  ,static_cast<int>(j->getSFX().compressor.fGain));
            compressor.insert("Predelay",static_cast<int>(j->getSFX().compressor.fPredelay));
            compressor.insert("Ratio",static_cast<int>(j->getSFX().compressor.fRatio));
            compressor.insert("Release",static_cast<int>(j->getSFX().compressor.fRelease));
            compressor.insert("Threshold",static_cast<int>(j->getSFX().compressor.fThreshold));
            soundEffects.insert("Compressor",compressor);

            QJsonObject gargle;
            gargle.insert("Rate",static_cast<int>(j->getSFX().gargle.dwRateHz));
            gargle.insert("Waveform",static_cast<bool>(j->getSFX().gargle.dwWaveShape));
            soundEffects.insert("Gargle",compressor);



            //soundCollection.insert(  j->url(), properties); // old wey
            // new way: (VER > 1.7.0.)
            QJsonObject numberedSound;
            properties.insert("SFX",soundEffects);
            numberedSound.insert( j->url(), properties);

            soundCollection.insert( QString::number(jIndex++), numberedSound );
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
    // if file doesn't exist controller will throw the SaveAs prompt,
    //else it will save to the same file
    LIDL::Controller::SaveController::GetInstance()->Save();
    return;

            //this->SetStatusTextEditText("Succesfully saved file: " + "blblblbl");

        // not needed cause it is already emitted once opened
        // emit lidlJsonDetected(QFileInfo(file)); // forsenBee


}

// Save as
void SoundboardMainUI::SaveAs()
{
//    LIDL::Controller::SaveController::GetInstance()-

    LIDL::Controller::SaveController::GetInstance()->SaveAs();

    return;

}





/***************************************************
                    HELP MENU SLOTS
****************************************************/

void SoundboardMainUI::HelpGuide()
{
    //if guide is already opened we return
    if ( _guideWidget != nullptr)
        return;

    bool wasMaximized = false;
    if (this->windowState() & Qt::WindowMaximized)
        wasMaximized = true;

    QSize previousSize = this->size();
    if (  previousSize.width() < 771)
        this->resize(1170,632);
    _guideUI = new Ui::Guide();
    _guideWidget = new QWidget();

    Ui::Guide * ui = _guideUI;

    // reseting selection
    resultView->clearSelection();
    //    this->setMaximumSize(1170,632);
    this->setMinimumSize(771,664);


    // Getting the state of the widget and disablin'
    // EVERY LAST OF THEM forsenC forsenGun
    QVector<bool> state;
    for (int i = 0; i < _gLayout->count(); ++i)
    {
        QWidget *widget = _gLayout->itemAt(i)->widget();
        if (widget != nullptr)
        {
            state.append(widget->isEnabled());
            //widget->setEnabled(false);
        }
    }
    // :point_down: I VON ZULUL
    _gLayout->addWidget(_guideWidget,0,8,9,1);
    _gLayout->setColumnStretch(8,0);
    _guideUI->setupUi(_guideWidget);
    _gLayout->setColumnStretch(0,100);
    _guideWidget->show();
    _guideWidget->setMaximumWidth(515);
    this->_gLayout->update();
    if (!wasMaximized)
        this->setGeometry(
                    QStyle::alignedRect(
                        Qt::LeftToRight,
                        Qt::AlignCenter,
                        this->size(),
                        qApp->desktop()->availableGeometry()
                       ));
    else
        this->setWindowState(Qt::WindowMaximized);



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
    connect(ui->btnNext, &QPushButton::clicked, [=] {
        if (ui->pageSlider->value() < 14)
            ui->pageSlider->setValue( ui->pageSlider->value() + 1  );
    });
    connect(ui->btnPrevious, &QPushButton::clicked, [=] {
        if (ui->pageSlider->value() > 1)
            ui->pageSlider->setValue( ui->pageSlider->value() - 1  );
    });


    // The close button
    connect(ui->okButton, &QPushButton::clicked, [=] {
        _guideWidget->close();
        _gLayout->removeWidget(_guideWidget);
        delete _guideWidget;
        _guideWidget = nullptr;
        // resetting widgets to their previous states
        for (int i = 0; i < _gLayout->count(); ++i)
        {
            QWidget *widget = _gLayout->itemAt(i)->widget();
            if (widget != nullptr)
                widget->setEnabled(state.at(i));
        }
        this->resize(previousSize);

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
        // resetting fullscreen or recentering anyway
        if (!wasMaximized)
            this->setGeometry(
                        QStyle::alignedRect(
                            Qt::LeftToRight,
                            Qt::AlignCenter,
                            this->size(),
                            qApp->desktop()->availableGeometry()
                           ));
        else
            this->setWindowState(Qt::WindowMaximized);

    });

    // the lambda we connect the slider to forsenE
    connect(ui->pageSlider,&QSlider::valueChanged, [=] (int value){
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

    QLabel *text2 = new QLabel("Based on Qt " + QString(QT_VERSION) + "5.12.1 (https://www.qt.io/) compiled with MinGW 5.3.0 32bit");
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
        UnregisterHotKey(nullptr,i);
    // Clearing it too, else size  of keysequence and winshortcut handle wont match forsenE
    _winShorcutHandle.clear();
    QKeySequence emptySeq;

    QVector<std::shared_ptr<SoundWrapper>> temp;
    for (auto &i: _sounds)
    {
        qDebug() << "[2404]­ use count before deletion of i.get():" << i.use_count();
        temp.append(std::make_shared<SoundWrapper>(i->getSoundList(),i->getPlayMode(),emptySeq, -1,i->getMainDevice(),i->getVacDevice(),nullptr));
        delete i.get();
        qDebug() << "[SoundboardMainUI::ToolClearShortcut]use count before deletion of i.get():" << i.use_count();
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
        this->addSound(i,-1,false,false);
    //refresh is expensive opeation
    this->refreshView();
    this->GenerateGlobalShortcuts();
    this->SetStatusTextEditText("Shortcuts cleared.");
}

void SoundboardMainUI::DealDragAndDrop(int newPlace)
{
    if (newPlace != -1)
        this->SwapWrappers(this->resultView->currentIndex().row(), newPlace );
}

void SoundboardMainUI::SwapWrappers(int firstRow, int secondRow)
{

    qDebug() << "[2435] Need to check if SwapWrappers let ref count unchanged";
    SoundWrapper * temp = _sounds.at(firstRow).get();

    auto firstPtr = std::make_shared<SoundWrapper>(temp->getSoundList(),
                                               temp->getPlayMode(),
                                               temp->getKeySequence(),
                                               temp->getShortcutVirtualKey(),
                                               temp->getMainDevice(),
                                               temp->getVacDevice(),
                                               nullptr);


    temp = _sounds.at(secondRow).get();

    auto secondPtr = std::make_shared<SoundWrapper>(temp->getSoundList(),
                                                temp->getPlayMode(),
                                                temp->getKeySequence(),
                                                temp->getShortcutVirtualKey(),
                                                temp->getMainDevice(),
                                                temp->getVacDevice(),
                                                nullptr);

    // Once we created those new items we can insert them (since it deletes the items we can't reuse them)
    // this should also swap the associated data :thinking:
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

            QString temp = LIDL::Controller::SaveController::GetInstance()->GetSaveName();
            // if snapshot now is different from stored one
            if ( LIDL::Controller::SaveController::GetInstance()->NeedsSaving())
            {
                if (temp.isEmpty() || temp.size() == 0)
                    _statusEdit->setText("Soundboard file not saved.");
                else
                    _statusEdit->setText("Soundboard file: " + temp   + " not saved."  );
            }
            else if (temp.isEmpty())
                _statusEdit->setText("Soundboard file not saved.");
            else //it is the same soundboard as before (ie it is saved)
                _statusEdit->setText("Soundboard file: \"" + temp  + "\".");
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
    for (auto i: LIDL::Controller::SettingsController::GetInstance()->GetRecentFiles() )
    {
        actions.append(new QAction( i.fileName()));
        // using lambdas forsenE
        connect( actions.last(),
                 &QAction::triggered,
                 [=] {
            LIDL::Controller::SaveController::GetInstance()->OpenSaveFile(i.filePath());
        });
        _openRecentMenu->addAction(actions.last());

        if (++count == LIDL::Controller::SettingsController::GetInstance()->GetRecentFilesCount())
            break;
    }
}


void SoundboardMainUI::CheckForUpdates()
{

    qDebug() << "gneeee";
    LIDL::Controller::UpdateChecker* test = new LIDL::Controller::UpdateChecker();
    delete test;
    return;
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
            Q_UNUSED(abortButton);
            msgBox.exec();
            QStringList args("--updater");
            if (msgBox.clickedButton() == nowButton)
            {
#ifdef QT_DEBUG
                bool success = QProcess::startDetached("C:/Program Files (x86)/LIDL Soundboard/SDKMaintenanceTool.exe", args);
                Q_UNUSED(success);
                qDebug() << "path is set to Program Files SDK";
#endif
#ifndef QT_DEBUG
                bool success = QProcess::startDetached(qApp->applicationDirPath() + "/SDKMaintenanceTool.exe", args);
                Q_UNUSED(success);
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
          if (!( LIDL::Controller::SettingsController::GetInstance()->GetSupportedMimeTypes().contains(type.name())))
          {
            accept = false;
            break;
          }
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
    QVector<LIDL::SoundFile*> file;
    foreach (const QUrl &url, e->mimeData()->urls())
    {
        // url are linux style, so /d/blabla/thingy.lidljson
        // we need to remove the first / else it doesn't understand forsenT
        if (url.toString().contains(".lidljson"))
            LIDL::Controller::SaveController::GetInstance()->OpenSaveFile(url.path().remove(0,1));
        else
        {
                // check anyway but it shouldn't be possible
              QMimeDatabase db;
              QMimeType type = db.mimeTypeForFile(url.toString());
              if (LIDL::Controller::SettingsController::GetInstance()->GetSupportedMimeTypes().contains(type.name()))
              {
                    // if the behaviour is to add one wrapper per sound
                    // OR we only have one sound
                    LIDL::SFX tempSfx;
                    tempSfx.flags = LIDL::SFX_TYPE::NONE;
                    if (LIDL::Controller::SettingsController::GetInstance()->GetDragAndDropSeveralWrappers() ||  e->mimeData()->urls().size() == 1)
                    {
                        file.clear();
                        file.append(
                            new LIDL::SoundFile(url.toString(),
                                        static_cast<float>(LIDL::Controller::SettingsController::GetInstance()->GetDefaultMainVolume()/100.0),
                                        static_cast<float>(LIDL::Controller::SettingsController::GetInstance()->GetDefaultVacVolume()/100.0),
                                        tempSfx));
                        QKeySequence emptySeq;
                        // adding sound with empty shortcut and defaulting to singleton
                        this->addSound( std::make_shared<SoundWrapper>(file,LIDL::Playback::Sequential,
                                                                 emptySeq, -1,
                                                                 this->_deviceListOutput->currentIndex(),
                                                                 this->_deviceListVAC->currentIndex()));
                    }
                    // if we have more than 1 sound and we want to add them  all in one thing
                    else if (LIDL::Controller::SettingsController::GetInstance()->GetDragAndDropSeveralWrappers() == false
                              &&  e->mimeData()->urls().size() > 1)
                    {
                        file.append(new LIDL::SoundFile(url.toString(),
                                                        static_cast<float>(LIDL::Controller::SettingsController::GetInstance()->GetDefaultMainVolume()/100.0),
                                                        static_cast<float>(LIDL::Controller::SettingsController::GetInstance()->GetDefaultVacVolume()/100.0),
                                                        tempSfx));
                    }
              }
        }
    }
    if (LIDL::Controller::SettingsController::GetInstance()->GetDragAndDropSeveralWrappers() == false
            &&  e->mimeData()->urls().size() > 1){
    QKeySequence emptySeq;
    // adding sound with empty shortcut and defaulting to sequential
    this->addSound( std::make_shared<SoundWrapper>(file,LIDL::Playback::Sequential,
                                             emptySeq, -1,
                                             this->_deviceListOutput->currentIndex(),
                                             this->_deviceListVAC->currentIndex()));
    }
}
