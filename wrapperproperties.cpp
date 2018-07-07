#include "wrapperproperties.h"




WrapperProperties::WrapperProperties(QWidget *parent) //: QWidget(parent)
{

    this->setFocusPolicy(Qt::StrongFocus);
    this->setMinimumSize(415,687);


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



    // DISTORTION

    _sfxTabWidget = new QTabWidget(this);
    _sfxLayout->addWidget(_sfxTabWidget);



    _sfxDistortionCheckBox = new QCheckBox("Enable Distortion");
    _sfxDistortionCheckBox->setCheckable(true);
    _sfxDistortionWidget = new QWidget();
    _sfxDistortionLayout = new QGridLayout(_sfxDistortionWidget);
    _sfxDistortionLayout->addWidget(_sfxDistortionCheckBox,0,0,1,5);

    _sfxTabWidget->addTab(_sfxDistortionWidget,"Distortion");
    _sfxDistortionLabels.append(new QLabel("Gain"));
    _sfxDistortionLabels.append(new QLabel("Edge"));
    _sfxDistortionLabels.append(new QLabel("Center Frequency"));
    _sfxDistortionLabels.append(new QLabel("Bandwidth"));
    _sfxDistortionLabels.append(new QLabel("Lowpass Cutoff"));

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

    /*

    connect(_sfxTabWidget,QTabWidget::currentChanged, [=](int index)
    {
        for(int i=0;i<_sfxTabWidget->count();i++)
            if(i!=index)
                _sfxTabWidget->widget(i)->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);

        _sfxTabWidget->widget(index)->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        _sfxTabWidget->widget(index)->resize(_sfxTabWidget->widget(index)->minimumSizeHint());
        _sfxTabWidget->widget(index)->adjustSize();
        resize(minimumSizeHint());
        adjustSize();

    });
*/
    /****************************************************/
    //                       CHORUS
    /*****************************************************/
    _sfxChorusWidget = new QWidget();
    _sfxChorusLayout = new QGridLayout(); //_sfxChorusWidget


    _sfxChorusScrollArea = new QScrollArea();
    _sfxTabWidget->addTab(_sfxChorusScrollArea,"Chorus");

    //   _sfxChorusLayout->setRowMinimumHeight(0,50);
    _sfxChorusWidget->setLayout(_sfxChorusLayout);
    _sfxChorusLayout->setSizeConstraint(QLayout::SetMinimumSize);

    //_sfxChorusLayout->setSizePolicy(QSizePolicy::MinimumExpanding,QSizePolicy::MinimumExpanding);
    _sfxTabWidget->addTab(_sfxChorusWidget,"Chorus");
    _sfxChorusCheckBox = new QCheckBox("Enable Chorus");
    _sfxChorusCheckBox->setCheckable(true);

    _sfxChorusLayout->addWidget(_sfxChorusCheckBox,0,0,1,5);

    //http://bass.radio42.com/help/html/846d2089-756b-8780-80ec-fc361bb434b6.htm
    _sfxChorusLabels.append(new QLabel("Delay"));
    _sfxChorusLabels.append(new QLabel("Depth"));
    _sfxChorusLabels.append(new QLabel("Feedback"));
    _sfxChorusLabels.append(new QLabel("Frequency"));
    _sfxChorusLabels.append(new QLabel("Wet Dry Mix"));
    _sfxChorusLabels.append(new QLabel("Phase Differential"));
    _sfxChorusLabels.append(new QLabel("Wave form"));
    //adding labels
    for (int i = 0;i<_sfxChorusLabels.size();++i)
        _sfxChorusLayout->addWidget(_sfxChorusLabels.at(i),i+1,0,1,1);
    // adding spinboxes forsenE
    for (int i=0; i<5;i++)
    {
        //connecting sliders and spinboxes is done in the ItemWasClicked Signal
        _sfxChorusSliders.append(new QSlider(Qt::Orientation::Horizontal));
        _sfxChorusSpinboxes.append(new QSpinBox());
        _sfxChorusLayout->addWidget( _sfxChorusSliders.last() ,i+1,1,1,3  );
        _sfxChorusLayout->addWidget(_sfxChorusSpinboxes.last(),i+1,4,1,1);
        _sfxChorusSpinboxes.last()->setEnabled(false);
        _sfxChorusSliders.last()->setEnabled(false);
    }
    _sfxChorusSliders.at(0)->setRange(0,20);
    _sfxChorusSpinboxes.at(0)->setRange(0,20);
    _sfxChorusSpinboxes.at(0)->setSuffix("ms");

    _sfxChorusSliders.at(1)->setRange(0,100);
    _sfxChorusSpinboxes.at(1)->setRange(0,100);
    _sfxChorusSpinboxes.at(1)->setSuffix("%");

    _sfxChorusSliders.at(2)->setRange(-99,99);
    _sfxChorusSpinboxes.at(2)->setRange(-99,99);
    _sfxChorusSpinboxes.at(2)->setSuffix("%");

    _sfxChorusSliders.at(3)->setRange(0,10);
    _sfxChorusSpinboxes.at(3)->setRange(0,10);
    _sfxChorusSpinboxes.at(3)->setSuffix("Hz");

    _sfxChorusSliders.at(4)->setRange(0,100);
    _sfxChorusSpinboxes.at(4)->setRange(0,100);
    _sfxChorusSpinboxes.at(4)->setSuffix("%");
    for (int i=0; i<2;i++)
    {
        _sfxChorusBox.append(new QComboBox());
        _sfxChorusLayout->addWidget( _sfxChorusBox.last() ,i+6,1,1,4 );
        _sfxChorusBox.last()->setEnabled(false);
    }

    _sfxChorusButton.append(new QPushButton("Reset to default"));
    _sfxChorusButton.append(new QPushButton("Preset A"));
    _sfxChorusButton.append(new QPushButton("Preset B"));
    for (auto &i: _sfxChorusButton)
        i->setEnabled(false);

    _sfxChorusLayout->addWidget( _sfxChorusButton.at(0),8,0,1,2);
    _sfxChorusLayout->addWidget( _sfxChorusButton.at(1),8,2,1,1);
    _sfxChorusLayout->addWidget( _sfxChorusButton.at(2),8,3,1,1);
    //http://bass.radio42.com/help/html/f23be39f-2720-aca0-9b58-ef3a54af2c34.htm
    _sfxChorusBox.at(0)->addItem("-180° (-π rad)", QVariant(0) ); //	BASS_FX_PHASE_NEG_180
    _sfxChorusBox.at(0)->addItem("-90° (-π/2 rad)", QVariant(1)  ); //	BASS_FX_PHASE_NEG_90
    _sfxChorusBox.at(0)->addItem("0° (0 rad)", QVariant(2)  ); //	BASS_FX_PHASE_ZERO
    _sfxChorusBox.at(0)->addItem("90° (π rad)", QVariant(3)  ); //	BASS_FX_PHASE_90
    _sfxChorusBox.at(0)->addItem("180° (π/2 rad)", QVariant(4)  );  //	BASS_FX_PHASE_180

    _sfxChorusBox.at(1)->addItem("Sinusoidal Wave", QVariant(1));
    _sfxChorusBox.at(1)->addItem("Triangular Wave", QVariant(0));
    _sfxChorusScrollArea->setWidget(_sfxChorusWidget);
    _sfxChorusScrollArea->setWidgetResizable(true);
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
    connect(_soundListDisplay,SIGNAL(fileDragged(QString)),this,SLOT(AddSoundFromDrop(QString)));
    /****************************SLIDERS*********************/

    // changing spin box will change slider
    connect(_sliderMainSpin,SIGNAL(valueChanged(int)),_sliderMain,SLOT(setValue(int)));
    connect(_sliderVACSpin,SIGNAL(valueChanged(int)),_sliderVAC,SLOT(setValue(int)));
    // changing slider will change spinbox
    connect(_sliderMain,SIGNAL(valueChanged(int)),_sliderMainSpin,SLOT(setValue(int)));
    connect(_sliderVAC,SIGNAL(valueChanged(int)),_sliderVACSpin,SLOT(setValue(int)));


    /* testing slider spin class */
   // SliderSpin* test = new SliderSpin(100,2897,"Hz");
//    SfxSettingsWidget* test = new SfxSettingsWidget("Testing new class");
//    test->addSlider("slider 1",0,50);
//    _gLayout->addWidget(test,10,0,1,6);
//    test->addSlider("LULULUL",322,3154);

//    connect(test,&SfxSettingsWidget::valueChanged,this,[=](int index, int value ){
//        qDebug() << "testing widget, combo box: " << index << "now has value: " << value;
//    });

//     connect(test,&SliderSpin::valueChanged, this, [=](int value){
//         qDebug() << "new value is:" << value;
//     });

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
                    tempFiles.append( new LIDL::SoundFile(item->text(),item->getMainVolume(),item->getVacVolume(), item->GetSFX()) );
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
                                                                                           static_cast<float>(LIDL::SettingsController::GetInstance()->GetDefaultVacVolume()/100.0) )) ;
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
        disconnect(_sfxChorusCheckBox);
        //  disconnecting the sfx distortion stuff
        for (int i = 0; i<5;i++)
        {
            disconnect(_sfxDistortionSliders.at(i));
            disconnect(_sfxDistortionSpinboxes.at(i));
        }
        // Default button value
        connect(_sfxDistortionDefault,QPushButton::clicked, [=]
        {
            // Set default values then update the sliders
            _selectedItem->setSFXDistortionDefault();
            _sfxDistortionSliders.at(0)->setValue(static_cast<int>(- _selectedItem->GetSFX().distortion.fGain));
            _sfxDistortionSliders.at(1)->setValue(static_cast<int>(_selectedItem->GetSFX().distortion.fEdge));
            _sfxDistortionSliders.at(2)->setValue(static_cast<int>(_selectedItem->GetSFX().distortion.fPostEQCenterFrequency));
            _sfxDistortionSliders.at(3)->setValue(static_cast<int>(_selectedItem->GetSFX().distortion.fPostEQBandwidth));
            _sfxDistortionSliders.at(4)->setValue(static_cast<int>(_selectedItem->GetSFX().distortion.fPreLowpassCutoff));
        });
        // 0 is default, 1 is preset A, 2 is preset B
        connect(_sfxChorusButton.at(0), QPushButton::clicked, [=]
        {
            // delay  depth feeback frequency wetdrymix phaseDifferential waveform
            _selectedItem->setSFXChorusDefault();
            _sfxChorusSliders.at(0)->setValue(static_cast<int>(_selectedItem->GetSFX().chorus.fDelay));
            _sfxChorusSliders.at(1)->setValue(static_cast<int>(_selectedItem->GetSFX().chorus.fDepth));
            _sfxChorusSliders.at(2)->setValue(static_cast<int>(_selectedItem->GetSFX().chorus.fFeedback));
            _sfxChorusSliders.at(3)->setValue(static_cast<int>(_selectedItem->GetSFX().chorus.fFrequency));
            _sfxChorusSliders.at(4)->setValue(static_cast<int>(_selectedItem->GetSFX().chorus.fWetDryMix));
            _sfxChorusBox.at(0)->setCurrentIndex( _sfxChorusBox.at(0)->findData( static_cast<int>(_selectedItem->GetSFX().chorus.lPhase) ) );
            _sfxChorusBox.at(1)->setCurrentIndex( _sfxChorusBox.at(1)->findData( static_cast<int>(_selectedItem->GetSFX().chorus.lWaveform) ) );
        });


        // CONNECTING DISTORTION CHECKBOX
        for (int i = 0; i<5;i++)
        {
            // connecting the check box to the state of the thing
            connect(_sfxDistortionCheckBox, QCheckBox::stateChanged,
                    [=] (bool checked){
                _sfxDistortionSpinboxes.at(i)->setEnabled(checked);
                _sfxDistortionSliders.at(i)->setEnabled(checked);
            });

            // connect sliders and spinbox
            connect(_sfxDistortionSliders.at(i), QSlider::valueChanged, _sfxDistortionSpinboxes.at(i),QSpinBox::setValue);
            connect(_sfxDistortionSpinboxes.at(i), static_cast<void (QSpinBox::*)(int)>(QSpinBox::valueChanged), _sfxDistortionSliders.at(i),QSlider::setValue);
        }

        // Still checkbox
        for (int i =0;i <2;i++)
        {
            _sfxChorusBox.at(i)->disconnect();
            connect(_sfxChorusCheckBox,QCheckBox::stateChanged,
                    [=](bool checked){ _sfxChorusBox.at(i)->setEnabled(checked); });
        }
        // CONNECTING CHORUS CHECKBOX TO ENABLE SLIDERS. AND CONNECTING SLIDERS AND SPINBOXES TOGETHER REEEEE
        for (int i = 0; i < 5; i ++)
        {
            connect(_sfxChorusCheckBox,QCheckBox::stateChanged,
                    [=](bool checked){
                _sfxChorusSpinboxes.at(i)->setEnabled(checked);
                _sfxChorusSliders.at(i)->setEnabled(checked);
            });

            connect(_sfxChorusSliders.at(i), QSlider::valueChanged, _sfxChorusSpinboxes.at(i),QSpinBox::setValue);
            connect(_sfxChorusSpinboxes.at(i), static_cast<void (QSpinBox::*)(int)>(QSpinBox::valueChanged), _sfxChorusSliders.at(i),QSlider::setValue);
        }
        // still check box Zzzz
        for (int i = 0;i<3;i++)
            connect(_sfxChorusCheckBox,QCheckBox::stateChanged, [=](bool state){
                _sfxChorusButton.at(i)->setEnabled(state);
            });
        // connecting all sliders
        connect(_sfxChorusSliders.at(0),QSlider::valueChanged, [=] (int i){
            _selectedItem->setSFXChorus( LIDL::SFX_CHORUS_PARAM::fDelay,i);});
        connect(_sfxChorusSliders.at(1),QSlider::valueChanged, [=] (int i){
            _selectedItem->setSFXChorus( LIDL::SFX_CHORUS_PARAM::fDepth,i);});
        connect(_sfxChorusSliders.at(2),QSlider::valueChanged, [=] (int i){
            _selectedItem->setSFXChorus( LIDL::SFX_CHORUS_PARAM::fFeedback,i);});
        connect(_sfxChorusSliders.at(3),QSlider::valueChanged, [=] (int i){
            _selectedItem->setSFXChorus( LIDL::SFX_CHORUS_PARAM::fFrequency,i);});
        connect(_sfxChorusSliders.at(4),QSlider::valueChanged, [=] (int i){
            _selectedItem->setSFXChorus( LIDL::SFX_CHORUS_PARAM::fWetDryMix,i);});
        connect(_sfxChorusBox.at(0),static_cast<void (QComboBox::*) (int)>(QComboBox::currentIndexChanged),[=] (int i){
            _selectedItem->setSFXChorus(LIDL::SFX_CHORUS_PARAM::lPhase, _sfxChorusBox.at(0)->itemData(i).toInt() );    });
        connect(_sfxChorusBox.at(1),static_cast<void (QComboBox::*) (int)>(QComboBox::currentIndexChanged),[=] (int i){
            _selectedItem->setSFXChorus(LIDL::SFX_CHORUS_PARAM::lWaveform, _sfxChorusBox.at(1)->itemData(i).toInt() );    });


        // if flags are checked then we select checkboxes
        if (_selectedItem->GetSFX().flags & LIDL::SFX_TYPE::DISTORTION)
        {
            _sfxDistortionCheckBox->setChecked(true);
            _sfxDistortionDefault->setEnabled(true);
        }
        else
            _sfxDistortionCheckBox->setChecked(false);

        if (_selectedItem->GetSFX().flags & LIDL::SFX_TYPE::CHORUS)
        {
            _sfxChorusCheckBox->setChecked(true);
            for (auto &i: _sfxChorusButton)
                i->setEnabled(true);
        }
        else
            _sfxChorusCheckBox->setChecked(false);

        _sfxDistortionSliders.at(0)->setValue(static_cast<int>(_selectedItem->GetSFX().distortion.fGain));
        _sfxDistortionSliders.at(1)->setValue(static_cast<int>(_selectedItem->GetSFX().distortion.fEdge));
        _sfxDistortionSliders.at(2)->setValue(static_cast<int>(_selectedItem->GetSFX().distortion.fPostEQCenterFrequency));
        _sfxDistortionSliders.at(3)->setValue(static_cast<int>(_selectedItem->GetSFX().distortion.fPostEQBandwidth));
        _sfxDistortionSliders.at(4)->setValue(static_cast<int>(_selectedItem->GetSFX().distortion.fPreLowpassCutoff));

        _sfxChorusSliders.at(0)->setValue(static_cast<int>(_selectedItem->GetSFX().chorus.fDelay));
        _sfxChorusSliders.at(1)->setValue(static_cast<int>(_selectedItem->GetSFX().chorus.fDepth));
        _sfxChorusSliders.at(2)->setValue(static_cast<int>(_selectedItem->GetSFX().chorus.fFeedback));
        _sfxChorusSliders.at(3)->setValue(static_cast<int>(_selectedItem->GetSFX().chorus.fFrequency));
        _sfxChorusSliders.at(4)->setValue(static_cast<int>(_selectedItem->GetSFX().chorus.fWetDryMix));
        _sfxChorusBox.at(0)->setCurrentIndex( _sfxChorusBox.at(0)->findData( static_cast<int>(_selectedItem->GetSFX().chorus.lPhase) ) );
        _sfxChorusBox.at(1)->setCurrentIndex( _sfxChorusBox.at(1)->findData( static_cast<int>(_selectedItem->GetSFX().chorus.lWaveform) ) );
        connect(_sfxDistortionCheckBox,QCheckBox::stateChanged,[=](bool enabled){
            _selectedItem->SetSFXEnabled(LIDL::SFX_TYPE::DISTORTION,enabled);
            _sfxDistortionDefault->setEnabled(enabled);
        });

        connect(_sfxChorusCheckBox,QCheckBox::stateChanged,[=](bool enabled){
            _selectedItem->SetSFXEnabled(LIDL::SFX_TYPE::CHORUS,enabled);
            //   _sfxChorusDefault->setEnabled(enabled);
        });




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
        //qDebug() << "Slider main old value:" << static_cast<int>(_selectedItem->getMainVolume()*100);
        // connecting volumes sliders
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
                    else
                        return false;

                }
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

                    }
                }// end if socket wait for connected
            }

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
                    }
                }


            }
        }

        // If everything is good we can add the sound, FeelsOkayMan
        _soundListDisplay->insertItem( _soundListDisplay->count() , new CustomListWidgetItem(url.toString(),
                                                                                             static_cast<float>(LIDL::SettingsController::GetInstance()->GetDefaultMainVolume()/100.0),
                                                                                             static_cast<float>(LIDL::SettingsController::GetInstance()->GetDefaultVacVolume()/100.0) )) ;


    }// end if ok and string isn't empty
}
