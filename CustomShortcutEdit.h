#ifndef CUSTOMSHORTCUTEDIT_H
#define CUSTOMSHORTCUTEDIT_H

#include <QWidget>
#include <QKeySequenceEdit>
//#include <QDebug>
#include <QWidget>
#include <QKeyEvent>





class CustomShortcutEdit : public QKeySequenceEdit
{
     Q_OBJECT
public:
     explicit CustomShortcutEdit(QWidget *parent = nullptr);
private:
     void keyPressEvent(QKeyEvent *e);
signals:
     void keyPressed();
};

#endif // CUSTOMSHORTCUTEDIT_H
