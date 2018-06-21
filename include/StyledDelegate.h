#ifndef STYLEDDELEGATE_H
#define STYLEDDELEGATE_H

#include <QStyledItemDelegate>
#include <QFontMetrics>
#include <QString>
#include <QStringList>
#include <QStyle>
#include <QApplication>
#include <QDebug>
#include <QItemDelegate>
#include <QPushButton>


/* Tweaked a bit from:
 * https://github.com/lowbees/Hover-entire-row-of-QTableView
 * Thanks this Lobeews guy a lot !
 * */

namespace LIDL{
    enum HoverBehavior {
        HoverItems,
        HoverRows,
        HoverColumns
    };
}




class StyledDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit StyledDelegate(LIDL::HoverBehavior behavior = LIDL::HoverBehavior::HoverRows, QObject *parent = 0);
    void paint(QPainter *painter, const QStyleOptionViewItem &option,
               const QModelIndex &index) const;



//    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
//                            const QModelIndex &index) const override;

//    void setEditorData(QWidget *editor, const QModelIndex &index) const override;
//    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;

//    void updateEditorGeometry(QWidget *editor,
//        const QStyleOptionViewItem &option, const QModelIndex &index) const override;


private:
    LIDL::HoverBehavior hoverBehavior;
};

#endif // STYLEDDELEGATE_H
