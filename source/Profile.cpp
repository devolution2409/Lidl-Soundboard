#include "Profile.h"

#include "soundwrapper.h"

Profile::Profile(QString name, QSet<QString> exe, Profile *parent,
                 QVector<std::shared_ptr<SoundWrapper> > wrappers,
                 int PttVirtualKey, int PttScanCode, QKeySequence PttKeySequence)
  : _name(name) , _exeList(exe), _parent(parent), _sounds(wrappers), _PTTVirtualKey(PttVirtualKey),
    _PTTScanCode(PttScanCode),_PTTKeySequence(PttKeySequence)
{

}


QString Profile::GetName() const
{
    return this->_name;

}

void Profile::SetName(QString newName)
{
    this->_name = newName;
}

QSet<QString> Profile::GetGameList() const
{
    return this->_exeList;
}

void Profile::SwapGames(QSet<QString> newListHYPERBRUH)
{
    this->_exeList = newListHYPERBRUH;
}



void Profile::AddSound(std::shared_ptr<SoundWrapper> wrapper)
{
    this->_sounds.push_back(wrapper);
}

void Profile::RemoveSoundFromSharedPtr(const std::shared_ptr<SoundWrapper>& wrapper)
{
    for (int i = 0; i < _sounds.size();  i++)
    {
        qDebug() << "i:" <<i;
        if  ( _sounds.value(i) == wrapper)
        {
            _sounds.value(i).reset();
            _sounds.removeAt(i);
            break;
        }
    }
    qDebug() << "testing unicity here:" << wrapper.unique();

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

void Profile::SetPttKeySequence(QKeySequence sequence)
{
    this->_PTTKeySequence.swap(sequence);
}

QKeySequence Profile::GetPttKeySequence() const
{
    return this->_PTTKeySequence;
}

QJsonObject Profile::GetProfileAsObject() const
{
    QJsonObject obj;

    obj.insert("Profile Name", this->_name);

    QJsonObject pttKey;
        pttKey.insert("Key Name",this->_PTTKeySequence.toString());
        pttKey.insert("VirtualKey" ,this->_PTTVirtualKey);
        pttKey.insert("ScanCode"   ,this->_PTTScanCode);
        obj.insert("Push To Talk Key",pttKey);

    QJsonArray sounds;

    for (auto &j: _sounds){
        sounds.append(j->GetWrapperAsObject());
    }

    obj.insert("Sounds" ,sounds);

    QJsonArray exe;
    for (auto &i: _exeList){
        exe.append(i);
    }

    obj.insert("Executables",exe);

    // checking for parent

    if (this->_parent != nullptr){
        obj.insert("Parent",this->_parent->GetName());
    };

    return obj;

}


Profile::~Profile()
{
    // clearing the array of sounds so the ref count decreases
    qDebug() << "[Profile::~Profile()] started destruction of profile: " << this->_name;
    _sounds.clear();
    qDebug() << "[Profile::~Profile()] ended nam";
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

