#ifndef CUSTOMEVENTFILTER_H
#define CUSTOMEVENTFILTER_H

#include <QObject>
#include <QDebug>
#include <QAbstractNativeEventFilter>
#include "windows.h"

class CustomEventFilter :  public QObject, public QAbstractNativeEventFilter
{
    Q_OBJECT
    public:
    bool nativeEventFilter(const QByteArray &eventType, void *message, long *) override;
    CustomEventFilter();
signals:
    void Keydown(int);
};

#endif // CUSTOMEVENTFILTER_H

//last resort solution: set the stderr and stdout to af ile in temp using QTemporaryFile
// and use QtConcurrent to read the file in another thread feels dank man
