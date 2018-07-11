#include "wrapperproperties.h"




WrapperProperties::WrapperProperties(QWidget *parent) //: QWidget(parent)
{
    Q_UNUSED(parent);
    this->setFocusPolicy(Qt::StrongFocus);
    this->setMinimumSize(419,687);
    //w*h

  //  this->_mainWidget = parent;
    // Ajout d'un layout vertical pour afficher les sons
    this->setWindowTitle("Lidl Sounboard Entry Editor");
    this->setWindowIcon(QIcon(":/icon/resources/forsenAim.png"));
    _vLayout = new QVBoxLayout(this);
    // Adding viewer to view sound list
    //_soundListDisplay = new QListWidget(this);

    //    _soundListGroup = new QGroupBox("Sound collection");


    _soundListDisplay = new CustomListWidget(this);

    //Enabling drag&drop reordering
    _soundListDisplay->setDragDropMode(QAbstractItemView::InternalMove);

    //      _soundLayout = new QVBoxLayout(_soundListGroup);
    //      _soundLayout->addWidget(_soundListDisplay);
    _vLayout->addWidget(_soundListDisplay);

    //  _vLayout->addWidget(_soundListGroup);


    _gLayout = new QGridLayout;
    // Add and delete
    _vLayout->addLayout(_gLayout);
    _btnAdd = new QPushButton("Add local file");
    _btnAddFromURL = new QPushButton("Add from URL");
    // connecting to show the modal
    connect(this->_btnAddFromURL, QPushButton::clicked,this, AddSoundFromUrl);

    _btnDelete= new QPushButton("Delete");
    _btnDelete->setEnabled(false);

    _soundListHint = new QLabel("💡 " +  tr("You can Drag and Drop files into this window.\n     Use drag and drop to re-order the sound collection.\n     You can set the volumen, and SFX, of each sound in the volume panel."));
    /*******************************************************
     *                                                     *
     *                     VOLUME SLIDERS                  *
     *                                                     *
     *******************************************************/
    // _sliderGroup = new QGroupBox("Volume");

    _sliderSpoiler = new Spoiler(tr("Volume"));

    _sliderLayout = new QGridLayout();
    _sliderMain  = new QSlider(Qt::Orientation::Horizontal,this);
    _sliderVAC   = new QSlider(Qt::Orientation::Horizontal,this);
    int maxVolume = 100;
    _sliderMain->setRange(0,maxVolume);
    _sliderVAC->setRange(0,maxVolume);
    _sliderLabelMain = new QLabel("Main Output Volume");
    _sliderLabelVAC = new QLabel("VAC Output Volume");
    _sliderMainSpin = new QSpinBox(this);
    _sliderVACSpin  =   new QSpinBox(this);

    _sliderMainSpin->setRange(0,maxVolume);
    _sliderVACSpin->setRange(0,maxVolume);
    _sliderMainSpin->setSuffix("%");
    _sliderVACSpin->setSuffix("%");
    //_sliderHint = new QLabel("💡 You can set the volumes of each sound individually.");


    _sliderLayout->addWidget(_sliderLabelMain,0,0,1,4);
    _sliderLayout->addWidget(_sliderMain,1,0,1,3);
    _sliderLayout->addWidget(_sliderMainSpin,1,3,1,1);
    _sliderLayout->addWidget(_sliderLabelVAC,2,0,1,4);
    _sliderLayout->addWidget(_sliderVAC,3,0,1,3);
    _sliderLayout->addWidget(_sliderVACSpin,3,3,1,1);
    //_sliderLayout->addWidget(_sliderHint,4,0,1,4);


    _sliderSpoiler->setContentLayout( _sliderLayout  );
    _sliderSpoiler->setEnabled(false);


    /*******************************************************
     *                                                     *
     *                     SFX PANEL                       *
     *                                                     *
     *******************************************************/
    _sfxSpoiler = new Spoiler("Special Effects");

    _sfxLayout  = new QGridLayout();



    //  _sfxLayout->setRowMinimumHeight(0,300);
    //    _sfxBtnGroup->setExclusive(false);

    _sfxSpoiler->setEnabled(false);
    // connecting the two spoilers together
    connect(_sfxSpoiler,Spoiler::Opened, [=]{
        _sliderSpoiler->Close();
        //_vLayout->setStretch(1,100);
        //_gLayout->setRowStretch(4,100);
        //->addWidget(_sfxSpoiler,4,0,3,4);
    });
    connect(_sliderSpoiler,Spoiler::Opened,[=] {
        _sfxSpoiler->Close();
        //_vLayout->setStretch(0,100);

    });

    _sfxTabWidget = new QTabWidget(this);
    _sfxLayout->addWidget(_sfxTabWidget);

    /*****************************************************/
    /*              REVAMPED  DISTORTION                 */
    /*****************************************************/

    _distortionWidget = new SfxSettingsWidget(tr("Distortion"));
    _distortionWidget->addSlider(tr("Gain"),0,60, " dB",static_cast<int>(LIDL::SFX_DIST_PARAM::fGain)," -");
    _distortionWidget->addSlider(tr("Edge"),0 ,100 ," %",static_cast<int>(LIDL::SFX_DIST_PARAM::fEdge));
    _distortionWidget->addSlider(tr("Center Frequency"),100,8000," Hz",static_cast<int>(LIDL::SFX_DIST_PARAM::fPostEQCenterFrequency));
    _distortionWidget->addSlider(tr("Bandwidth"),100,8000, " Hz",static_cast<int>(LIDL::SFX_DIST_PARAM::fPostEQBandwidth));
    _distortionWidget->addSlider(tr("Lowpass Cutoff"),100,8000," Hz",static_cast<int>(LIDL::SFX_DIST_PARAM::fPreLowpassCutoff));
//    _distortionWidget->addSpacer();


    /*****************************************************/
    /*              REVAMPED  CHORUS                     */
    /*****************************************************/
    _chorusWidget = new SfxSettingsWidget(tr("Chorus"));
    // check EnumsAndStructs.h for special value
    _chorusWidget->addSlider(tr("Delay"),0,20," ms", static_cast<int>(LIDL::SFX_CHORUS_PARAM::fDelay));
    _chorusWidget->addSlider(tr("Depth"),0,100," %",static_cast<int>(LIDL::SFX_CHORUS_PARAM::fDepth));
    _chorusWidget->addSlider(tr("Feedback"),-99,99," %",static_cast<int>(LIDL::SFX_CHORUS_PARAM::fFeedback));
    _chorusWidget->addSlider(tr("Frequency"),0,10," Hz",static_cast<int>(LIDL::SFX_CHORUS_PARAM::fFrequency));
    _chorusWidget->addSlider(tr("Wet Dry Mix"),0,100," %",static_cast<int>(LIDL::SFX_CHORUS_PARAM::fWetDryMix));
    _chorusWidget->addComboBox(tr("Phase Differential"),
                               (QStringList()  << "-180° (-π rad)"   //    BASS_FX_PHASE_NEG_180
                                               << "-90° (-π/2 rad)"  //    BASS_FX_PHASE_NEG_90
                                               <<"0° (0 rad)"        //    BASS_FX_PHASE_ZERO
                                               <<"90° (π rad)"       //BASS_FX_PHASE_90
                                               <<"180° (π/2 rad)"),
                               static_cast<int>(LIDL::SFX_CHORUS_PARAM::lPhase)); //BASS_FX_PHASE_180

    _chorusWidget->addComboBox("Wave Form", (QStringList() <<tr("Triangular Wave")
                                                  <<  tr("Sinusoidal Wave")),static_cast<int>(LIDL::SFX_CHORUS_PARAM::lWaveform));

    /*****************************************************/
    /*                     ECHO                          */
    /*****************************************************/
    _echoWidget = new SfxSettingsWidget(tr("Echo"));
    _echoWidget->addSlider(tr("Feedback"),0,100," %", static_cast<int>(LIDL::SFX_ECHO_PARAM::fFeedback) );
    _echoWidget->addSlider(tr("Left Delay"),1,2000," ms", static_cast<int>(LIDL::SFX_ECHO_PARAM::fLeftDelay) );
    _echoWidget->addSlider(tr("Right Delay"),1,2000," ms", static_cast<int>(LIDL::SFX_ECHO_PARAM::fRightDelay) );
    _echoWidget->addSlider(tr("Wet Dry Mix"),0,100," %", static_cast<int>(LIDL::SFX_ECHO_PARAM::fWetDryMix) );
    _echoWidget->addComboBox(tr("Swap left and right\n"
                                "delay after echo"),
                             (QStringList() << tr("No") << tr("Yes")),
                             static_cast<int>(LIDL::SFX_ECHO_PARAM::lPanDelay));
    /*****************************************************/
    /*                  COMPRESSOR                   */
    /*****************************************************/
    _compressorWidget = new SfxSettingsWidget(tr("Compressor"));
    _compressorWidget->addSlider(tr("Attack"),0,500," ms", static_cast<int>(LIDL::SFX_COMPRESSOR_PARAM::fAttack) );
    _compressorWidget->addSlider(tr("Gain"),-60,60," dB", static_cast<int>(LIDL::SFX_COMPRESSOR_PARAM::fGain) );
    _compressorWidget->addSlider(tr("Pre Delay"),0,4," ms", static_cast<int>(LIDL::SFX_COMPRESSOR_PARAM::fPredelay) );

    _compressorWidget->addSlider(tr("Ratio"),1,100,":1", static_cast<int>(LIDL::SFX_COMPRESSOR_PARAM::fRatio) );
    _compressorWidget->addSlider(tr("Release"),50,3000," ms", static_cast<int>(LIDL::SFX_COMPRESSOR_PARAM::fRelease) );
    _compressorWidget->addSlider(tr("Threshold"),-60,0," dB", static_cast<int>(LIDL::SFX_COMPRESSOR_PARAM::fThreshold) );

    /*****************************************************/
    /*                     FLANGER                       */
    /*****************************************************/
    _flangerWidget = new SfxSettingsWidget(tr("Flanger"));
    _flangerWidget->addSlider(tr("Delay"),0,4," ms", static_cast<int>(LIDL::SFX_FLANGER_PARAM::fDelay) );
    _flangerWidget->addSlider(tr("Depth"),0,100," ‱", static_cast<int>(LIDL::SFX_FLANGER_PARAM::fDepth) );
    _flangerWidget->addSlider(tr("Feedback"),-99,99," %", static_cast<int>(LIDL::SFX_FLANGER_PARAM::fFeedback) );
    _flangerWidget->addSlider(tr("Frequency"),0,10," Hz", static_cast<int>(LIDL::SFX_FLANGER_PARAM::fFrequency) );
    _flangerWidget->addSlider(tr("Wet Dry Mix"),0,100," %", static_cast<int>(LIDL::SFX_FLANGER_PARAM::fWetDryMix) );
    _flangerWidget->addComboBox(tr("Phase Differential"),
                               (QStringList()  << "-180° (-π rad)"   //    BASS_FX_PHASE_NEG_180
                                               << "-90° (-π/2 rad)"  //    BASS_FX_PHASE_NEG_90
                                               <<"0° (0 rad)"        //    BASS_FX_PHASE_ZERO
                                               <<"90° (π rad)"       //BASS_FX_PHASE_90
                                               <<"180° (π/2 rad)"),
                               static_cast<int>(LIDL::SFX_FLANGER_PARAM::lPhase)); //BASS_FX_PHASE_180

    _flangerWidget->addComboBox(tr("Wave Form"), (QStringList() <<tr("Triangular Wave")
                                              <<  tr("Sinusoidal Wave")),static_cast<int>(LIDL::SFX_FLANGER_PARAM::lWaveform));

    /*****************************************************/
    /*    GARGLE (the cum like a bitch boi gachiBASS     */
    /*****************************************************/
    _gargleWidget = new SfxSettingsWidget(tr("Gargle"));
    _gargleWidget->addSlider(tr("Modulation rate"),1,1000,"Hz",static_cast<int>(LIDL::SFX_GARGLE_PARAM::dwRateHz));
    _gargleWidget->addComboBox(tr("Wave Form"), (QStringList() <<tr("Triangular Wave")
                                              <<  tr("Sinusoidal Wave")),static_cast<int>(LIDL::SFX_GARGLE_PARAM::dwWaveShape));


    _sfxTabWidget->addTab(_distortionWidget,tr("Distortion"));
    _sfxTabWidget->addTab(_chorusWidget,tr("Chorus"));
    _sfxTabWidget->addTab(_echoWidget,tr("Echo"));
    _sfxTabWidget->addTab(_compressorWidget,tr("Compressor"));
    _sfxTabWidget->addTab(_flangerWidget,tr("Flanger"));
    _sfxTabWidget->addTab(_gargleWidget,tr("Gargle"));

    // this line needs be be last so that the spoiler isn't blank
    // (i think there's a copy constructor somewhere maybe)

    _sfxSpoiler->setContentLayout(_sfxLayout);

    /*******************************************************
     *                                                     *
     *                      PLAYBACK MODES                 *
     *                                                     *
     *******************************************************/

    //Radiobox
    _radioGroupBox      = new QGroupBox("Playback mode",this);
    _radioGroup     = new QButtonGroup(this);
    _radioLayout     = new QHBoxLayout(_radioGroupBox);

    // Creating buttons
    _radioSingleton  = new QRadioButton("Singleton",this);
    _radioSequential = new QRadioButton("Sequential",this);
    _radioAuto       = new QRadioButton("Sequential (auto)",this);
    _radioCancer     = new QRadioButton("Singleton (Cancer)",this);

    // setting 1 has default value for playblack
    _playBackMode    = LIDL::Playback::Singleton;

    // Adding them to layout, and setting singleton checked by default
    _radioLayout->addWidget(_radioSingleton);
    _radioLayout->addWidget(_radioCancer);
    _radioSingleton->setChecked(true);
    _radioLayout->addWidget(_radioSequential);
    _radioLayout->addWidget(_radioAuto);
    //_radioLayout->addWidget(_radioToolTip);
    // Adding them to the group
    _radioGroup->addButton(_radioSingleton,1);
    _radioGroup->addButton(_radioSequential,2);
    _radioGroup->addButton(_radioAuto,3);
    _radioGroup->addButton(_radioCancer,4);


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
    _shortcutHint      = new QLabel(tr("Set a shortcut to play the sound.\nWait one second after last released key to apply."));
    _shortcutResetBtn->setFixedHeight(22);
    _shortcutLayout->addWidget(_shortcutEdit,1,0,1,2);
    _shortcutLayout->addWidget(_shortcutResetBtn,1,2,1,1);
    _shortcutLayout->addWidget(_shortcutHint,0,0,1,3);
    _shortcutWarning = new QLabel("⚠️ " + tr("Don't use the same shortcut twice, it will not work."));
    _shortcutLayout->addWidget(_shortcutWarning,2,0,1,3);



    //Done button
    _btnDone = new QPushButton(tr("Done!"));

    //Abort button
    _btnAbort = new QPushButton(tr("Abort mission"));


    /*******************************************************
     *                                                     *
     *                 Displaying widget                   *
     *                                                     *
     *******************************************************/
    _gLayout->addWidget(_btnAdd,0,0,1,2);
    _gLayout->addWidget(_btnAddFromURL,0,2,1,2);
    _gLayout->addWidget(_btnDelete,0,4,1,2);
    _gLayout->addWidget(_soundListHint,1,0,2,6);
    _gLayout->addWidget(_sliderSpoiler,3,0,1,6 );
    _gLayout->addWidget(_sfxSpoiler,4,0,3,6);
    _gLayout->addWidget(_radioGroupBox,7,0,1,6);
    _gLayout->addWidget(_shortcutGroup,8,0,1,6);
    _gLayout->addWidget(_btnDone,9,0,1,4);
    _gLayout->addWidget(_btnAbort,9,4,1,2);

    //_sfxTabWidget->setMinimumHeight(200);

    _sfxTabWidget->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
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
    connect(_btnDelete,&QPushButton::clicked,this, [=]{
        delete this->_selectedItem;
        this->_selectedItem = nullptr;
        this->_btnDelete->setEnabled(false);
        this->_sfxSpoiler->Close();
        this->_sliderSpoiler->Close();
        this->_sliderSpoiler ->setEnabled(false);
        this->_sfxSpoiler->setEnabled(false);

        // need to renable add button if we are in singleton song and this was the last sound
        if (_soundListDisplay->count() == 0)
        {
            this->_radioSingleton->setEnabled(true);
            this->_btnAdd->setEnabled(true);
        }
        // we clear selection
        _soundListDisplay->clearSelection();

    });

    /*******************RADIO BUTTONS************************/
    connect(_radioGroup, SIGNAL(buttonClicked(int)), this, SLOT(RadioPressed(int)));

    /*******************HOTKEY EDITION***********************/
    connect(_shortcutResetBtn,&QPushButton::clicked, this, [=]{
        //Clear the input box for the shortcut but also clears
        //the actual shortcut since it emits the signal that calls the SetKeySequenceSlot
            this->_shortcutEdit->clear();
    });
    connect(_shortcutEdit,SIGNAL(keySequenceChanged(QKeySequence)),this,SLOT(SetKeySequence(QKeySequence)));
    // connect two signals to deal with the one sec delay
    connect(_shortcutEdit, &CustomShortcutEdit::keyPressed,this,[=]{
        // disabling the done button until the done editing signal is recieved
        this->_btnDone->setEnabled(false);
    });
    connect(_shortcutEdit,&CustomShortcutEdit::editingFinished,this,[=]{
        // Enabling the button
        this->_btnDone->setEnabled(true);
        // Showing warning if no modifiers are used
        if ( !(this->_shortcutEdit->getText().contains("Shift",Qt::CaseInsensitive)) &&
             !(this->_shortcutEdit->getText().contains("Ctrl",Qt::CaseInsensitive))  &&
             !(this->_shortcutEdit->getText().contains("Alt",Qt::CaseInsensitive)))
        {
            QMessageBox messageBox;
            messageBox.warning(this,"LIDL Soundboard: " + tr("Warning"),tr("You didn't provide a modifier for this key sequence, be warned this soundboard will INTERCEPT the key, and won't allow it to be used elsewhere."));
            messageBox.setFixedSize(400,200);
            messageBox.show();
        }
    });

    /*******************BUTTON DONE AND ABORT*****************/
    // connect(_btnDone, SIGNAL(clicked()), this, SLOT(CreateWrapper()));
    connect(_btnAbort, &QPushButton::clicked, this, [=]{
         this->close();
    });
    /*******************DRAG AND DROP EVENT******************/
    connect(_soundListDisplay,&CustomListWidget::fileDragged,this,&WrapperProperties::AddSoundFromDrop);
    /****************************SLIDERS*********************/

    // changing spin box will change slider
    connect(_sliderMainSpin,SIGNAL(valueChanged(int)),_sliderMain,SLOT(setValue(int)));
    connect(_sliderVACSpin,SIGNAL(valueChanged(int)),_sliderVAC,SLOT(setValue(int)));
    // changing slider will change spinbox
    connect(_sliderMain,SIGNAL(valueChanged(int)),_sliderMainSpin,SLOT(setValue(int)));
    connect(_sliderVAC,SIGNAL(valueChanged(int)),_sliderVACSpin,SLOT(setValue(int)));

}

