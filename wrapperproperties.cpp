#include "wrapperproperties.h"




WrapperProperties::WrapperProperties(QWidget *parent) //: QWidget(parent)
{


    this->setFocusPolicy(Qt::StrongFocus);
    this->setMinimumSize(413,611);


    this->_mainWidget = parent;
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
    _btnAdd = new QPushButton("Add");
    _btnDelete= new QPushButton("Delete");
    _btnDelete->setEnabled(false);

    _soundListHint = new QLabel("💡 You can Drag and Drop files into this window.\n     Use drag and drop to re-order the sound collection.\n     You can set the volume of a sound in the volume panel.");
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
    _sliderHint = new QLabel("💡 You can set the volumes of each sound individually.");


    _sliderLayout->addWidget(_sliderLabelMain,0,0,1,4);
    _sliderLayout->addWidget(_sliderMain,1,0,1,3);
    _sliderLayout->addWidget(_sliderMainSpin,1,3,1,1);
    _sliderLayout->addWidget(_sliderLabelVAC,2,0,1,4);
    _sliderLayout->addWidget(_sliderVAC,3,0,1,3);
    _sliderLayout->addWidget(_sliderVACSpin,3,3,1,1);
    _sliderLayout->addWidget(_sliderHint,4,0,1,4);


    _sliderSpoiler->setContentLayout( _sliderLayout  );
    _sliderSpoiler->setEnabled(false);


    /*******************************************************
     *                                                     *
     *                     SFX PANEL                       *
     *                                                     *
     *******************************************************/
    _sfxSpoiler = new Spoiler("Special Effects");

    _sfxLayout  = new QGridLayout();
//    _sfxChorusEnabled  = new QCheckBox("Enable Chorus",this);
//    _sfxCompressorEnabled = new QCheckBox("Compressor");

//    _sfxEchoEnabled       = new QCheckBox("Echo");
//    _sfxFlangerEnabled    = new QCheckBox("Flanger");
//    _sfxGargleEnabled     = new QCheckBox("Gargle");
//    _sfxReverbEnabled     = new QCheckBox("Reverb");

//    _sfxChorusEnabled->setCheckable(true);
//    _sfxCompressorEnabled->setCheckable(true);
//
//    _sfxEchoEnabled->setCheckable(true);
//    _sfxFlangerEnabled->setCheckable(true);
//    _sfxGargleEnabled->setCheckable(true);
//    _sfxReverbEnabled->setCheckable(true);


    // DISTORTION

    _sfxTabWidget = new QTabWidget(this);
    _sfxLayout->addWidget(_sfxTabWidget);
    _sfxDistortionCheckBox = new QCheckBox("Enable distortion");
    _sfxDistortionCheckBox->setCheckable(true);
    _sfxDistortionWidget = new QWidget();
    _sfxDistortionLayout = new QGridLayout(_sfxDistortionWidget);
    _sfxDistortionLayout->addWidget(_sfxDistortionCheckBox,0,0,1,5);

    _sfxDistortionLabels.append(new QLabel("Gain"));
    _sfxDistortionLabels.append(new QLabel("Edge"));
    _sfxDistortionLabels.append(new QLabel("Center Frequency"));
    _sfxDistortionLabels.append(new QLabel("Bandwidth"));
    _sfxDistortionLabels.append(new QLabel("Lowpass Cutoff"));
    _sfxTabWidget->addTab(_sfxDistortionWidget,"Distortion");

    for (int i = 0;i<_sfxDistortionLabels.size();++i)
        _sfxDistortionLayout->addWidget(_sfxDistortionLabels.at(i),i+1,0,1,1);
    for (int i=0; i<5;i++)
    {
        _sfxDistortionSliders.append(new QSlider(Qt::Orientation::Horizontal));
        _sfxDistortionSpinboxes.append(new QSpinBox());
        _sfxDistortionLayout->addWidget( _sfxDistortionSliders.last() ,i+1,1,1,3  );
        _sfxDistortionLayout->addWidget(_sfxDistortionSpinboxes.last(),i+1,4,1,1);
        _sfxDistortionSpinboxes.last()->setEnabled(false);
        _sfxDistortionSliders.last()->setEnabled(false);
     }
    _sfxDistortionDefault = new QPushButton("Reset To Default Values");
    _sfxDistortionDefault->setEnabled(false);
    _sfxDistortionLayout->addWidget( _sfxDistortionDefault,6,0,1,5);
    _sfxDistortionSliders.at(0)->setMinimum(0);
    _sfxDistortionSpinboxes.at(0)->setMinimum(0);
    _sfxDistortionSliders.at(0)->setMaximum(60);
    _sfxDistortionSpinboxes.at(0)->setMaximum(60);
    _sfxDistortionSpinboxes.at(0)->setPrefix("-");
    _sfxDistortionSpinboxes.at(0)->setSuffix("dB");

    _sfxDistortionSliders.at(1)->setMinimum(0);
    _sfxDistortionSliders.at(1)->setMaximum(100);
    _sfxDistortionSpinboxes.at(1)->setMinimum(0);
    _sfxDistortionSpinboxes.at(1)->setMaximum(100);
    _sfxDistortionSpinboxes.at(1)->setSuffix("%");

    _sfxDistortionSliders.at(2)->setMinimum(100);
    _sfxDistortionSliders.at(2)->setMaximum(8000);
    _sfxDistortionSpinboxes.at(2)->setMinimum(100);
    _sfxDistortionSpinboxes.at(2)->setMaximum(8000);

    _sfxDistortionSliders.at(3)->setMinimum(100);
    _sfxDistortionSliders.at(3)->setMaximum(8000);
    _sfxDistortionSpinboxes.at(3)->setMinimum(100);
    _sfxDistortionSpinboxes.at(3)->setMaximum(8000);

    _sfxDistortionSliders.at(4)->setMinimum(100);
    _sfxDistortionSliders.at(4)->setMaximum(8000);
    _sfxDistortionSpinboxes.at(4)->setMinimum(100);
    _sfxDistortionSpinboxes.at(4)->setMaximum(8000);

    for (int i=2;i<5;i++)
        _sfxDistortionSpinboxes.at(i)->setSuffix("Hz");
//    _sfxBtnGroup = new QButtonGroup();
//    _sfxBtnGroup->addButton(_sfxChorus  );
//    _sfxBtnGroup->addButton(_sfxCompressor);
//    _sfxBtnGroup->addButton(_sfxDistortion);
//    _sfxBtnGroup->addButton(_sfxEcho);
//    _sfxBtnGroup->addButton(_sfxFlanger);
//    _sfxBtnGroup->addButton(_sfxGargle);
//    _sfxBtnGroup->addButton(_sfxReverb);
    _sfxSpoiler->setContentLayout(_sfxLayout);
//    _sfxBtnGroup->setExclusive(false);

    _sfxSpoiler->setEnabled(false);
    // connecting the two spoilers together
    connect(_sfxSpoiler,Spoiler::Opened, _sliderSpoiler,Spoiler::Close);
    connect(_sliderSpoiler,Spoiler::Opened,_sfxSpoiler,Spoiler::Close);

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
     // Tooltip hint
     //  _radioToolTip    = new QLabel("       ❔");
     //_radioToolTip->setToolTip("Singleton: A Single Sound\nSequential: A Sound Collection. Next item will play after pressing play or the shortcut.\nSequential(Auto): Same as sequential, but automated.");

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
     _shortcutHint      = new QLabel("Set a shortcut to play the sound.\nWait one second after last released key to apply.");
    _shortcutResetBtn->setFixedHeight(22);
    _shortcutLayout->addWidget(_shortcutEdit,1,0,1,2);
    _shortcutLayout->addWidget(_shortcutResetBtn,1,2,1,1);
    _shortcutLayout->addWidget(_shortcutHint,0,0,1,3);
    _shortcutWarning = new QLabel("⚠️ Don't use the same shortcut twice, it will not work.");
    _shortcutLayout->addWidget(_shortcutWarning,2,0,1,3);



    //Done button
    _btnDone = new QPushButton("Done!");

    //Abort button
    _btnAbort = new QPushButton("Abort mission");


    /*******************************************************
     *                                                     *
     *                 Displaying widget                   *
     *                                                     *
     *******************************************************/
    _gLayout->addWidget(_btnAdd,0,0,1,2);
    _gLayout->addWidget(_btnDelete,0,2,1,2);
    _gLayout->addWidget(_soundListHint,1,0,2,4);
     _gLayout->addWidget(_sliderSpoiler,3,0,1,4 );
     _gLayout->addWidget(_sfxSpoiler,4,0,1,4);
    _gLayout->addWidget(_radioGroupBox,5,0,1,4);
    _gLayout->addWidget(_shortcutGroup,6,0,1,4);
    _gLayout->addWidget(_btnDone,7,0,1,3);
    _gLayout->addWidget(_btnAbort,7,3,1,1);





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
    connect(this,SIGNAL(signalAddDone(SoundWrapper*)),_mainWidget,SLOT(addSound(SoundWrapper*)));
    connect(_btnAbort, SIGNAL(clicked()), this, SLOT(AbortMission()));
    /*******************DRAG AND DROP EVENT******************/
    connect(_soundListDisplay,SIGNAL(fileDragged(QString)),this,SLOT(AddSoundFromDrop(QString)));
    /****************************SLIDERS*********************/

    // changing spin box will change slider
    connect(_sliderMainSpin,SIGNAL(valueChanged(int)),_sliderMain,SLOT(setValue(int)));
    connect(_sliderVACSpin,SIGNAL(valueChanged(int)),_sliderVAC,SLOT(setValue(int)));
    // changing slider will change spinbox
    connect(_sliderMain,SIGNAL(valueChanged(int)),_sliderMainSpin,SLOT(setValue(int)));
    connect(_sliderVAC,SIGNAL(valueChanged(int)),_sliderVACSpin,SLOT(setValue(int)));


}

