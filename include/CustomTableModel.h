#ifndef INCLUDECUSTOMTABLEMODEL_H
#define INCLUDECUSTOMTABLEMODEL_H

#include <QStandardItemModel>
#include <QMimeData>
#include <QModelIndex>
#include <QDebug>
#include <QStringListModel>

class CustomTableModel : public QStandardItemModel
{
    Q_OBJECT
public:

    CustomTableModel(QObject *parent = nullptr);
    CustomTableModel(int rows, int columns, QObject *parent = nullptr);

private:
    // Supported drops actions
    Qt::DropActions supportedDropActions() const;

    // Setting the flags on the items
    Qt::ItemFlags flags(const QModelIndex &index) const;

    // Attempting to set custom MIME type
    //QMimeData *QAbstractItemModel::mimeData(const QModelIndexList &indexes) const;

    bool canDropMimeData(const QMimeData *data, Qt::DropAction action,
                                            int row, int column, const QModelIndex &parent);
    bool dropMimeData(const QMimeData *data,
        Qt::DropAction action, int row, int column, const QModelIndex &parent);

signals:
    void draggedOnRow(int);
};

#endif // INCLUDECUSTOMTABLEMODEL_H
//appendRow
//insertRow
