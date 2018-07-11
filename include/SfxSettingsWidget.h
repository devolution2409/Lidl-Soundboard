#ifndef SFXSETTINGSWIDGET_H
#define SFXSETTINGSWIDGET_H

#include <QWidget>
#include <QVector>
#include "SliderSpin.h"
#include <QScrollArea>
#include <QLabel>
#include <QCheckBox>
#include <QDebug>
#include <QStringList>
#include <QComboBox>
#include <map>
#include <QString>
#include "EnumsAndStructs.h"
#include <QLineEdit>
class SfxSettingsWidget : public QScrollArea
{
    Q_OBJECT
public:
    /*!
     * \brief SfxSettingsWidget Default constructor
     *
     * Sets widget and QScrollArea properties to achieve A E S T H E T I C purposes.
     * \param parent
     */
    explicit SfxSettingsWidget(QWidget *parent = nullptr);
    /*!
     * \brief SfxSettingsWidget Overloaded constructor to account for SFX name.
     * \param sfxName The sfx name. Will add a label and a checkbox for enabling/disabling purposes.
     * \param parent eShrug.
     */
    SfxSettingsWidget(QString sfxName, LIDL::SFX_TYPE type = LIDL::SFX_TYPE::NONE, QWidget *parent = nullptr);
    /*!
     * \brief addSlider
     * \param label The label to be shown alongside the slider+spinbox.
     * \param min The minimum value.
     * \param max The maximum value.
     * \param suffix The suffix of the spinbox.
     * \param prefix The prefix of the spinbox.
     */
    void addSlider(QString label="",int min = 0, int max=100, QString suffix = "",int specialValue = -1,QString prefix ="" );

    /*!
     * \brief addComboBox Add a combo box to the widget.
     * \param label The label shown along the combobox.
     * \param values String List of possible values.
     */
    void addComboBox(QString label, QStringList values, int enumValue = -1);

    /*!
     * \brief setSliderValue Set a slider's value.
     * \param sliderIndex Which slider to update.
     * \param newValue The slider new value.
     */
    void setSliderValue(int sliderIndex, int newValue);

    /*!
     * \brief setComboBoxIndex Set a combobox's displayed item.
     * \param comboIndex Which combobox to update.
     * \param newItemIndex The index of the new item to be displayed.
     */
    void setComboBoxIndex(int comboIndex, int newItemIndex);

    /*!
     * \brief setValueOfEnumParam Will set the value of a widget item settings.
     *
     * Will look through the std::map associating the int corresponding to the enum parameter,
     * and a pointer to the widget that deals with it.
     * \param enumValue The int representing the enum value (EnumsAndStructs.h).
     * \param newValue The new value.
     */
    void setValueOfEnumParam(int enumValue, int newValue);

    /*!
     * \brief setCheckboxState Set this widget checkbox checked state.
     * \param state The new state of the check box (checked is true, unchecked is false).
     */
    void setCheckboxState(bool state);

    /*!
     * \brief deactivateAll Deactivates all widgets except the checkbox.
     */
    void deactivateAll();

    /*!
     * \brief addSpacer Add a spacer to the layout
     */
    void addSpacer();

    /*!
     * \brief sizeHint Reimplemented to account for the QScrollArea
     * \return The size hint.
     */
    QSize sizeHint() const;

//    void beautify();

private:
    QWidget *_container; /*!< The container widget.*/
    QCheckBox *_checkbox; /*!< Enable/disable Checkbox*/
    QGridLayout* _layout; /*!< The layout. */
    QVector<SliderSpin *> _sliders; /*!< Array of Sliders + Spinboxes */
    QVector<QComboBox *> _comboBox; /*!< Array of comboBox */
    std::map<int, QWidget* > _specialMap; /*!<Map linking which widgets deals with witch settings (enum)*/
    QComboBox *_presetBox;




signals:
    /*!
     * \brief sliderValueChanged Emitted when a slider's value changed.
     * \param sliderIndex The index of the slider in the _sliders array.
     * \param newValue The new value.
     * \param specialValue The index of the settings in the enumeration (EnumsAndStruct.h).
     */
    void sliderValueChanged(int sliderIndex, int newValue, int specialValue);
    /*!
     * \brief comboBoxValueChanged Emitted when a combo box text has changed.
     * \param comboBoxIndex The index of the combobox in the _comboBox array.
     * \param newIndex The new index of the displayed text.
     * \param enumValue The index of the settings in the enumeration (EnumsAndStruct.h).
     */
    void comboBoxValueChanged(int comboBoxIndex, int newIndex, int enumValue);


    /*!
     * \brief checkBoxStateChanged Emitted when checkbox is toggled.
     * \param newState The new state of the checkbox.
     */
    void checkBoxStateChanged(bool newState);
public slots:
};

#endif // SFXSETTINGSWIDGET_H
