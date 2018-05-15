#include "FancySlider.h"

#include <QStyleOptionSlider>
#include <QToolTip>

FancySlider::FancySlider(QWidget * parent)
    : QSlider(parent)
{
}

FancySlider::FancySlider(Qt::Orientation orientation, QWidget * parent)
    : QSlider(orientation, parent)
{
}

void FancySlider::sliderChange(QAbstractSlider::SliderChange change)
{
    QSlider::sliderChange(change);

    if (change == QAbstractSlider::SliderValueChange )
    {
        QStyleOptionSlider opt;
        initStyleOption(&opt);

        QRect sr = style()->subControlRect(QStyle::CC_Slider, &opt, QStyle::SC_SliderHandle, this);
        //QPoint bottomRightCorner = sr.bottomLeft();
        QPoint bottomRightCorner = sr.topRight();
        QToolTip::showText(mapToGlobal( QPoint( bottomRightCorner.x() , bottomRightCorner.y() -40 ) ), QString::number(value()), this);
    }
}
