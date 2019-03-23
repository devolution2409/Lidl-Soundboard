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
#include <type_traits> // is same

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


    void AddPreset(QString name, BASS_DX8_CHORUS preset, bool overwrite = false);
    void AddPreset(QString name, BASS_DX8_DISTORTION preset, bool overwrite = false);
    void AddPreset(QString name, BASS_DX8_COMPRESSOR preset, bool overwrite = false);
    void AddPreset(QString name, BASS_DX8_ECHO preset, bool overwrite = false);
    void AddPreset(QString name, BASS_DX8_FLANGER preset, bool overwrite = false);
    void AddPreset(QString name, BASS_DX8_GARGLE preset, bool overwrite = false);

    /*!
     * \brief Declares the template we are going to specialize
     *
     * Used as syntax flavor, to call only GetPreset<PresetName> instead of GetPresetName weSmart
     */
    template<typename BASS_SFX_STRUCT>
    std::map<int,int> GetPreset(QString name);


private:
    /*!
     * \brief PresetController default constructor.
     * \param parent
     */
    explicit PresetController(QObject *parent = nullptr);

    static PresetController * self; /*!< A pointer to the only existing PresetController.*/


    QMap<QString , BASS_DX8_DISTORTION  > _distortionPreset;
    QMap<QString , BASS_DX8_ECHO        > _echoPreset;
    QMap<QString , BASS_DX8_CHORUS      > _chorusPreset;
    QMap<QString , BASS_DX8_COMPRESSOR  > _compressorPreset;
    QMap<QString , BASS_DX8_FLANGER     > _flangerPreset;
    QMap<QString , BASS_DX8_GARGLE      > _garglePreset;


signals:

public slots:
}; // end class




}// end namespace lidl




#endif // PRESETCONTROLLER_H
