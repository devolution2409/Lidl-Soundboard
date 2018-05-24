#include "CustomTableModel.h"

CustomTableModel::CustomTableModel(QObject* parent) : QStandardItemModel(parent)
{
}

CustomTableModel::CustomTableModel(int rows,int columns, QObject* parent) : QStandardItemModel(rows,columns,parent)
{

}
//https://stackoverflow.com/questions/45077778/qtableview-qabstracttablemodel-move-rows-via-dragndrop

Qt::DropActions CustomTableModel::supportedDropActions() const
{
    return Qt::MoveAction;
}

Qt::ItemFlags CustomTableModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags defaultFlags = QStandardItemModel::flags(index);

    if (index.isValid())
       return Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled |  defaultFlags;  // Qt::ItemIsDropEnabled | defaultFlags;
    else
        return Qt::ItemIsDropEnabled | defaultFlags;
}



bool CustomTableModel::canDropMimeData(const QMimeData *data, Qt::DropAction action,
                                       int row, int column, const QModelIndex &parent)
{
    Q_UNUSED(action)
    Q_UNUSED(row)
    //Q_UNUSED(parent)
    Q_UNUSED(data)
    Q_UNUSED(column)

    //qDebug() << parent.row();
//    if (!data->hasFormat("application/vnd.text.list"))
//        return false;
//    // We only want data to be able to be dropped on the first column
//    if (column > 0)
//        return false;

    return true;
}



bool CustomTableModel::dropMimeData(const QMimeData *data,
    Qt::DropAction action, int row, int column, const QModelIndex &parent)
{
    // qDebug() << "Can drop:" << canDropMimeData(data, action, row, column, parent);
    //qDebug() << "Attempting to drop row: " << row << " on row: " << parent.row();

    Q_UNUSED(action);
    Q_UNUSED(*data);
    Q_UNUSED(column);
    Q_UNUSED(row);
    emit draggedOnRow(parent.row());

}




