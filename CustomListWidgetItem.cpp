#include "CustomListWidgetItem.h"

CustomListWidgetItem::CustomListWidgetItem(QListWidget *parent) : QListWidgetItem(parent)
{
    //setting default volume to 100
    _mainVolume = 100;
    _vacVolume  = 100;
}

void CustomListWidgetItem::setMainVolume(float newMainVolume)
{
    _mainVolume = newMainVolume;
}

void CustomListWidgetItem::setVacVolume(float newVacVolume)
{
    _vacVolume  = newVacVolume;
}


CustomListWidgetItem::CustomListWidgetItem(const QString & text, QListWidget * parent, int type)
 : QListWidgetItem(text,parent, type)
{
    _mainVolume = 100;
    _vacVolume  = 100;
}
