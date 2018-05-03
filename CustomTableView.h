#ifndef CUSTOMTABLEVIEW_H
#define CUSTOMTABLEVIEW_H

#include <QWidget>
#include <QTableView>
#include <QDebug>
#include <QFocusEvent>
//#include <QEvent>
//#include <QMouseEvent>
//#include <QModelIndex>
#include <QItemSelection>
class CustomTableView : public QTableView
{
    Q_OBJECT
public:
    explicit CustomTableView(QWidget *parent = nullptr);

private:
   // QWidget * _mainWindow;
   // void focusOutEvent(QFocusEvent* event);
    void selectionChanged(const QItemSelection & selected, const QItemSelection & deselected);

   // bool event(QEvent * e);
signals:
    void disableButtons();
    void enableButtons();

public slots:
};

#endif // CUSTOMTABLEVIEW_H    resultView = new QTableView(this);
