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
        bool accept = true;
        foreach (const QUrl &url, event->mimeData()->urls())
        {
            QFileInfo info = QFile(url.toLocalFile());
            //if one of the file isn't a song we reject
            if (!(info.suffix() == "wav" || info.suffix() == "wave" || info.suffix() == "mp3"))
                accept = false;
        }
        if (accept)
            event->acceptProposedAction();
    }
    // QListItems have no mime data so we can test it that way...
    // ITS A HACK THO forsenT
    else if (event->mimeData()->text() == "")
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
