#ifndef SAVECONTROLLER_H
#define SAVECONTROLLER_H
#include <QString>

#include "ProfileController.h"
#include "Profile.h"
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include "SettingsController.h"

namespace LIDL {

namespace Controller {

class SaveController
{
public:
    static SaveController* GetInstance();


    /*!
     * \brief WriteSaveFile
     * \param which
     */
    void WriteSaveFile(QString which) const;

    /*!
     * \brief SetMainOutputDevice
     * Only used to remember which device was set, in order to save it to the JSON
     */
    void SetMainOutputDevice(QString);

    /*!
     * \brief SetVacOutputDevice
     * Only used to remember which device was set, in order to save it to the JSON
     */
    void SetVacOutputDevice(QString);

    /*!
     * \brief GetMainOutputDevice
     * \return
     */
    QString GetMainOutputDevice() const;

    /*!
     * \brief GetVacOutputDevice
     * \return
     */
    QString GetVacOutputDevice() const;
private:
        SaveController();
        static SaveController *self;
        QString _mainOutputDevice;

        QString _vacOutputDevice;


};

} // end namespace Controller
} //end namespace LIDL

#endif // SAVECONTROLLER_H010
