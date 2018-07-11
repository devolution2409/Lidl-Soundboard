#ifndef LOADINGWIDGETWORKER_H
#define LOADINGWIDGETWORKER_H

#include <QObject>
#include <QStandardItem>
#include <QDebug>
#include <QTimer>
class LoadingWidgetWorker : public QObject
{
    Q_OBJECT
public:
    LoadingWidgetWorker( QVector<QList< QStandardItem* >> * ptrData,int max,QObject *parent = nullptr);
    ~LoadingWidgetWorker();

signals:

    void finished();
    void setValue(int value);

private:

    QVector<QList< QStandardItem* >> * _ptrData;
    int _maximum;
public slots:
    void process();
};

#endif // LOADINGWIDGETWORKER_H
