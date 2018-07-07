#ifndef SLIDERSPIN_H
#define SLIDERSPIN_H

#include <QWidget>
#include <QSlider>
#include <QSpinBox>
#include <QGridLayout>

class SliderSpin : public QWidget
{
    Q_OBJECT
public:
    explicit SliderSpin(QWidget *parent = nullptr);
    SliderSpin(int min = 0, int max = 100, QString suffix = "", QString prefix ="", QWidget *parent = nullptr);
    /*!
     * \brief setRange Sets the range of the slider and spin box.
     * \param min Minimum value (int).
     * \param max Maximum value (int).
     */
    void setRange(int min,int max);
    void setSuffix(const QString &suffix);
    void setPrefix(const QString &prefix);
    void setValue(int value);
    void setEnabled(bool state);

    int value() const;

private:
    QGridLayout *_layout;
    QSlider *_slider;
    QSpinBox *_spinbox;

signals:
    void valueChanged(int value);

public slots:

};

#endif // SLIDERSPIN_H
