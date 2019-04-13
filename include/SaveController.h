#ifndef SAVECONTROLLER_H
#define SAVECONTROLLER_H
#include <QString>

#include "ProfileController.h"
#include "Profile.h"
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>


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

private:
        SaveController();
        static SaveController *self;
};

} // end namespace Controller
} //end namespace LIDL

#endif // SAVECONTROLLER_H010