// Overloaded contructor to show properties of already built SoundWrapper object
// we call the other constructor so we don't have to to this twice forsenE

//Send infos about all the devices and the push to talk key to hold
//Basically, first 4 things are
// the main audio output
// the VAC output
// last item is the soundwrapper
WrapperProperties::WrapperProperties(int mainOutput, int VACOutput, SoundWrapper *sound, QWidget *parent)
    : WrapperProperties(parent) //: QWidget(parent)
{
    this->_mainOutput = mainOutput;
    this->_VACOutput = VACOutput;
    // if we are opening a sound
    if (sound!= nullptr)
    {
        // Disconnect the done button because else it will create another wrapper
        // disconnect(_btnDone, SIGNAL(clicked()), this, SLOT(CreateWrapper()));
        // Add the files contained in the wrapper to the list


        for (auto &i: sound->getSoundList())
        {
            //qDebug() << "Sound volume:"  << i->getMainVolume() <<       i->getVacVolume() ;
            _soundListDisplay->insertItem(_soundListDisplay->count(),new CustomListWidgetItem(i->url().toUtf8(),
                                                                                              i->getMainVolume(),
                                                                                              i->getVacVolume(),
                                                                                              i->getSFX()
                                                                                              ));
        }
        // Set the mode to the according one
        _playBackMode = sound->getPlayMode();
        switch(_playBackMode)
        {
        case LIDL::Playback::Singleton : this->_radioSingleton->setChecked(true); ; break;
        case LIDL::Playback::Sequential :_radioSequential->setChecked(true); break;
        case LIDL::Playback::Auto: _radioAuto->setChecked(true); break;
        case LIDL::Playback::Cancer: _radioCancer->setChecked(true); break;
        }
        // set the shortcut
        this->_shortcutEdit->setKeySequence(sound->getKeySequence());
        // need to set the virtual key now
        this->_shortcutEdit->setVirtualKey(sound->getShortcutVirtualKey());

        connect(_btnDone,&QPushButton::clicked,this, [=]{
            if( (this->_playBackMode == LIDL::Playback::Singleton) && (this->_soundListDisplay->count()  >1))
            {
                QMessageBox::critical(this, "Error", "Singleton cannot contain more than one sound file.");
                return;
            }
            //SoundWrapper::SoundWrapper(QListWidget* soundList, int playbackMode,QKeySequence * shortcut, QObject * parent)


            QVector<LIDL:: SoundFile *> tempFiles;
            for (int row = 0; row < _soundListDisplay->count(); row++)
            {
                CustomListWidgetItem *item = dynamic_cast<CustomListWidgetItem*>(_soundListDisplay->item(row));
                if (item == nullptr)
                    return;
                else
                    tempFiles.append( new LIDL::SoundFile(item->text(),item->getMainVolume(),item->getVacVolume(), item->getSFX(),item->getSize() ) );
            }
            // Calling constructor IV
            SoundWrapper *tmpSound = new SoundWrapper(tempFiles,
                                                      this->_playBackMode,
                                                      *(this->_shortcutSequence),
                                                      _shortcutEdit->getVirtualKey(),
                                                      _mainOutput,
                                                      _VACOutput);
            // we emit the signal so that the main window knows
            // IT KNOWS forsenKek */
            emit signalEditDone(tmpSound);
            this->close();

        });



    }
    else // if it is a fresh window we need to connect the _btnDone to send a new wrapper
    {
        // we check for the sound being singleton or not, if it is, we don't accept if we have more than 1 sound
        if( (this->_playBackMode == LIDL::Playback::Singleton) && (this->_soundListDisplay->count()  >1))
        {
            QMessageBox::critical(this, "Error", "Singleton cannot contain more than one sound file.");
            return;
        }

        /*SoundWrapper *tmpSound = new SoundWrapper(this->_soundListDisplay,
                                                  this->_playBackMode,
                                                  this->_shortcutSequence,
                                                  this->_shortcutEdit->getVirtualKey(),
                                                  nullptr);*/
        connect(_btnDone,&QPushButton::clicked,this,[=]{
            QVector<LIDL:: SoundFile *> tempFiles;
            for (int row = 0; row < _soundListDisplay->count(); row++)
            {
                CustomListWidgetItem *item = dynamic_cast<CustomListWidgetItem*>(_soundListDisplay->item(row));
                if (item == nullptr)
                    return;
                else
                    tempFiles.append( new LIDL::SoundFile(item->text(),item->getMainVolume(),item->getVacVolume(), item->getSFX(), item->getSize()) );
            }
            // Calling constructor IV
            SoundWrapper *tmpSound = new SoundWrapper(tempFiles,
                                                      this->_playBackMode,
                                                      *(this->_shortcutSequence),
                                                      _shortcutEdit->getVirtualKey(), _mainOutput,_VACOutput);
            // we emit the signal so that the main window knows
            // IT KNOWS forsenKek
            emit signalAddDone(tmpSound);
            this->close();
        });
    }
}



