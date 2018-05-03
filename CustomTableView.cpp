#include "CustomTableView.h"


CustomTableView::CustomTableView(QWidget *parent) : QTableView(parent)
{
    //_mainWindow = parent;
  //  this->setAttribute(Qt::WA_TransparentForMouseEvents,false);
}

//void CustomTableView::focusOutEvent(QFocusEvent* event)
//{
//   // qDebug() << event->type();

//}
//void CustomTableView::currentChanged(const QModelIndex & current, const QModelIndex & previous)
//{
//    qDebug() << current.isValid() << "   " << previous.isValid();


//}

void CustomTableView::selectionChanged(const QItemSelection & selected, const QItemSelection & deselected)
{
    // calling parent method so that the view get actualized accordingly
    QTableView::selectionChanged(selected,deselected);
    // and adding our code
    // if new selection is empty it means we didn't click on an item, therefore we disable edit and delete
    if (selected.isEmpty())
        emit disableButtons();
    else //we clicked on valid item
        emit enableButtons();

}



//bool CustomTableView::event(QEvent *e)
//{
//   // qDebug() << e->type();
//  //  return true;
//}
