#include "CustomTableView.h"


CustomTableView::CustomTableView(QWidget *parent) : QTableView(parent)
{
    //_mainWindow = parent;
    this->setAttribute(Qt::WA_TransparentForMouseEvents,false);
    this->setEditTriggers(QAbstractItemView::NoEditTriggers);
    /* If its value is true, the selected data will overwrite the existing item data when dropped,
     * while moving the data will clear the item.
     * If its value is false, the selected data will be inserted as a new item when the data is dropped.
     * When the data is moved, the item is removed as well.
     *
     * The default value is false, as in the QListView and QTreeView subclasses.
     * In the QTableView subclass, on the other hand, the property has been set to true.
    */
    this->setDragDropOverwriteMode(false);
    this->setDragEnabled(true);
    this->setAcceptDrops(true);
    this->setDropIndicatorShown(true);
//    this->viewport()->setAcceptDrops(true);
    this->setDragDropMode(QAbstractItemView::InternalMove);
    this->setDefaultDropAction(Qt::MoveAction);

    // hiding the vertical headers on the left side
    this->verticalHeader()->hide();
    // Setting so the user can only select row forsenE
    this->setSelectionBehavior(QAbstractItemView::SelectRows);
    this->setSelectionMode(QAbstractItemView::SingleSelection);
    _delegate = new StyledDelegate(LIDL::HoverBehavior::HoverRows ,this);
    this->setItemDelegate(_delegate);

}

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

