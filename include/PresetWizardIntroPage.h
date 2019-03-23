#ifndef PRESETWIZARDINTROPAGE_H
#define PRESETWIZARDINTROPAGE_H

#include <QObject>
#include <QWidget>
#include <QWizard>
#include <QRadioButton>
#include <QGridLayout>
#include <QLabel>
#include <QComboBox>
#include "Spoiler.h"
#include "PresetController.h"
#include "SfxSettingsWidget.h"
#include <QPushButton>
#include <QMessageBox>
#include <QInputDialog>
class PresetWizardIntroPage : public QWizardPage
{
    Q_OBJECT
public:
    PresetWizardIntroPage(QWidget *parent = 0);
private:
    QGridLayout * _layout;
    QLabel * _pastorLUL;
    QRadioButton* _radioAdd;
    QRadioButton* _radioEdit;

    Spoiler * _spoilerSFX;
    QGridLayout * _spoilerSFXLayout;
    QComboBox * _spoilerSFXcomboBox;

    Spoiler * _presetSFXSpoiler;
    QGridLayout * _presetSFXLayout;
    QComboBox * _presetSFXcomboBox;
 //   QMetaObject::Connection *conn;

    void radioWasClicked();
    void sfxBoxIndexChanged(int index);
    void presetBoxIndexChanged(const QString& name);
    void addWidgets();
    void finished();
    Spoiler *_settingsSpoiler;
    QGridLayout *_settingsLayout;
    // DISTORTION
        SfxSettingsWidget* _distortionWidget; /*!< The widget responsible of the Distortion tab construction*/
    // CHORUS
        SfxSettingsWidget* _chorusWidget; /*!< The widget responsible of the Chorus tab construction*/
    // ECHO
        SfxSettingsWidget* _echoWidget;  /*!< The widget responsible of the Echo tab construction*/
    // COMPRESSOR
        SfxSettingsWidget* _compressorWidget;  /*!< The widget responsible of the Compressor tab construction*/
    // FLANGER
        SfxSettingsWidget* _flangerWidget;  /*!< The widget responsible of the Flanger tab construction*/
    // GARGLE
        SfxSettingsWidget* _gargleWidget;  /*!< The widget responsible of the Gargle tab construction*/
        QLineEdit * _presetName; /*!< The preset name line edit, only shown when creating new preset */

    QPushButton* _finishButton; /*!< The finish button*/
    QPushButton* _cancelButton; /*!< The cancel button*/

};

#endif // PRESETWIZARDINTROPAGE_H
