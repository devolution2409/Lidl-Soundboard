#ifndef SAVECONTROLLER_H
#define SAVECONTROLLER_H

#include <QString>

#include "ProfileController.h"
#include "Profile.h"
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include "SettingsController.h"
#include <QFileDialog>
#include <QString>
#include <QObject>
#include <QRegExp>
#include "EnumsAndStructs.h"
#include "soundwrapper.h"

namespace LIDL {

namespace Controller {

class SaveController : public QObject
{
    Q_OBJECT
public:
    static SaveController* GetInstance();


    /*!
     * \brief GenerateSaveFile
     * \return
     */
    QJsonObject GenerateSaveFile() const;

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

    /*!
     * \brief SaveAs
     */
    void SaveAs(QString fileName = "");


    /*!
     * \brief Save
     */
    void Save();

    /*!
     * \brief SetStopKeyName
     */
    void SetStopKeyName(QString name);

    /*!
     * \brief SetStopVirtualKey
     * \param vk
     */
    void SetStopVirtualKey( unsigned int vk);

    /*!
     * \brief OpenSaveFile
     */
    void OpenSaveFile();

    //void CompareSaves();
    // Generate hash of json and compare it :QString blah = QString(QCryptographicHash::hash(("myPassword"),QCryptographicHash::Md5).toHex())


private:
        SaveController();
        static SaveController *self;
        QString _mainOutputDevice;

        QString _vacOutputDevice;

        QString _saveName;

        QString _stopKeyName;
        unsigned int _stopVirtualKey;
signals:
   void Clear();
   void SetDevices(QString main,QString vac);


};

} // end namespace Controller
} //end namespace LIDL

#endif // SAVECONTROLLER_H010
