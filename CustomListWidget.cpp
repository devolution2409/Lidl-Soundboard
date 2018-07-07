#include "CustomListWidget.h"

CustomListWidget::CustomListWidget(QWidget *parent) : QListWidget(parent)
{
    // enabling dropping items on this
    this->setAcceptDrops(true);
}

void CustomListWidget::dragEnterEvent(QDragEnterEvent *event)
{
    // If the drop event contains a URI
    if (event->mimeData()->hasUrls())
    {
        bool accept  = true;
        QList<QUrl> urlList = event->mimeData()->urls();

        QMimeDatabase db;

        for (auto i: urlList)
        {
          QMimeType type = db.mimeTypeForFile(i.path());
          qDebug() << type.name();
          if (!( LIDL::SettingsController::GetInstance()->GetSupportedMimeTypes().contains(type.name())))
          {
            accept = false;
            break;
          }
        }


        if (accept)
        event->acceptProposedAction();
    }
    // QListItems have no mime data so we can test it that way (for reorder purposes)
    // ITS A HACK THO forsenT
    // fixed FeelsOkayMan: https://stackoverflow.com/questions/10597444/how-to-accept-drag-and-drop-from-qlistwidget-in-custom-qtextedit
    else if (event->mimeData()->formats() == QStringList("application/x-qabstractitemmodeldatalist"))
        event->acceptProposedAction();

}

void CustomListWidget::dropEvent(QDropEvent *e)
{
    foreach (const QUrl &url, e->mimeData()->urls())
    {

        QString fileName = url.toLocalFile();
        emit fileDragged(fileName);
    }
    // Need to call the parent function afterwards or we can't
    // order the list by drag and drop actions.
    QListWidget::dropEvent(e);
}
