#include "CustomShortcutEdit.h"
//Calling the parent constructor
CustomShortcutEdit::CustomShortcutEdit(QWidget *parent) : QKeySequenceEdit(parent)
{
    this->setFocusPolicy(Qt::ClickFocus);
    _virtualKey = -1;
    _scanCode = -1;
    connect(this,SIGNAL(editingFinished()),this,SLOT(sendSignal()));
}

// Calling the parent method too
void CustomShortcutEdit::keyPressEvent(QKeyEvent *e)
{
    // Custom signal to force the user to wait 1sec before being able to press done.

    // Forcing only ONE shortcut
    // Thanks Stack Overflow.
    // https://stackoverflow.com/questions/30006562/limit-qkeysequence-qkeysequenceedit-to-only-one-shortcut
    QKeySequence seq(QKeySequence::fromString(keySequence().toString().split(", ").first()));
    this->setKeySequence(seq);
    // how ever we have to reimplement the 1 sec timer because it's broken forsenT
    //timerEvent(); probably. too lazy to look at now
    // fixed by swapping the order of the operations forsenE
    QKeySequenceEdit::keyPressEvent(e);
    emit keyPressed();

    // Since the last key pressed will always be the one the shortcut use, we store it
    // if it's not ctrl, alt, altgrv or shift
    if ( (e->key() != Qt::Key_Alt)      &&
         (e->key() != Qt::Key_Control)  &&
         (e->key() != Qt::Key_AltGr)    &&
         (e->key() != Qt::Key_Shift)
        )
    {
        _virtualKey =  e->nativeVirtualKey();
        _scanCode    = e->nativeScanCode();
    }
    qDebug() << "new scancode:" << _scanCode << " new virtual key" << _virtualKey;

}

int CustomShortcutEdit::getVirtualKey()
{
    return _virtualKey;
}
int CustomShortcutEdit::getScanCode()
{
    return _scanCode;
}
// to set pause button when we open and PTT aswell
void CustomShortcutEdit::setVirtualKey(int vk)
{
    this->_virtualKey = vk;
    emit virtualKeyChanged(_virtualKey);
}

// to set pause button when we open
void CustomShortcutEdit::setScanCode(int sc)
{
    this->_scanCode = sc;
   emit scanCodeChanged(_scanCode);

}


// This slot is called whenever the editing is done
void CustomShortcutEdit::sendSignal()
{
   this->clearFocus();
   emit virtualKeyChanged(_virtualKey);
   emit scanCodeChanged(_scanCode);
}


//used in wrapper properties to actually check if a modifier was used or not
QString CustomShortcutEdit::getText()
{
    // trying to accoutn for numpads
    if (this->_virtualKey <= 0x69 && this->_virtualKey >= 0x60)
    {
        //the number stored here is, in fact, from numpad. Thanks qt for not telling forsenD
        // splitting over + in case there is modifiers, using string stream
        /*
        std::istringstream ss(this->keySequence().toString().toStdString());
        std::string token;

        while(std::getline(ss, token, '+')) {
            qDebug() << token.c_str() << '\n';
        }
        */

        // or more simply, just use reverse find to find the last + and add the text there
        std::string text = this->keySequence().toString().toStdString();
        std::size_t found = text.rfind("+");
        // if it has at least one modifier, we insert right after it
         if (found!=std::string::npos)
         {
            // qDebug() << "string: " << text.c_str() << " pos:" << found;
            text.insert(found+1,"Numpad ");
         }
         else // we prepend
         {
             text = "Numpad " + text;
         }
         qDebug() << text.c_str();
         return QString::fromStdString(text);


    }
    return this->keySequence().toString();
}
