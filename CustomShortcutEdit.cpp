#include "CustomShortcutEdit.h"
//Calling the parent constructor
CustomShortcutEdit::CustomShortcutEdit(QWidget *parent) : QKeySequenceEdit(parent)
{


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
}

