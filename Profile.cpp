#include "Profile.h"

#include "soundwrapper.h"

Profile::Profile(QString name, QSet<QString> exe) : _name(name) , _exeList(exe)
{

}

QString Profile::GetName() const
{
    return this->_name;

}

QSet<QString> Profile::GetGameList() const
{
    return this->_exeList;
}

void Profile::RemoveGame(QString exe)
{
    //TODO IMPLEMENT
}

void Profile::AddGame(QString exe)
{
    //TODO IMPLEMENT
}

void Profile::AddSound(SoundWrapper * wrapper)
{
    this->_sounds.push_back(wrapper);
}

Profile::~Profile()
{
    //deleting those as they aren't GC cause profile are not QObjects
    for (auto &i: _sounds)
        delete i;
}

QString Profile::GetConfigAsString() const
{
    QString temp = "Profile name: " + this->_name + "\nList of exe:";
    for (auto i: _exeList)
    {
        temp += "\n\t" + i;
    }

    return temp;

}

bool Profile::IsContainingExe(QString exe) const
{
    return this->_exeList.contains(exe);
}

QVector<SoundWrapper *> Profile::GetSounds() const
{
    return this->_sounds;
}

