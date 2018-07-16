#ifndef PRESETWIZARD
#define PRESETWIZARD

#include <QObject>
#include <QWidget>
#include <QWizard>
#include "PresetWizardIntroPage.h"
#include "PresetController.h"

class PresetWizard : public QWizard
{
    Q_OBJECT
public:
    PresetWizard(QWidget *parent = 0);
    void accept() override;
};

#endif // PRESETWIZARD
