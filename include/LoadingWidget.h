#ifndef LOADINGWIDGET_H
#define LOADINGWIDGET_H

#include <QObject>
#include <QWidget>
#include "ui_loadingJson.h"

class LoadingWidget : public QWidget
{
    Q_OBJECT
public:
    LoadingWidget(QString fileName, QWidget *parent = nullptr);
private:
    Ui::loadingWidget  *ui;
    explicit LoadingWidget(QWidget *parent = nullptr);
    QString _fileName;
signals:

public slots:
    void setMaximum(int);
    void setCurrent(int);
};

#endif // LOADINGWIDGET_H
