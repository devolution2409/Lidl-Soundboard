#ifndef SFXSETTINGSWIDGET_H
#define SFXSETTINGSWIDGET_H

#include <QWidget>
#include <QVector>
#include "SliderSpin.h"
#include <QScrollArea>
#include <QLabel>
#include <QCheckBox>
#include <QDebug>
class SfxSettingsWidget : public QScrollArea
{
    Q_OBJECT
public:
    explicit SfxSettingsWidget(QWidget *parent = nullptr);
    SfxSettingsWidget(QString sfxName, QWidget *parent = nullptr);
    void addSlider(QString name="",int min = 0, int max=100, QString suffix = "",QString prefix ="" );

private:
    QWidget *_container;
    QGridLayout* _layout;
  //  QString _sfxName;
    QVector<SliderSpin *> _sliders;
    QCheckBox *_checkbox;

signals:
    void valueChanged(int comboBoxIndex, int newValue);

public slots:
};

#endif // SFXSETTINGSWIDGET_H
