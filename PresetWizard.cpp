#include "PresetWizard.h"

PresetWizard::PresetWizard(QWidget *parent) : QWizard(parent)
{
    this->setWizardStyle(QWizard::ModernStyle);
    this->setOption(QWizard::NoBackButtonOnStartPage, true);
    this->setOption(QWizard::HaveHelpButton, false);
    this->setFixedSize(620,660);
    this->setWindowTitle(tr("LIDL Preset Editor"));
    this->addPage(new PresetWizardIntroPage(this));
}
void PresetWizard::accept()
{
//    QByteArray className = field("className").toByteArray();
//    QByteArray baseClass = field("baseClass").toByteArray();
//    QByteArray macroName = field("macroName").toByteArray();
//    QByteArray baseInclude = field("baseInclude").toByteArray();

//    QString outputDir = field("outputDir").toString();
//    QString header = field("header").toString();
//    QString implementation = field("implementation").toString();
//    ...
    QDialog::accept();
}
