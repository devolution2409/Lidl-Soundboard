#include "soundboardMainUI.h"

SoundboardMainUI::SoundboardMainUI(QWidget *parent) : QWidget(parent)

{
    // Setting up the layouts
    vLayout = new QVBoxLayout(this);
    // not need since adding a parent in the constructor set layout by itself
    // this->setLayout(vLayout);



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

      // Label

      _label3 = new QLabel("Microphone to be injected (optional)",this);
      _gLayout->addWidget(_label3,5,0,1,6);
      _deviceListInjector = new QComboBox(this);
      _gLayout->addWidget(_deviceListInjector,6,0,1,6);

      // Fetching the devices
      this->fetchDeviceList(_deviceListOutput,QAudio::AudioOutput);
      this->fetchDeviceList(_deviceListVAC,QAudio::AudioInput);
      this->fetchDeviceList(_deviceListInjector,QAudio::AudioInput);

      // connecting signals
      connect(this->_btnAdd, SIGNAL(clicked()), this, SLOT(addSoundDialog()));
      connect(this->resultView,SIGNAL(clicked(QModelIndex)),this,SLOT(onCellClicked(QModelIndex)));

      connect(this->_btnEdit, SIGNAL(clicked()), this, SLOT(editSoundDialog()));
      connect(this->_btnDelete, SIGNAL(clicked()), this, SLOT(deleteSound()));


      connect(this->resultView,SIGNAL(enableButtons()),this,SLOT(enableButtons()));
      connect(this->resultView,SIGNAL(disableButtons()),this,SLOT(disableButtons()));


}

//SoundboardMainUI::addSound



void SoundboardMainUI::fetchDeviceList(QComboBox *comboBox, QAudio::Mode mode)
{
    // Adding null device
    comboBox->addItem("<No device selected>",Qt::DisplayRole);

    for (auto &deviceInfo: QAudioDeviceInfo::availableDevices(mode))
        comboBox->addItem(deviceInfo.deviceName(),Qt::DisplayRole);

}
// The dialogue to be opened when the Add button is pressed
void SoundboardMainUI::addSoundDialog()
{
    this->setEnabled(false);
    _propertiesWindow = new WrapperProperties(this);
    _propertiesWindow->show();

}

void SoundboardMainUI::deleteSound()
{
    // check if selected sound is inside the array
    if (this->lastSelectedRow <= this->_sounds.size())
    {
        this->_sounds.removeAt(lastSelectedRow);
        this->_data.removeAt(lastSelectedRow);
        this->_model->removeRow(lastSelectedRow);
    }
    // If we have no more sounds in the soundboard, disable delete and edit button
    if (this->_sounds.size() == 0)
    {
        this->_btnDelete->setEnabled(false);
        this->_btnEdit->setEnabled(false);
    }

}



void SoundboardMainUI::soundAdded(SoundWrapper * modifiedSound)
{

    _sounds.append(modifiedSound);
    QList<QStandardItem*> tempList;
    tempList = modifiedSound->getSoundAsItem();
    //setting the flags
    for (auto &i:tempList)
        i->setFlags(Qt::ItemIsSelectable |Qt::ItemIsEnabled);



    _data.append(tempList);

    _model->appendRow(_data.last());
    this->resultView->resizeRowsToContents();
    // Enabling the edit button


}


// Dealing with click on a row: update index
void SoundboardMainUI::onCellClicked(QModelIndex index)
{
    lastSelectedRow = index.row();
    this->_btnEdit->setEnabled(true);
    this->_btnDelete->setEnabled(true);
    //qDebug() << "User clicked a cell on row number: " << index.row();

}

// open the dialog to edit sound
void SoundboardMainUI::editSoundDialog()
{
    //if lastSelectedRow is valid (ie we didn't delete this entry)
    if (this->lastSelectedRow <= this->_sounds.size())
    {
       _propertiesWindow= new WrapperProperties(this->_sounds.at(this->lastSelectedRow)  ,this);
       _propertiesWindow->show();
    }
}
// deal with modified sound
void SoundboardMainUI::soundModified(SoundWrapper *modifiedSound)
{
    // we must delete the previous sound and insert this one in place
    // lastSelectedRow should still be the same
    _sounds.removeAt(lastSelectedRow);
    _sounds.insert(lastSelectedRow,modifiedSound);

    QList<QStandardItem*> tempList;
    tempList = modifiedSound->getSoundAsItem();
    //setting the flags
    for (auto &i:tempList)
        i->setFlags(Qt::ItemIsSelectable |Qt::ItemIsEnabled);

    // Need to update the model data as well
    _data.removeAt(lastSelectedRow);
    _data.insert(lastSelectedRow,tempList);

    //we need to update model accordingly
    _model->removeRow(lastSelectedRow);
    _model->insertRow(lastSelectedRow,_data.at(lastSelectedRow));
    //Resizing policy
    this->resultView->resizeRowsToContents();
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




