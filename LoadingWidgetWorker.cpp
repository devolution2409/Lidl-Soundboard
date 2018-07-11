#include "LoadingWidgetWorker.h"



LoadingWidgetWorker::LoadingWidgetWorker( QVector<QList< QStandardItem* >> * ptrData,int max,QObject *parent)
 : QObject(parent)
{
    this->_ptrData = ptrData;
    this->_maximum = max;
}

LoadingWidgetWorker::~LoadingWidgetWorker()
{

}


void LoadingWidgetWorker::process()
{
    do{
            qDebug() <<  _ptrData->size() << "/" << _maximum;
        emit setValue(_ptrData->size());
    }while (_ptrData->size() != _maximum);
    emit setValue(100);
    // 0.1 sec timer so we can see 100%.
    // NOW I AM COMPLETE.
    QTimer::singleShot(100,this,[=]{ emit finished();});

}

