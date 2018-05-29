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
    void setMainVolume(float);
    void setVacVolume(float);
    float getMainVolume();
    float getVacVolume();

   // void setSFXDistortion(float gain = -18, float edge = 15, float centerFrequency=2400, float postEQBandwidth =2400, float preLowpassCutoff =8000  );
    // for DEFAULT
    void setSFXDistortionDefault();
    // to set value
    void setSFXDistortion(LIDL::SFX_DIST_PARAM param, int value);
    //


    void SetDistortionEnabled(bool);
    bool GetDistortionEnabled();
    BASS_DX8_DISTORTION GetDistortionParams();
    LIDL::SFX GetSFX();
private:
    float _mainVolume;
    float _vacVolume;

    LIDL::SFX _sfx;


};

#endif // CUSTOMLISTWIDGETITEM_H
