#ifndef CUSTOMLISTWIDGET_H
#define CUSTOMLISTWIDGET_H

#include <QObject>
#include <QWidget>
#include <QListWidget>
#include <QDebug>
#include <QDropEvent>
#include <QMimeData>
#include <QFileInfo>
#include <QPoint>
#include <QMouseEvent>
#include <QDrag>
#include "SettingsController.h"
#include <QMimeDataBase>
#include <QMimeType>
class CustomListWidget : public QListWidget
{
    Q_OBJECT
public:
    explicit CustomListWidget(QWidget *parent = nullptr);
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *e);

signals:
    void fileDragged(QString);

public slots:

private:
    QPoint _dragStartPosition;

};

#endif // CUSTOMLISTWIDGET_H