// overload to ADD sound
//WrapperProperties::WrapperProperties(int mainOutput,int VACOutput,int pttScanCode,int pttVirtualKey,QWidget *parent) : WrapperProperties(parent)
//{

//}

// Overloaded contructor to show properties of already built SoundWrapper object
// we call the other constructor so we don't have to to this twice forsenE

//Send infos about all the devices and the push to talk key to hold
//Basically, first 4 things are
// the main audio output
// the VAC output
// and the PTT key to autohold (as both Scan Code and Virtual Key)
// last item is the soundwrapper
WrapperProperties::WrapperProperties(int mainOutput,int VACOutput,int pttScanCode,int pttVirtualKey,SoundWrapper *sound, QWidget *parent)
    : WrapperProperties(parent) //: QWidget(parent)
{
    this->_mainOutput = mainOutput;
    this->_VACOutput = VACOutput;
    this->_pttScanCode = pttScanCode;
    this->_pttVirtualKey = pttVirtualKey;
    // if we are opening a sound
    if (sound!= nullptr)
    {
        // Disconnect the done button because else it will create another wrapper
        disconnect(_btnDone, SIGNAL(clicked()), this, SLOT(CreateWrapper()));
        // Add the files contained in the wrapper to the list


        for (auto &i: sound->getSoundList())
        {
            //qDebug() << "Sound volume:"  << i->getMainVolume() <<       i->getVacVolume() ;
            _soundListDisplay->insertItem(_soundListDisplay->count(),new CustomListWidgetItem(i->fileName(),
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


        connect(_btnDone, SIGNAL(clicked()), this, SLOT(SendEditedWrapper()));
        connect(this,SIGNAL(signalEditDone(SoundWrapper*)),_mainWidget,SLOT(soundModified(SoundWrapper *)));


    }
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
    //if we already have more than one sound we set the mode to sequential (default)


    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),LIDL::SettingsController::GetInstance()->GetDefaultSoundFolder(), tr("Sounds (*.wav *.mp3)"));
    // if the fileName isn't empty, the user selected a file, so we add it.
    if (!fileName.isEmpty())
    {
        //QListWidgetItem * tempItem = new QListWidgetItem(fileName);
        //qDebug() << _soundListDisplay->row(item);
        _soundListDisplay->insertItem( _soundListDisplay->count() , new CustomListWidgetItem(fileName,
                                                                                             static_cast<float>(LIDL::SettingsController::GetInstance()->GetDefaultMainVolume()/100.0),
                                                                                             static_cast<float>(LIDL::SettingsController::GetInstance()->GetDefaultVacVolume()/100.0) )) ;
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
}
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
            _soundListDisplay->insertItem(_soundListDisplay->count() ,new CustomListWidgetItem(fileName));
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
    QVector<LIDL:: SoundFile *> tempFiles;
    for (int row = 0; row < _soundListDisplay->count(); row++)
    {
         CustomListWidgetItem *item = dynamic_cast<CustomListWidgetItem*>(_soundListDisplay->item(row));
         if (item == nullptr)
             return;
         else
            tempFiles.append( new LIDL::SoundFile(item->text(),item->getMainVolume(),item->getVacVolume(), item->GetSFX()) );
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
}

void WrapperProperties::SendEditedWrapper()
{
    if( (this->_playBackMode == LIDL::Playback::Singleton) && (this->_soundListDisplay->count()  >1))
    {
        QMessageBox::critical(this, "Error", "Singleton cannot contain more than one sound file.");
        return;
    }
    //SoundWrapper::SoundWrapper(QListWidget* soundList, int playbackMode,QKeySequence * shortcut, QObject * parent)
    // We construct the item first and than set the player parameters.
/*    SoundWrapper *tmpSound = new SoundWrapper(this->_soundListDisplay,
                                              this->_playBackMode,
                                              this->_shortcutSequence,
                                              this->_shortcutEdit->getVirtualKey() ,
                                              nullptr);
    // we emit the signal so that the main window knows
    // IT KNOWS forsenKek
    tmpSound->setPlayerPTTScanCode(_pttScanCode);
    tmpSound->setPlayerPTTVirtualKey(_pttVirtualKey);
    tmpSound->setPlayerMainOutput(_mainOutput);
    tmpSound->setPlayerVACOutput(_VACOutput); */
    QVector<LIDL:: SoundFile *> tempFiles;
    for (int row = 0; row < _soundListDisplay->count(); row++)
    {
         CustomListWidgetItem *item = dynamic_cast<CustomListWidgetItem*>(_soundListDisplay->item(row));
         if (item == nullptr)
             return;
         else
            tempFiles.append( new LIDL::SoundFile(item->text(),item->getMainVolume(),item->getVacVolume(), item->GetSFX()) );
    }
    // Calling constructor IV
    SoundWrapper *tmpSound = new SoundWrapper(tempFiles,
                                              this->_playBackMode,
                                              *(this->_shortcutSequence),
                                              _shortcutEdit->getVirtualKey(), _mainOutput,_VACOutput);

    emit signalEditDone(tmpSound);
    this->close();
}

void WrapperProperties::editingDone()
{
    this->_btnDone->setEnabled(true);
        if ( !(this->_shortcutEdit->getText().contains("Shift",Qt::CaseInsensitive)) &&
             !(this->_shortcutEdit->getText().contains("Ctrl",Qt::CaseInsensitive))  &&
             !(this->_shortcutEdit->getText().contains("Alt",Qt::CaseInsensitive)))
        {
            QMessageBox messageBox;
            messageBox.warning(this,"LIDL Soundboard: Warning",tr("You didn't provide a modifier for this key sequence, be warned this soundboard will INTERCEPT the key, and won't allow it to be used elsewhere."));
            messageBox.setFixedSize(400,200);
            messageBox.show();
        }
}

void WrapperProperties::editingStarted()
{
    this->_btnDone->setEnabled(false);
}


void WrapperProperties::ItemWasClicked(QListWidgetItem *item)
{
    // need to cast item to child class else it doesn't work
    if (item != nullptr)
        _selectedItem = dynamic_cast<CustomListWidgetItem*> (item);
    // if cast was successfull
    if (_selectedItem != nullptr)
    {

        // disconnecting sounds
        disconnect(_sliderVAC,SIGNAL(valueChanged(int)),this,SLOT(SetItemVACVolume(int)));
        disconnect(_sliderMain,SIGNAL(valueChanged(int)),this,SLOT(SetItemMainVolume(int)));
        //disconnecting checkBox because it's connected to the setter via the lambda
        disconnect(_sfxDistortionCheckBox);
        disconnect(_sfxDistortionDefault);
        connect(_sfxDistortionDefault,QPushButton::clicked, [=]{ _selectedItem->setSFXDistortionDefault();
            _sfxDistortionSliders.at(0)->setValue(static_cast<int>(- _selectedItem->GetDistortionParams().fGain));
            _sfxDistortionSliders.at(1)->setValue(static_cast<int>(_selectedItem->GetDistortionParams().fEdge));
            _sfxDistortionSliders.at(2)->setValue(static_cast<int>(_selectedItem->GetDistortionParams().fPostEQCenterFrequency));
            _sfxDistortionSliders.at(3)->setValue(static_cast<int>(_selectedItem->GetDistortionParams().fPostEQBandwidth));
            _sfxDistortionSliders.at(4)->setValue(static_cast<int>(_selectedItem->GetDistortionParams().fPreLowpassCutoff));
        });

        // disconnecting the sfx stuff
        for (int i = 0; i<5;i++)
        {
            disconnect(_sfxDistortionSliders.at(i));
            disconnect(_sfxDistortionSpinboxes.at(i));
        }


        for (int i = 0; i<5;i++)
        {
            // connecting the check box to the state of the thing
            connect(_sfxDistortionCheckBox, QCheckBox::stateChanged,
                    [=] (bool checked){
                _sfxDistortionSpinboxes.at(i)->setEnabled(checked);
                _sfxDistortionSliders.at(i)->setEnabled(checked);
                _sfxDistortionDefault->setEnabled(checked);

            });

            // connect sliders and spinbox
            connect(_sfxDistortionSliders.at(i), QSlider::valueChanged, _sfxDistortionSpinboxes.at(i),QSpinBox::setValue);
            connect(_sfxDistortionSpinboxes.at(i), static_cast<void (QSpinBox::*)(int)>(QSpinBox::valueChanged), _sfxDistortionSliders.at(i),QSlider::setValue);
        }
        _sfxDistortionCheckBox->setChecked(_selectedItem->GetDistortionEnabled());
        _sfxDistortionSliders.at(0)->setValue(static_cast<int>(- _selectedItem->GetDistortionParams().fGain));
        _sfxDistortionSliders.at(1)->setValue(static_cast<int>(_selectedItem->GetDistortionParams().fEdge));
        _sfxDistortionSliders.at(2)->setValue(static_cast<int>(_selectedItem->GetDistortionParams().fPostEQCenterFrequency));
        _sfxDistortionSliders.at(3)->setValue(static_cast<int>(_selectedItem->GetDistortionParams().fPostEQBandwidth));
        _sfxDistortionSliders.at(4)->setValue(static_cast<int>(_selectedItem->GetDistortionParams().fPreLowpassCutoff));
        connect(_sfxDistortionCheckBox,QCheckBox::stateChanged,[=](bool enabled){ _selectedItem->SetDistortionEnabled(enabled); });

        connect(_sfxDistortionSliders.at(0),QSlider::valueChanged, [=] (int i){ // gain is negative so we have to add -
            _selectedItem->setSFXDistortion( LIDL::SFX_DIST_PARAM::fGain,-i  );  });
        connect(_sfxDistortionSliders.at(1),QSlider::valueChanged, [=] (int i){
            _selectedItem->setSFXDistortion( LIDL::SFX_DIST_PARAM::fEdge,i  );  });
        connect(_sfxDistortionSliders.at(2),QSlider::valueChanged, [=] (int i){
            _selectedItem->setSFXDistortion( LIDL::SFX_DIST_PARAM::fPostEQCenterFrequency ,i  );  });
        connect(_sfxDistortionSliders.at(3),QSlider::valueChanged, [=] (int i){
            _selectedItem->setSFXDistortion( LIDL::SFX_DIST_PARAM::fPostEQBandwidth,i  );  });
        connect(_sfxDistortionSliders.at(4),QSlider::valueChanged, [=] (int i){
            _selectedItem->setSFXDistortion( LIDL::SFX_DIST_PARAM::fPreLowpassCutoff ,i  );  });

        _btnDelete->setEnabled(true);
        //_sliderGroup->setEnabled(true);
        _sliderSpoiler->setEnabled(true);
        _sfxSpoiler->setEnabled(true);
        // setting sliders value
        _sliderMain->setValue( static_cast<int>(_selectedItem->getMainVolume()*100));
        _sliderVAC->setValue( static_cast<int>(_selectedItem->getVacVolume()*100));
        qDebug() << "Slider main old value:" << static_cast<int>(_selectedItem->getMainVolume()*100);
        // connecting sliders
        connect(_sliderVAC,SIGNAL(valueChanged(int)),this,SLOT(SetItemVACVolume(int)));
        connect(_sliderMain,SIGNAL(valueChanged(int)),this,SLOT(SetItemMainVolume(int)));
    }



}




void WrapperProperties::DeleteSelectedSound()
{
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






