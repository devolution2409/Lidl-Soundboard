#ifndef CUSTOMTABLEVIEW_H
#define CUSTOMTABLEVIEW_H

#include <QWidget>
#include <QTableView>
//#include <QDebug>
#include <QFocusEvent>
//#include <QEvent>
//#include <QMouseEvent>
//#include <QModelIndex>
#include <QItemSelection>
//#include "CustomTableViewDelegate.h"
//#include <QHoverEvent>
//#include <QDrag>
//#include <QRubberBand>

#include <QHeaderView>
#include "StyledDelegate.h"

class CustomTableView : public QTableView
{
    Q_OBJECT
public:
    explicit CustomTableView(QWidget *parent = nullptr);

private:
   // QWidget * _mainWindow;
   // void focusOutEvent(QFocusEvent* event);
    void selectionChanged(const QItemSelection & selected, const QItemSelection & deselected);
    //Reimplemenation for hovering
    void onMouseHover(QEvent *event);

    //bool eventFilter(QObject* object, QEvent* event);

    StyledDelegate * _delegate;

    int _currentHovered;
   // QRubberBand * _rubberBand;
   // bool event(QEvent * e);
signals:
    void disableButtons();
    void enableButtons();
    void hoverIndexChanged(QModelIndex);
public slots:
    //void drawHoverWidget(QModelIndex);
};

#endif // CUSTOMTABLEVIEW_H    resultView = new QTableView(this);

