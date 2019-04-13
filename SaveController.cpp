#include "SaveController.h"


namespace LIDL {

namespace Controller {

SaveController* SaveController::self = nullptr;

SaveController::SaveController()
{

}

SaveController *SaveController::GetInstance()
{
    if (self == nullptr)
        self = new SaveController();
    return self;

}

void SaveController::WriteSaveFile(QString which) const
{
    QJsonObject obj;
    QJsonArray profiles;

    for (auto &i: LIDL::Controller::ProfileController::GetInstance()->GetProfiles())
    {
        profiles.append(i->GetProfileAsObject());

    }

    obj.insert("Profiles",profiles);

    qDebug() << QJsonDocument(obj).toJson();
}

}

}
