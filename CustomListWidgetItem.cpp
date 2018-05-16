#include "CustomListWidgetItem.h"

CustomListWidgetItem::CustomListWidgetItem(QListWidget *parent) : QListWidgetItem(parent)
{
    //setting default volume to 100%
    _mainVolume = 1;
    _vacVolume  = 1;
}

void CustomListWidgetItem::setMainVolume(float newMainVolume)
{
    _mainVolume = newMainVolume;
}

void CustomListWidgetItem::setVacVolume(float newVacVolume)
{
    _vacVolume  = newVacVolume;
}


CustomListWidgetItem::CustomListWidgetItem(const QString & text,float mainVolume,
                                           float vacVolume,QListWidget * parent, int type)
    : QListWidgetItem(text,parent,type)
{
    _mainVolume = mainVolume;
    _vacVolume  = vacVolume;
}

float CustomListWidgetItem::getMainVolume()
{
    return _mainVolume;
}
float CustomListWidgetItem::getVacVolume()
{
    return _vacVolume;
}
