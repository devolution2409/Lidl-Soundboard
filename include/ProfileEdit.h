#ifndef PROFILEEDIT_H
#define PROFILEEDIT_H

#include <QObject>
#include <QDialog>
#include <QWidget>
#include <ui_ProfileEdit.h>

class ProfileEdit : public QDialog
{
    Q_OBJECT
public:
    explicit ProfileEdit(QWidget *parent = nullptr);

private:
    Ui::ProfilEdit * ui;
signals:

public slots:
};

#endif // PROFILEEDIT_H