void WrapperProperties::closeEvent(QCloseEvent *event)
{
    _soundListDisplay->clearSelection();
    emit closed();
    this->QWidget::close();
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
    //if we already have more than one s²ound we set the mode to sequential (default)


    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),LIDL::SettingsController::GetInstance()->GetDefaultSoundFolder(), tr("Sounds (*.wav *.mp3 *.ogg *.flac)"));
    // if the fileName isn't empty, the user selected a file, so we add it.
    if (!fileName.isEmpty())
    {
        //QListWidgetItem * tempItem = new QListWidgetItem(fileName);
        //qDebug() << _soundListDisplay->row(item);
        _soundListDisplay->insertItem( _soundListDisplay->count() , new CustomListWidgetItem(fileName,
                                                                                             static_cast<float>(LIDL::SettingsController::GetInstance()->GetDefaultMainVolume()/100.0),
                                                                                             static_cast<float>(LIDL::SettingsController::GetInstance()->GetDefaultVacVolume()/100.0),
                                                                                             0,_soundListDisplay)) ;
        if (_soundListDisplay->count()>1)
        {

            _radioSingleton->setEnabled(false);
            if (_radioGroup->checkedId() == 1)
            {
                _radioSequential->setChecked(true);
                _playBackMode = LIDL::Playback::Sequential;
            }
        }
    }
} // adding of the URI scheme (file:/// will be dealt in CustomListWidgetItem
void WrapperProperties::AddSoundFromDrop(QString file)
{
    //if we already have more than one sound and playback mode was singleton
    // we set the mode to sequential (default)
    if (_soundListDisplay->count()>1)
    {

        _radioSingleton->setEnabled(false);
        if (_radioGroup->checkedId() == 1)
        {
            _radioSequential->setChecked(true);
            _playBackMode = LIDL::Playback::Sequential;
        }
    }
    // if the playback is singleton and we already have one we don't do nuffin
    QString fileName = file;
    // if the fileName isn't empty, the user selected a file, so we add it.
    if (!fileName.isEmpty())
        _soundListDisplay->insertItem(_soundListDisplay->count() ,new CustomListWidgetItem(fileName,
                                                                                           static_cast<float>(LIDL::SettingsController::GetInstance()->GetDefaultMainVolume()/100.0),
                                                                                           static_cast<float>(LIDL::SettingsController::GetInstance()->GetDefaultVacVolume()/100.0),
                                                                                           0,_soundListDisplay)) ;
}


