#include "CustomShortcutEdit.h"
//Calling the parent constructor
CustomShortcutEdit::CustomShortcutEdit(QWidget *parent) : QKeySequenceEdit(parent)
{


}

// Calling the parent method too
void CustomShortcutEdit::keyPressEvent(QKeyEvent *e)
{
    QKeySequenceEdit::keyPressEvent(e);
    emit keyPressed();
}
