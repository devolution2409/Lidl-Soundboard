#ifndef STYLEDDELEGATE_H
#define STYLEDDELEGATE_H

#include <QStyledItemDelegate>




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
private:
    LIDL::HoverBehavior hoverBehavior;
};

#endif // STYLEDDELEGATE_H