// Event dealing with the colors if mode is changed
void WrapperProperties::RadioPressed(int id)
{
    this->_playBackMode = static_cast<LIDL::Playback>(id);
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



void WrapperProperties::SetKeySequence(QKeySequence shortcut)
{
    this->_shortcutSequence->swap(shortcut);
}





void WrapperProperties::ItemWasClicked(QListWidgetItem *item)
{
    // Dealing with static QMetaObject so we can delete previous connections weSmart
    // IT WORKS: FeelsAmazingMan
    static QMetaObject::Connection *chorusChkConn;
    static QMetaObject::Connection *chorusSliderConn;
    static QMetaObject::Connection *chorusComboConn;

    static QMetaObject::Connection *distortChkConn;
    static QMetaObject::Connection *distortSliderConn;

    static QMetaObject::Connection *echoChkConn;
    static QMetaObject::Connection *echoSliderConn;
    static QMetaObject::Connection *echoComboConn;

    // need to cast item to child class else it doesn't work
    if (item != nullptr)
        _selectedItem = dynamic_cast<CustomListWidgetItem*> (item);
    // if cast was successfull
    if (_selectedItem != nullptr)
    {
        auto dealChorus  = [=]{
            // deactivating every widget if the checkbox isn't checked.
            // However we must add a check because if it is checked already and we click
            // on an item where it is checked aswell, the state of the button will not change.
            if(!( _selectedItem->getSFX().flags & LIDL::SFX_TYPE::CHORUS))
                _chorusWidget->deactivateAll();
            // now we are sure every settings widget is disabled.
            // connecting the checkbox realquick :wrench: forsenE
            if ( chorusChkConn != nullptr)
            {
                delete chorusChkConn;
                chorusChkConn = nullptr;
            }
            chorusChkConn = new QMetaObject::Connection;
            *chorusChkConn = connect(_chorusWidget,&SfxSettingsWidget::checkBoxStateChanged, this,[=](bool newState){
                _selectedItem->SetSFXEnabled(LIDL::SFX_TYPE::CHORUS ,newState);
            } );
            // checking the SFX flag and setting the checkbox accordingly
            _chorusWidget->setCheckboxState(_selectedItem->getSFX().flags & LIDL::SFX_TYPE::CHORUS);


            // Construct the appropriates sliders :)
            // Check limit in EnumsAndStruct
            // starting at 0 this way we can change the order of the items in the enum
            for (int i= 0; i < static_cast<int>(LIDL::SFX_CHORUS_PARAM::ITER_END); i++)
                _chorusWidget->setValueOfEnumParam(i, _selectedItem->getSFXChorus(static_cast<LIDL::SFX_CHORUS_PARAM>(i) )   );


            // if the connection already exists we delete it
            if ( chorusSliderConn != nullptr)
            {
                disconnect(*chorusSliderConn);
                delete chorusSliderConn;
            }
            chorusSliderConn = new QMetaObject::Connection;
            //ONE connection for everything instead of BAZILIONS :FeelsAmazingMan:
            *chorusSliderConn = connect(_chorusWidget,&SfxSettingsWidget::sliderValueChanged,this,[=](int index, int value, int specialValue){
                Q_UNUSED(index);
                _selectedItem->setSFXChorus( static_cast<LIDL::SFX_CHORUS_PARAM>(specialValue),value);
            });

            if (chorusComboConn != nullptr)
            {
                disconnect(*chorusComboConn);
                delete chorusComboConn;
            }
            chorusComboConn = new QMetaObject::Connection;
            *chorusComboConn = connect(_chorusWidget,&SfxSettingsWidget::comboBoxValueChanged,this,[=](int whichOne,int newIndex, int specialValue){
                Q_UNUSED(whichOne);
                //http://bass.radio42.com/help/html/f23be39f-2720-aca0-9b58-ef3a54af2c34.htm
                // index is equal to the value of the BASS_DX8 enum
                // specialValue is equal to EnumsAndStructs.h value
                _selectedItem->setSFXChorus(static_cast<LIDL::SFX_CHORUS_PARAM>(specialValue),newIndex);
            });

        };

        //same logic as chorus (not the same comments so it's worth looking into aswell
        auto dealDistortion = [=]{
            // deactivating every widget if the checkbox isn't checked.
            // However we must add a check because if it is checked already and we click
            // on an item where it is checked aswell, the state of the button will not change.
            if (! (_selectedItem->getSFX().flags & LIDL::SFX_TYPE::DISTORTION))
                _distortionWidget->deactivateAll();
            if (distortChkConn != nullptr)
            {
                delete distortChkConn;
                distortChkConn = nullptr;
            }
            // connecting checkbox to set the flag (or remove it)
            distortChkConn = new QMetaObject::Connection;
            *distortChkConn = connect(_distortionWidget,&SfxSettingsWidget::checkBoxStateChanged, this,[=](bool newState){
                _selectedItem->SetSFXEnabled(LIDL::SFX_TYPE::DISTORTION ,newState);
            } );
            // checking the SFX flag and setting the checkbox accordingly
            _distortionWidget->setCheckboxState(_selectedItem->getSFX().flags & LIDL::SFX_TYPE::DISTORTION);
            // Construct the sliders when the values that were there before.
            // Overflow should be dealt as the min value and max value are capped.
            // Check limit in EnumsAndStruct
            // Works as long as the parameters are contiguous.
            for (int i = 0;i < static_cast<int>(LIDL::SFX_DIST_PARAM::ITER_END);i++)
                _distortionWidget->setValueOfEnumParam(i, _selectedItem->getSFXDistortion(static_cast<LIDL::SFX_DIST_PARAM>(i) )   );

            // if the connection already exists we delete it
            if ( distortSliderConn != nullptr)
            {
                disconnect(*distortSliderConn);
                delete distortSliderConn;
            }
            distortSliderConn = new QMetaObject::Connection;
            //ONE connection for everything instead of BAZILIONS :FeelsAmazingMan:
            *distortSliderConn = connect(_distortionWidget,&SfxSettingsWidget::sliderValueChanged,this,[=](int index, int value, int specialValue){
                Q_UNUSED(index);
                _selectedItem->setSFXDistortion(static_cast<LIDL::SFX_DIST_PARAM>(specialValue),value);
            });
        };

        auto dealEcho = [=]{
                   // deactivating every widget if the checkbox isn't checked.
                   // However we must add a check because if it is checked already and we click
                   // on an item where it is checked aswell, the state of the button will not change.
                   if(!( _selectedItem->getSFX().flags & LIDL::SFX_TYPE::ECHO ))
                       _echoWidget->deactivateAll();
                   // now we are sure every settings widget is disabled.
                   // connecting the checkbox realquick :wrench: forsenE
                   if ( echoChkConn != nullptr)
                   {
                       delete echoChkConn;
                       echoChkConn = nullptr;
                   }
                   echoChkConn = new QMetaObject::Connection;
                   *echoChkConn = connect(_echoWidget,&SfxSettingsWidget::checkBoxStateChanged, this,[=](bool newState){
                       _selectedItem->SetSFXEnabled(LIDL::SFX_TYPE::ECHO ,newState);
                   } );
                   // checking the SFX flag and setting the checkbox accordingly
                   _echoWidget->setCheckboxState(_selectedItem->getSFX().flags & LIDL::SFX_TYPE::ECHO);


                   // Construct the appropriates sliders :)
                   // Check limit in EnumsAndStruct
                   // starting at 0 this way we can change the order of the items in the enum
                   for (int i= 0; i < static_cast<int>(LIDL::SFX_ECHO_PARAM::ITER_END); i++)
                       _echoWidget->setValueOfEnumParam(i, _selectedItem->getSFXEcho(static_cast<LIDL::SFX_ECHO_PARAM>(i) )   );


                   // if the connection already exists we delete it
                   if ( echoSliderConn != nullptr)
                   {
                       disconnect(*echoSliderConn);
                       delete echoSliderConn;
                   }
                   echoSliderConn = new QMetaObject::Connection;
                   //ONE connection for everything instead of BAZILIONS :FeelsAmazingMan:
                   *echoSliderConn = connect(_echoWidget,&SfxSettingsWidget::sliderValueChanged,this,[=](int index, int value, int specialValue){
                       Q_UNUSED(index);
                       _selectedItem->setSFXEcho( static_cast<LIDL::SFX_ECHO_PARAM>(specialValue),value);
                   });

                   if (echoComboConn != nullptr)
                   {
                       disconnect(*echoComboConn);
                       delete echoComboConn;
                   }
                   echoComboConn = new QMetaObject::Connection;
                   *echoComboConn = connect(_echoWidget,&SfxSettingsWidget::comboBoxValueChanged,this,[=](int whichOne,int newIndex, int specialValue){
                       Q_UNUSED(whichOne);
                       //http://bass.radio42.com/help/html/f23be39f-2720-aca0-9b58-ef3a54af2c34.htm
                       // index is equal to the value of the BASS_DX8 enum
                       // specialValue is equal to EnumsAndStructs.h value
                       _selectedItem->setSFXEcho(static_cast<LIDL::SFX_ECHO_PARAM>(specialValue),newIndex);
                   });

               };
        dealChorus();
        dealDistortion();
        dealEcho();

        // enabling the delete button
        _btnDelete->setEnabled(true);
        // enabling the spoilers
        _sliderSpoiler->setEnabled(true);
        _sfxSpoiler->setEnabled(true);
        // setting sliders value of volumes
        _sliderMain->setValue( static_cast<int>(_selectedItem->getMainVolume()*100));
        _sliderVAC->setValue( static_cast<int>(_selectedItem->getVacVolume()*100));
        //qDebug() << "Slider main old value:" << static_cast<int>(_selectedItem->getMainVolume()*100);

        // disconnecting sounds volume sliders
        disconnect(_sliderVAC,SIGNAL(valueChanged(int)),this,SLOT(SetItemVACVolume(int)));
        disconnect(_sliderMain,SIGNAL(valueChanged(int)),this,SLOT(SetItemMainVolume(int)));
        // reconnecting volumes sliders
        connect(_sliderVAC,SIGNAL(valueChanged(int)),this,SLOT(SetItemVACVolume(int)));
        connect(_sliderMain,SIGNAL(valueChanged(int)),this,SLOT(SetItemMainVolume(int)));
    }



}




// SLOTS for the sliders
void WrapperProperties::SetItemMainVolume(int newValue)
{
    if (_selectedItem != nullptr )
        this->_selectedItem->setMainVolume(static_cast<float>(newValue/100.0));
}
void WrapperProperties::SetItemVACVolume(int newValue)
{
    if (_selectedItem != nullptr )
        this->_selectedItem->setVacVolume(static_cast<float>(newValue/100.0));
}


void WrapperProperties::AddSoundFromUrl()
{
    bool ok;
    QInputDialog dialog;
    // -> this isn't workking somehow
    //dialog.setMinimumSize(417,121);
    QString urlString = dialog.getText(this,
                                       tr("Add sound from URL"),
                                       tr("Enter complete file URL.\n"
                                          "Supported protocols are http, https (and maybe, one day, ftp."),
                                       QLineEdit::Normal,
                                       "https://pajlada.se/files/clr/AAAAAAAH.mp3", &ok);

    if (ok && !urlString.isEmpty())
    {
        QUrl url(urlString);
        qDebug() << "url.scheme():" << url.scheme();
        // If the scheme wasn't provided we abort
        if ( url.scheme() != "http" && url.scheme() != "https") // && url.scheme() != "ftp" )
        {
            QMessageBox::critical(this, tr("LIDL Soundboard Entry Editor"),
                                  tr("Specified URL is invalid !\n"
                                     "Did you forget protocol? OMGScoods"),
                                  QMessageBox::Discard);

            return;
        }
        else // we coo? cmonBruh
        {
            unsigned long long size = 0;
            //lambda to check the serverAnswer for mimes types
            // so we dont write the same code twice weSmart
            auto mimeSupported = [](QByteArray serverAnswer){
                QString answer(serverAnswer);


                // The regular expression to compare the servers reply against:
                QRegExp reg("audio/[\\w]+(-{,1}[\\w]{,10})*");


                if (answer.contains("Content-Type"))
                {
                    // Supported Mimes types
                    QStringList supportedMimes = LIDL::SettingsController::GetInstance()->GetSupportedMimeTypes();
                    /* https://shugo.developpez.com/tutoriels/regexqt/
                         * \w is any letter/number or _, + is "once or more"
                         * slash needs to be escaped
                         * so audio/ will search for audio/
                         * [\\w]+ search for any number of char or _
                         * we need to allow -anycharacter-anychar etc and
                         * ()* means 0 or any number of times
                         * QString test = "audio/x-mpeg-3-forsen-is-foobar";
                         */

                    // comparing the string against the QRegExp
                    answer.contains(reg);
                    //qDebug() << reg.cap(0);
                    // reg.cap(0) should now contain the MIME type (if its audio)
                    // if meme type isn't supported we return
                    if  (supportedMimes.contains(reg.cap(0)))
                        return true;
                }
                return false;
            };

            auto getSize = [](QByteArray serverAnswer) -> unsigned long long {
                QString answer(serverAnswer);
                    // need to grab Content-Length: 66240 from server answer
                QRegExp reg("Content-Length: [\\d]+");
                if (answer.contains("Content-Length") )
                {
                    answer.contains(reg);
                    unsigned long long size = static_cast<QString>(reg.cap(0).split(" ").at(1)).toULongLong() ;
                    return size;
                }
                return 0;
            };


            // if it is https
            if (url.scheme() == "https")
            {
                // Testing purposes:
                // Need to work: https://pajlada.se/files/clr/2018-05-28/howstrong.ogg
                // Need to fail: https://pajlada.se/files/clr/
                QSslSocket socket;
                socket.connectToHostEncrypted(url.host() , 443);
                if (socket.waitForConnected())
                {
                    socket.write("HEAD " + url.path().toUtf8() + " HTTP/1.1\r\n"
                                                                 "Host: " + url.host().toUtf8() + "\r\n"
                                                                                                  "\r\n");
                    if (socket.waitForReadyRead())
                    {
                        QByteArray bytes = socket.readAll();
                       // qDebug() << bytes;
                        // If the server answer is ok
                        if (bytes.contains("200 OK"))
                            if  (! mimeSupported(bytes))  // and if the mime type is NOT supported
                            {
                                QMessageBox::critical(this, tr("LIDL Soundboard Entry Editor"),
                                                      tr("MIME Type for specified URL is invalid !\n"
                                                         "Are you sure this is a valid sound file? OMGScoods"),
                                                      QMessageBox::Discard);
                                // we return (we don't add the sound)
                                return;
                            }
                            size = getSize(bytes);

                    }
                }// end if socket wait for connected
            } // end if https

            // if it http
            if (url.scheme()== "http")
            {
                //same logic as for https
                //Testing purposes:
                //Need to work:   http://ppnelles.com/7e/sounds/vive.ogg
                //Need to fail:   http://ppnelles.com/7e/
                QTcpSocket socket;
                socket.connectToHost(url.host() , 80);
                if (socket.waitForConnected())
                {
                    socket.write("HEAD " + url.path().toUtf8() + " HTTP/1.1\r\n"
                                                                 "Host: " + url.host().toUtf8() + "\r\n"
                                                                                                  "\r\n");
                    if (socket.waitForReadyRead())
                    {
                        QByteArray bytes = socket.readAll();
                        if (bytes.contains("200 OK"))
                            if  (!(mimeSupported(bytes)))  // and if the mime type is NOT supported
                            {
                                QMessageBox::critical(this, tr("LIDL Soundboard Entry Editor"),
                                                      tr("MIME Type for specified URL is invalid !\n"
                                                         "Are you sure this is a valid sound file? OMGScoods"),
                                                      QMessageBox::Discard);

                                return;
                            }
                        size = getSize(bytes);
                    }
                }


            } // end if http
            // If everything is good we can add the sound, FeelsOkayMan
            _soundListDisplay->insertItem( _soundListDisplay->count() , new CustomListWidgetItem(url.toString(),
                                                                                                 static_cast<float>(LIDL::SettingsController::GetInstance()->GetDefaultMainVolume()/100.0),
                                                                                                 static_cast<float>(LIDL::SettingsController::GetInstance()->GetDefaultVacVolume()/100.0),
                                                                                                 size, _soundListDisplay)) ;

        }// end else
    }// end if ok and string isn't empty
}

