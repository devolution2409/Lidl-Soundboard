#ifndef CUSTOMLISTWIDGETITEM_H
#define CUSTOMLISTWIDGETITEM_H


#include <QListWidget>
#include <QListWidgetItem>
#include <QDebug>
#include "EnumsAndStructs.h"
#include "lib/bass.h"
class CustomListWidgetItem : public QListWidgetItem
{

public:
    explicit CustomListWidgetItem(QListWidget *parent = nullptr);

    CustomListWidgetItem(const QString & text,float mainVolume = 1.0, float vacVolume =1.0 ,QListWidget * parent = 0, int type = Type);
    CustomListWidgetItem(const QString & text,float mainVolume,float vacVolume, LIDL::SFX sfx ,QListWidget * parent=0, int type = Type);
    void setMainVolume(float);
    void setVacVolume(float);
    float getMainVolume();
    float getVacVolume();


    void setSFXDistortionDefault();
    void setSFXDistortion(LIDL::SFX_DIST_PARAM param, int value);

    void setSFXChorusDefault();
    void setSFXChorus(LIDL::SFX_CHORUS_PARAM param, int value);

 //   void SetDistortionEnabled(bool);


    void SetSFXEnabled(LIDL::SFX_TYPE type, bool enabled);


    LIDL::SFX GetSFX();
private:
    float _mainVolume;
    float _vacVolume;

    LIDL::SFX _sfx;


};

#endif // CUSTOMLISTWIDGETITEM_H
