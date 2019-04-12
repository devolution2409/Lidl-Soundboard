#include "Profile.h"

#include "soundwrapper.h"

Profile::Profile(QString name, QSet<QString> exe,Profile* parent) : _name(name) , _exeList(exe), _parent(parent)
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
    // OR MB NOT if we just swap a profile for another :)
}

void Profile::AddGame(QString exe)
{
    //TODO IMPLEMENT
}

void Profile::AddSound(std::shared_ptr<SoundWrapper> wrapper)
{
    this->_sounds.push_back(wrapper);
}

void Profile::RemoveSoundFromSharedPtr(std::shared_ptr<SoundWrapper> wrapper)
{
    for (int i = 0; i < _sounds.size();  i++)
    {
        qDebug() << "i:" <<i;
        if  ( _sounds.at(i) == wrapper)
        {
            _sounds.removeAt(i);
            return;
        }
    }
}

void Profile::SwapSound(std::shared_ptr<SoundWrapper> oldWrapper, std::shared_ptr<SoundWrapper> newWrapper)
{
    for (int i = 0; i < _sounds.size();  i++)
    {
        if  ( _sounds.at(i) == oldWrapper)
        {
            qDebug() << "[Profile::SwapSound() Ref count before swap old:" << oldWrapper.use_count();
            qDebug() << "[Profile::SwapSound() Ref count before swap new:" << newWrapper.use_count();

            //_sounds.takeAt(i).swap(newWrapper);
            // ZuluJebaited swap does shit with refcount while removing and inserting doesn't forsenD
            _sounds.removeAt(i);
            _sounds.insert(i,newWrapper);

            qDebug() << "[Profile::SwapSound() Ref count after swap old:" << oldWrapper.use_count();
            qDebug() << "[Profile::SwapSound() Ref count after swap new:" << newWrapper.use_count();
            return;
        }
    }
}

Profile *Profile::GetParent() const
{
    return this->_parent;
}

void Profile::ClearSounds()
{
    this->_sounds.clear();
}

void Profile::SetPttScanCode(int sc)
{
    this->_PTTScanCode= sc;
}

void Profile::SetPttVirtualKey(int vk)
{
    this->_PTTVirtualKey = vk;
}

int Profile::GetPttScanCode() const
{
    return this->_PTTScanCode;
}

int Profile::GetPttVirtualKey() const
{
    return this->_PTTVirtualKey;
}


Profile::~Profile()
{
    for (auto &i: _sounds)
        qDebug() << "[~Profile()] Ref count of _sounds:" << i.use_count();
    _sounds.clear();


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

QVector<std::shared_ptr<SoundWrapper>> Profile::GetSounds() const
{
    return this->_sounds;
}

