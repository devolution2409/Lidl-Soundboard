#ifndef CUSTOMLISTWIDGETITEM_H
#define CUSTOMLISTWIDGETITEM_H


#include <QListWidget>
#include <QListWidgetItem>
#include <QDebug>
class CustomListWidgetItem : public QListWidgetItem
{

public:
    explicit CustomListWidgetItem(QListWidget *parent = nullptr);
    CustomListWidgetItem(const QString & text, QListWidget * parent = 0, int type = Type);
    void setMainVolume(float);
    void setVacVolume(float);
    float getMainVolume();
    float getVacVolume();
private:
    float _mainVolume;
    float _vacVolume;

};

#endif // CUSTOMLISTWIDGETITEM_H
