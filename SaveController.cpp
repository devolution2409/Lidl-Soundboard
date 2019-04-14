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

void SaveController::WriteSaveFile(QString which) const
{
    QJsonObject obj;

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

    qDebug() << QJsonDocument(obj).toJson();
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
