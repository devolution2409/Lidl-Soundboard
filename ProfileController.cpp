#include "include/ProfileController.h"

namespace LIDL {

namespace Controller{

ProfileController * ProfileController::self = nullptr;

ProfileController *ProfileController::GetInstance()
{
    if (self == nullptr)
        self = new ProfileController();
    return self;


}

ProfileController::ProfileController()
{
    _activeProfile = nullptr;
}

std::vector<Profile *> ProfileController::GetProfiles() const
{
    return this->_profiles;
}

void  ProfileController::ManualGameConfigurationChanged(const QString &name)
{
    // searching for the profile with the correct name
    qDebug() << "[ProfileController::ManualGameConfigurationChanged()] called";

    bool found = false;
    for (auto &i: _profiles)
    {
        if (i->GetName() == name)
        {

            this->_activeProfile = i;
            found = true;
            qDebug() << "found profile:" << name;
            break;
        }

    }
    if (found)
    {
        // if the profile has a parent we need to reflect the sounds from there
        if (_activeProfile->GetParent() != nullptr)
        {
            qDebug() <<  "[ProfileController::ManualGameConfigurationChanged()] Please implement mirroring the sounds";
            // while not being editabel
        }
        else
        {
            // clear everything from the main UI and then
            // and the profile forsenT
            QVector<std::shared_ptr<SoundWrapper>> temp = _activeProfile->GetSounds();
            _activeProfile->ClearSounds();
            // add sounds

            //connected to ProfileSwitched(QVector<std::shared_ptr<SoundWrapper>> wrappers)
            emit AddSoundsToMainUI(temp);

        }


    }

}

Profile *ProfileController::GetActiveProfile() const
{
    return this->_activeProfile;
}

void ProfileController::RemoveAllProfiles()
{
    for (auto &i: _profiles)
    {
        qDebug() << "lel ---------------------" << i->GetName();
        delete i;

    }
    _profiles.clear();
    this->_activeProfile = nullptr;

}

void ProfileController::AutomaticConfigurationChange(const QString &name)
{
    // searching for the profile with the correct name
    qDebug() << "[ProfileController::AutomaticConfigurationChange()] called";

    bool found = false;
    for (auto &i: _profiles)
    {

        qDebug() << "greighreioghreoighreioghreoighreoighoireghiore";
        if (i->GetName() == name)
        {

            this->_activeProfile = i;
            found = true;
          //  qDebug() << "found profile:" << name;
            break;
        }

    }
    if (found)
    {
        // if the profile has a parent we need to reflect the sounds from there
        if (_activeProfile->GetParent() != nullptr)
        {
            qDebug() <<  "[ProfileController::AutomaticConfigurationChange()] Please implement mirroring the sounds";
            // while not being editabel
        }
        else
        {
            // clear everything from the main UI and then
            // and the profile forsenT

            QVector<std::shared_ptr<SoundWrapper>> temp = _activeProfile->GetSounds();
            _activeProfile->ClearSounds();
            // add sounds

            //connected to ProfileSwitched(QVector<std::shared_ptr<SoundWrapper>> wrappers)
            emit AddSoundsToMainUI(temp);

        }


    }
    emit RefreshComboBox();
     qDebug() << "[ProfileController::AutomaticConfigurationChange()] ended nam";
}

void ProfileController::AddProfile(Profile* profile,LIDL::PROFILE_COPY_MODE copyMode)
{
    // pushing the profile in the array
    _profiles.push_back(profile);
    qDebug() << "[ProfileController::AddProfile] Adding profile to array:  " << profile->GetName();

    qDebug() << "[ProfileController::AddProfile] Please add profile name collision here for when soundboard is opening so that defualt profile isn't duplicated";


    if (copyMode == LIDL::PROFILE_COPY_MODE::NO_COPY)
    {
        // do nothing
    }
    else if (copyMode == LIDL::PROFILE_COPY_MODE::COPY)
    {
        qDebug() << "TODO: implement copy wrappers from another profile ";
    }
    else if (copyMode == LIDL::PROFILE_COPY_MODE::MIRROR)
    {
        qDebug() << "TODO: Add mirroring wrappers from another profile. Maybe be connecting the sound added signal ?"
                    " or by holding a list of profile that are mirrored  in the profile? IDK KEV PepeS";
    }




    qDebug().noquote() << profile->GetConfigAsString();


      this->AutomaticConfigurationChange(profile->GetName());






}

void ProfileController::ReplaceProfiles(std::vector<Profile*> profiles)
{
    for (auto &i: _profiles)
    {
        delete i;
    }

    _profiles.clear();
    _profiles = profiles;
}

void ProfileController::DeleteActiveProfile()
{
    // we don't wanna delete the default profile :)
    if ( _activeProfile == nullptr || _activeProfile->GetName() == "Default")
        return;


    // Remove it from the profile array
    for  (unsigned int i = 0; _profiles.size(); i++)
    {       
        if (_profiles.at(i) == _activeProfile){
            _profiles.erase(_profiles.begin() + i);
            break;
        }
    }

    // deleting profile will clear the pointers array and decrease ref count
    delete _activeProfile;

    // reverting to default profile

    for (auto &i: _profiles)
    {
        if (i->GetName() == "Default")
            _activeProfile = i;
    }


}


Profile *ProfileController::GetProfileForExe(QString exe)
{
    // if we find a corresponding profile we return a pointer to it
    for (auto &i: _profiles)
    {
        if (i->IsContainingExe(exe))
                return i;

    }
    // else we return nullptr
     return nullptr;

}




}
}
