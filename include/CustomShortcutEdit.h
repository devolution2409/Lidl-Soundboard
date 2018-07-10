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

/*!
 * \file CustomShortcutEdit.h
 * \brief Code of the mainUI.
 *
 * \author Devolution
 * \version 1.7.0
 * \since 0.42
 *
 */

/*!
 * \brief The CustomShortcutEdit class
 *
 * \version 1.7.0
 * \since 0.42
 */
class CustomShortcutEdit : public QKeySequenceEdit
{
     Q_OBJECT
public:
    /*!
      * \brief CustomShortcutEdit
      * \param parent
      */
     explicit CustomShortcutEdit(QWidget *parent = nullptr);
    /*!
      * \brief getVirtualKey
      * \return
      */
     int getVirtualKey();
     /*!
      * \brief getScanCode
      * \return
      */
     int getScanCode();

     /*!
      * \brief getText
      * \return
      */
     QString getText();
     /*!
      * \brief setVirtualKey
      */
     void setVirtualKey(int);
     /*!
      * \brief setScanCode
      */
     void setScanCode(int);
private:
     /*!
      * \brief _virtualKey
      */
     int _virtualKey;
     /*!
      * \brief _scanCode
      */
     int _scanCode;
     /*!
      * \brief keyPressEvent
      * \param e
      */
     void keyPressEvent(QKeyEvent *e);

     //void focusInEvent(QFocusEvent* e);
     //void focusOutEvent(QFocusEvent* e);

signals:
     /*!
      * \brief keyPressed
      */

     void keyPressed();
     /*!
      * \brief scanCodeChanged
      */
     void scanCodeChanged(int);
     /*!
      * \brief virtualKeyChanged
      */
     void virtualKeyChanged(int);
public slots:
     /*!
      * \brief sendSignal This slot is called whenever the editing is done, could be replaced by lambda but i'm lazy today
      */
     void sendSignal();
};

#endif // CUSTOMSHORTCUTEDIT_H
