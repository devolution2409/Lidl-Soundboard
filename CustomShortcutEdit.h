#ifndef CUSTOMSHORTCUTEDIT_H
#define CUSTOMSHORTCUTEDIT_H

#include <QWidget>
#include <QKeySequenceEdit>
//#include <QDebug>
#include <QWidget>
#include <QKeyEvent>

#include <QDebug>




class CustomShortcutEdit : public QKeySequenceEdit
{
     Q_OBJECT
public:
     explicit CustomShortcutEdit(QWidget *parent = nullptr);
     int getVirtualKey();
     int getScanCode();
private:
     int _virtualKey;
     int _scanCode;
     void keyPressEvent(QKeyEvent *e);
signals:
     void keyPressed();
     void scanCodeChanged(int);

public slots:
     void sendSignal();
};

#endif // CUSTOMSHORTCUTEDIT_H
