#ifndef PRESETCONTROLLER_H
#define PRESETCONTROLLER_H

#include <QObject>
#include <QWidget>
#include <QWizard>
#include <map>
#include <vector>
#include <QString>
#include "lib/bass.h"
#include <QStringList>
#include "EnumsAndStructs.h"
#include <QDebug>
#include <QMap>

namespace LIDL {

/*!
  * \brief The PresetController singleton.
  *
  * Deals with storing the presets and acessing them.
  * \author Devolution
  * \version 1.8.0
  *
  * \since 1.8.0
  */
class PresetController : public QObject
{
    Q_OBJECT
public:
    /*!
     * \brief GetInstance, returns the instance of the controller, creating one if it doesn't exist.
     */
    static PresetController * GetInstance();

    /*!
     * \brief GetExistingPresetsList
     * \param type The SFX_TYPE we wanna get the preset list.
     * \return The list of available presets
     */
    QStringList GetExistingPresetsList(LIDL::SFX_TYPE type) const;

    QStringList GetExistingSFXList() const;

    template<LIDL::SFX_TYPE, class Param>
    int AddPreset(QString name, Param preset);


private:
    /*!
     * \brief PresetController default constructor.
     * \param parent
     */
    explicit PresetController(QObject *parent = nullptr);

    static PresetController * self; /*!< A pointer to the only existing PresetController.*/


    QMap<QString , SFX_DIST_PARAM       > _distortionPreset;
    QMap<QString , SFX_ECHO_PARAM       > _echoPreset;
    QMap<QString , SFX_CHORUS_PARAM     > _chorusPreset;
    QMap<QString , SFX_COMPRESSOR_PARAM > _compressorPreset;
    QMap<QString , SFX_FLANGER_PARAM    > _flangerPreset;
    QMap<QString , SFX_GARGLE_PARAM     > _garglePreset;


signals:

public slots:



}; // end class
}// end namespace lidl
// Template :feelsokayman:
template<LIDL::SFX_TYPE Type, class Param>
int LIDL::PresetController::AddPreset(QString name, Param preset)
{
    switch (Type){
        case LIDL::SFX_TYPE::DISTORTION:
                if (_distortionPreset.find(name) != _distortionPreset.constEnd())
                    return 1; // key already exists
                _distortionPreset.insert(name,preset);
                return 0;
                break;
    case LIDL::SFX_TYPE::ECHO:
            if (_echoPreset.find(name) != _echoPreset.constEnd())
                return 1; // key already exists
            _echoPreset.insert(name,preset);
            return 0;
            break;
    case LIDL::SFX_TYPE::CHORUS:
            if (_chorusPreset.find(name) != _chorusPreset.constEnd())
                return 1; // key already exists
            _chorusPreset.insert(name,preset);
            return 0;
            break;
    case LIDL::SFX_TYPE::COMPRESSOR:
            if (_compressorPreset.find(name) != _compressorPreset.constEnd())
                return 1; // key already exists
            _compressorPreset.insert(name,preset);
            return 0;
            break;
    case LIDL::SFX_TYPE::FLANGER:
            if (_flangerPreset.find(name) != _flangerPreset.constEnd())
                return 1; // key already exists
            _flangerPreset.insert(name,preset);
            return 0;
            break;
    case LIDL::SFX_TYPE::GARGLE:
            if (_garglePreset.find(name) != _garglePreset.constEnd())
                return 1; // key already exists
            _garglePreset.insert(name,preset);
            return 0;
            break;
    }
}



#endif // PRESETCONTROLLER_H
