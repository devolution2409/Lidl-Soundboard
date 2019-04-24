#ifndef UPDATECHECKER_H
#define UPDATECHECKER_H

#include <QObject>
#include <QWidget>
#include <QDebug>
#include <QNetworkAccessManager>
#include <QHttpPart>
#include <QHttpMultiPart>
#include <QNetworkReply>
#include <QStringLiteral>
#include <QProcess>
namespace LIDL {

namespace Controller {



class UpdateChecker : public QObject
{
    Q_OBJECT
public:
    explicit UpdateChecker(QObject *parent = nullptr);

signals:

public slots:
};
}
}
#endif // UPDATECHECKER_H
