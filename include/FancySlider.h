#ifndef FANCYSLIDER_H
#define FANCYSLIDER_H

#include <QSlider>


// https://stackoverflow.com/questions/18383885/qslider-show-min-max-and-current-value
// Sebastien Senegas at github

class FancySlider : public QSlider
{
    Q_OBJECT
public:
    explicit FancySlider(QWidget *parent = 0);
    explicit FancySlider(Qt::Orientation orientation, QWidget *parent = 0);

protected:
    virtual void sliderChange(SliderChange change);
};

#endif // FANCYSLIDER_H
