#ifndef CUSTOMSHORTCUTEDIT_H
#define CUSTOMSHORTCUTEDIT_H

#include <QWidget>
#include <QKeySequenceEdit>
//#include <QDebug>
#include <QWidget>
#include <QKeyEvent>

#include <QDebug>


/*! Custom KeySequenceEdit so we are able to fetch the virtualKey and scancode integers from the event.
 * (see https://code.woboq.org/qt5/qtbase/src/widgets/widgets/qkeysequenceedit.cpp.html)
 *  Hover and focus color are dealt in CSS (QLineEdit:Hover,QLineEdit:Focus)
 */




class CustomShortcutEdit : public QKeySequenceEdit
{
     Q_OBJECT
public:
     explicit CustomShortcutEdit(QWidget *parent = nullptr);
     int getVirtualKey();
     int getScanCode();
     QString getText();

     void setVirtualKey(int);
     void setScanCode(int);
private:
     int _virtualKey;
     int _scanCode;
     void keyPressEvent(QKeyEvent *e);

     //void focusInEvent(QFocusEvent* e);
     //void focusOutEvent(QFocusEvent* e);

signals:
     void keyPressed();
     void scanCodeChanged(int);
     void virtualKeyChanged(int);
public slots:
     void sendSignal();
};

#endif // CUSTOMSHORTCUTEDIT_H
