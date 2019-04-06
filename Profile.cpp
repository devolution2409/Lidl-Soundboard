#include "Profile.h"



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

