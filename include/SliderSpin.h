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
    /*!
     * \brief SliderSpin
     * \param parent
     */
    explicit SliderSpin(QWidget *parent = nullptr);

    /*!
     * \brief SliderSpin Construct a slider + spinbox obect.
     *
     * Constructs both of the QSlider and the QSpinBox widget and add them to a QGridLayout.
     * The Slider will take 80% of the horizontal space.
     * The SpinBox will take the remaining 20%.
     * \param min The minimum value (int).
     * \param max The maximum value (int).
     * \param suffix A suffix to append to the QSpinBox.
     * \param prefix A prefix to prepend to the QSpinBox.
     * \param parent The parent. (usually null since this widget will be in a layout).
     */
    SliderSpin(int min = 0, int max = 100, QString suffix = "", QString prefix ="", QWidget *parent = nullptr);

    /*!
     * \brief setRange Sets the range of the slider and spin box.
     * \param min Minimum value (int).
     * \param max Maximum value (int).
     */
    void setRange(int min,int max);

    /*!
     * \brief setSuffix Sets the suffix of the QSpinBox.
     * \param suffix The suffix.
     */
    void setSuffix(const QString &suffix);

    /*!
     * \brief setPrefix Sets the prefix of the QSpinBox.
     * \param prefix The prefix.
     */
    void setPrefix(const QString &prefix);

    /*!
     * \brief setValue Sets the current value of both the QSlider and the QSpinBox.
     * \param value The new value.
     */
    void setValue(int value);
    /*!
     * \brief setEnabled
     * \param state
     */
    void setEnabled(bool state);
    /*!
     * \brief value
     * \return
     */
    int value() const;
    /*!
     * \brief specialValue
     * \return
     */
    int specialValue() const;
private:
    QGridLayout *_layout;   /*!< The GridLayout this widget uses.*/
    QSlider *_slider;       /*!< The QSlider this widget uses.*/
    QSpinBox *_spinbox;     /*!< The QSpinBox this widget uses.*/



signals:
    /*!
     * \brief valueChanged Signal emitted when the value is changed.
     * \param value
     */
    void valueChanged(int value);

public slots:

};

#endif // SLIDERSPIN_H
