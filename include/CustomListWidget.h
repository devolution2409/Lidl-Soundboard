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

/*!
 * \file CustomListWidget.h
 * \brief File describing the CustomListWidget class.
 *
 * \author Devolution
 * \version 1.7.0
 */


/*!
 * \brief The CustomListWidget class : inherits QListWidget
 */
class CustomListWidget : public QListWidget
{
    Q_OBJECT
public:
    /*!
     * \brief CustomListWidget Default constructor.
     * \param parent As usual, the parent.
     */
    explicit CustomListWidget(QWidget *parent = nullptr);

    /*!
     * \brief dragEnterEvent To deal with drag and drop events.
     *
     * We check the event to know if its MIME type are accepted, it it is, we allow the event to continue
     * to dropEvent.
     * \param event The variable containing the event.
     *
     */
    void dragEnterEvent(QDragEnterEvent *event);

    /*!
     * \brief dropEvent If a dragEnterEvent was accepted, it will end in the dropEvent when release.
     * \param e The variable containing the event.
     */
    void dropEvent(QDropEvent *e);

signals:
    /*!
     * \brief fileDragged Signal to send when a file is dragged onto the view.
     *
     * It is intercepted in wrapperproperties.cpp, it will add the file to the wrapper being edited.
     */
    void fileDragged(QString);


};

#endif // CUSTOMLISTWIDGET_H
