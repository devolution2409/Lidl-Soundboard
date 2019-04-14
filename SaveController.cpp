#include "SaveController.h"


namespace LIDL {

namespace Controller {

SaveController* SaveController::self = nullptr;

SaveController::SaveController()
{

}

SaveController *SaveController::GetInstance()
{
    if (self == nullptr)
        self = new SaveController();
    return self;

}

void SaveController::Save()
{

    // if file doesn't exist we throw the save as prompt
    if (this->_saveName.isEmpty())
    {
        this->SaveAs();
        return;
    }
    else
    {
        this->SaveAs(this->_saveName);
    }
}

void SaveController::SaveAs(QString fileName)
{
    // if filename is empty we show the prompt
    if (fileName.isEmpty())
    {
        fileName  = QFileDialog::getSaveFileName(nullptr,
                                                     QObject::tr("Save Soundboard As.."),
                                                     LIDL::Controller::SettingsController::GetInstance()->GetDefaultSoundboardFolder() ,
                                                     QObject::tr("LIDL JSON file(*.lidljson)"));
    }
    //  fileName.append(".lidljson");
    QJsonObject save = GenerateSaveFile();
    QJsonDocument *doc = new QJsonDocument(save);
    QString jsonString = doc->toJson(QJsonDocument::Indented);
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly))
    {

        QMessageBox::information(nullptr, QObject::tr("Unable to open file"), file.errorString());
        return;
    }
    else
    {
        this->_saveName = fileName;


        QTextStream out(&file);
        out.setCodec("UTF-8");
        out << jsonString.toUtf8();
        // lidljson detected is used to add the file in the recent file thing
     //   emit lidlJsonDetected(QFileInfo(file));
        file.close();
        // Saving Soundboard state in the SettingsController object
       // emit SaveSoundboardState();
       // this->SetStatusTextEditText("Succesfully saved file: " + fileName);
    }
}


QJsonObject SaveController::GenerateSaveFile() const
{
    QJsonObject obj;

    // inserting version
    // defined in qmake (.pro) file
    obj.insert("Version", VER_STRING);

    QJsonObject settings;
    settings.insert("Main Output Device", this->_mainOutputDevice);
    settings.insert("VAC Output Device",  this->_vacOutputDevice);
    settings.insert("Show Flags", static_cast<int>(LIDL::Controller::SettingsController::GetInstance()->getShowFlags()));

//    QJsonObject stopSoundKey;
//    stopSoundKey.insert("Key Name",this->_shortcutEditStop->getText());
//    stopSoundKey.insert("VirtualKey" ,this->_shortcutEditStop->getVirtualKey());
//    settings.insert("Stop Sound Key",stopSoundKey);

    obj.insert("Settings", settings);

    QJsonArray profiles;

    for (auto &i: LIDL::Controller::ProfileController::GetInstance()->GetProfiles())
    {
        profiles.append(i->GetProfileAsObject());

    }

    obj.insert("Profiles",profiles);
    return obj;
   // qDebug() << QJsonDocument(obj).toJson();
}

void SaveController::SetMainOutputDevice(QString device)
{
    this->_mainOutputDevice = device;
}

void SaveController::SetVacOutputDevice(QString device)
{
    this->_vacOutputDevice = device;
}

QString SaveController::GetMainOutputDevice() const
{
    return this->_mainOutputDevice;
}

QString SaveController::GetVacOutputDevice() const
{
    return this->_vacOutputDevice;
}


} // end namespace controller

} // end namespace LIDL
