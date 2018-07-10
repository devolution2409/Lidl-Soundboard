#include "LoadingWidget.h"

LoadingWidget::LoadingWidget(QWidget *parent) : QWidget(parent)
{
    ui = new Ui::loadingWidget;
    ui->setupUi(this);
    this->setWindowFlags(Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint);
}

LoadingWidget::LoadingWidget(QString fileName, QWidget *parent) : LoadingWidget(parent)
{
    _fileName = fileName;

}

void LoadingWidget::setMaximum(int max)
{
    ui->progressBar->setRange(0,max);
}
void LoadingWidget::setCurrent(int value)
{
    int curr = ui->progressBar->value();
    int max = ui->progressBar->maximum();
    float percent = 100 * static_cast<float>(curr)/static_cast<float>(max);
    ui->progressBar->setValue(value);
    ui->label->setText(QString(tr("Opening file: %1, %2 %")).arg(_fileName).arg(percent)) ;
}
