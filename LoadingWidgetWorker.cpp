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

    qDebug() << "finished";
    emit finished();

}

