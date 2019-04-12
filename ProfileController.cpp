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

}

std::vector<Profile *> ProfileController::GetProfiles() const
{
    return this->_profiles;
}

void  ProfileController::ManualGameConfigurationChanged(const QString &name)
{
    // searching for the profile with the correct name
    for (auto &i: _profiles)
    {
        if (i->GetName() == name)
        {
            this->_activeProfile = i;
            return;
        }

    }


    qDebug() << "Please implement ManualGameConfiguraitonChanged me in ProfileController" << name;
}

Profile *ProfileController::GetActiveProfile() const
{
    return this->_activeProfile;
}

void ProfileController::AddProfile(Profile* profile,LIDL::PROFILE_COPY_MODE copyMode)
{
    // pushing the profile in the array
    _profiles.push_back(profile);

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


    emit ProfileConfigurationChanged();
    qDebug().noquote() << profile->GetConfigAsString();


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
