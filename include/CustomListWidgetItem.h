#ifndef CUSTOMLISTWIDGETITEM_H
#define CUSTOMLISTWIDGETITEM_H


#include <QListWidget>
#include <QListWidgetItem>
#include <QDebug>
#include "EnumsAndStructs.h"
#include "lib/bass.h"
#include <QUrl>

#include <type_traits> // is_same
/*!
 * \file CustomListWidgetItem.h
 * \brief File describing the CustomListWidgetItem class.
 *
 * \author Devolution
 * \version 1.7.0
 */




/*!
 * \brief The CustomListWidgetItem class, inherits QListWidgetItem
 */
class CustomListWidgetItem : public QListWidgetItem
{

public:

    /*!
     * \brief CustomListWidgetItem Overloaded constructor. Called when adding sound to a wrapper.
     *
     * No need for SFX param since it will be blank (new sound).
     * \param text The URI of the file (could be one of the following: file://, http://, or https://).
     * \param mainVolume The main output volume. Default to 100%. But the value we send as a parameter is the SettingsController default value (which can be changed by the user).
     * \param vacVolume The VAC output volume. Default to 100%. But the value we send as a parameter is the SettingsController default value (which can be changed by the user).
     * \param size The size of the file (bytes). Used to determine when to start holding ptt is sound is remote.
     * \param parent The display widget.
     * \param type The type. idk wtf this is, and we don't use it
     */
    CustomListWidgetItem(const QString & text,
                         float mainVolume = 1.0,
                         float vacVolume =1.0 ,
                         unsigned long long size = 0,
                         QListWidget * parent = 0,
                         int type = Type);


    /*!
     * \brief CustomListWidgetItem Overloaded constructor. Called to show existing sounds when editing a wrapper.
     *
     * Basically this constructor will be called whenever the edition of a wrapper is started.
     * The WrapperProperty class constructor will iterate through the wrapper,
     * for each LIDL::SoundFile found the the sound array, it will call this constructor to add the items to the view.
     *
     * \param text The URI of the file (could be one of the following: file://, http://, or https://).
     * \param mainVolume The Main volume of the sound in the existing collection.
     * \param vacVolume The VAC volume of the sound in the existing collection.
     * \param sfx The SFX of the sound in the existing collection.
     * \param size The size of the file (bytes). Used to determine when to start holding ptt is sound is remote.
     * \param parent The display widget.
     * \param type idk kev. Not used anyway.
     */
    CustomListWidgetItem(const QString & text,
                         float mainVolume,
                         float vacVolume,
                         LIDL::SFX sfx,
                         unsigned long long size = 0,
                         QListWidget * parent=0,
                         int type = Type);

    /*!
     * \brief setMainVolume Used to change the main output volume with the sliders once a CustomListWidgetItem has been created.
     */
    void setMainVolume(float);

    /*!
     * \brief setVacVolume Used to change the vac output volume with the sliders once a CustomListWidgetItem has been created.
     */
    void setVacVolume(float);

    /*!
     * \brief getMainVolume Used by WrapperProperties to retrieve the main volume associated to the sound before creating (and sending the wrapper to the main ui).
     * \return
     */
    float getMainVolume();

    /*!
     * \brief getVacVolume Used by WrapperProperties to retrieve the VAC volume associated to the sound before creating (and sending the wrapper to the main ui).
     * \return
     */
    float getVacVolume();



    /*!
     * \brief setSFXDistortion Used to set the parameters of the Distortion.
     *
     * The Sound Entry Editor (WrapperProperties class) uses this method to set the parameters of Distortion SFX.
     * It is used in conjuction with lambda function that are connected and disconnected everytime and item is clicked on.
     * This accounts for any of the parameters through the EnumValue in the contructor of the SfxSettingsWidget
     * IF and only if the values of the enumerations are contiguous.<br>
     * <br>
     * <b>All the necessary casts are done through <i>static_cast</i> in this function.</b><br>
     * Once the casts are done, the values are assigned to this->_sfx.BASS_DX8_DISTORTION variable.
     * \param param The parameter to change (enumeration, see EnumsAndStructs.h).
     * \param value The new value.
     */
    void setSFXDistortion(LIDL::SFX_DIST_PARAM param, int value);

    /*!
     * \brief setSFXChorud Used to set the parameters of the Chorus.
     *
     * The Sound Entry Editor (WrapperProperties class) uses this method to set the parameters of Chorus SFX.
     * It is used in conjuction with lambda function that are connected and disconnected everytime and item is clicked on.
     * This accounts for any of the parameters through the EnumValue in the contructor of the SfxSettingsWidget.
     * IF and only if the values of the enumerations are contiguous.<br>
     * <br>
     * <b>All the necessary casts are done through <i>static_cast</i> in this function.</b><br>
     * Once the casts are done, the values are assigned to this->_sfx.BASS_DX8_CHORUS variable.
     * \param param The parameter to change (enumeration, see EnumsAndStructs.h).
     * \param value The new value.
     */
    void setSFXChorus(LIDL::SFX_CHORUS_PARAM param, int value);

    /*!
     * \brief getSFXChorus Get the value of the specified Chorus SFX parameter.
     * \param which Which paramater to get (enumeration, see EnumsAndStructs.h).
     * \return The value of the parameter.
     */
    int getSFXChorus(LIDL::SFX_CHORUS_PARAM which) const;

    /*!
     * \brief getSFXDistortion Get the value of the specified Distortion SFX parameter.
     * \param which Which paramater to get (enumeration, see EnumsAndStructs.h).
     * \return The value of the parameter.
     */
    int getSFXDistortion(LIDL::SFX_DIST_PARAM which) const;

    /*!
     * \brief SetSFXEnabled Sets (or unsets) the _sfx.flags member.
     * \param type On of the SFX_TYPE enumeration (see EnumsAndStrucs.h)
     * This enumeration can be used as flags. (thanks to bitmask_operator.h).
     * \param enabled If the SFX flag should be added or removed from this->_sfx.flags
     */
    void SetSFXEnabled(LIDL::SFX_TYPE type, bool enabled);

    /*!
     * \brief getSFX
     * \return Returns a LIDL::SFX structure which contains a .flags member to know which SFX is enabled, and several BASS_DX8_<SFX> class.
     */
    LIDL::SFX getSFX();

    /*!
     * \brief getSize
     * \return The size of the sound (in bytes).
     */
    unsigned long long getSize() const;

    /*!
     * \brief setSFXEcho
     * \param param
     * \param value
     */
    void setSFXEcho(LIDL::SFX_ECHO_PARAM param, int value);

    int getSFXEcho(LIDL::SFX_ECHO_PARAM which) const;
    /*!
     * \brief setSFXCompressor
     * \param value
     */
    void setSFXCompressor(LIDL::SFX_COMPRESSOR_PARAM param, int value);
    int getSFXCompressor(LIDL::SFX_COMPRESSOR_PARAM which);

    void setSFXFlanger(LIDL::SFX_FLANGER_PARAM param, int value);
    int  getSFXFlanger(LIDL::SFX_FLANGER_PARAM param);

    void setSFXGargle(LIDL::SFX_GARGLE_PARAM param, int value);
    int getSFXGargle(LIDL::SFX_GARGLE_PARAM param);

    template <class ParamType>
    void setSFX(int specialValue, int value);

    template <class ParamType>
    int getSFXValue(int which);


private:
    /*!
     * \brief CustomListWidgetItem Default constructor, not used
     * \param parent
     */
    explicit CustomListWidgetItem(QListWidget *parent = nullptr);
    float _mainVolume; /*!< Temporary mainVolume variable, used to contruct the LIDL::Soundfile properly.*/
    float _vacVolume; /*!< Temporary vacVolume variable, used to contruct the LIDL::Soundfile properly.*/
    unsigned long long _size; /*!< Temporary size variable, used to contruct the LIDL::Soundfile properly if it is distant sound.*/

    LIDL::SFX _sfx; /*!< Structure containing the flags corresponding to the enabled SFX, and the parameters of those SFX*/


};

template <class ParamType>
int CustomListWidgetItem::getSFXValue(int which)
{
    if (std::is_same<ParamType, LIDL::SFX_DIST_PARAM>::value)
        return this->getSFXDistortion(static_cast<LIDL::SFX_DIST_PARAM>(which));

    if (std::is_same<ParamType, LIDL::SFX_ECHO_PARAM>::value)
        return this->getSFXEcho(static_cast<LIDL::SFX_ECHO_PARAM>(which));

    if (std::is_same<ParamType, LIDL::SFX_CHORUS_PARAM >::value)
        return this->getSFXChorus(static_cast<LIDL::SFX_CHORUS_PARAM>(which));

    if (std::is_same<ParamType, LIDL::SFX_COMPRESSOR_PARAM >::value)
        return this->getSFXCompressor(static_cast<LIDL::SFX_COMPRESSOR_PARAM>(which));

    if (std::is_same<ParamType, LIDL::SFX_FLANGER_PARAM >::value)
        return this->getSFXFlanger(static_cast<LIDL::SFX_FLANGER_PARAM>(which));

    if (std::is_same<ParamType, LIDL::SFX_GARGLE_PARAM >::value)
        return this->getSFXGargle(static_cast<LIDL::SFX_GARGLE_PARAM>(which));

    return -1;
}

template <class ParamType>
void CustomListWidgetItem::setSFX(int specialValue, int value)
{
    // If param is distortion param:
    if (std::is_same<ParamType, LIDL::SFX_DIST_PARAM>::value){
        this->setSFXDistortion(static_cast< LIDL::SFX_DIST_PARAM>(specialValue),value);
        return;
    }

    if (std::is_same<ParamType, LIDL::SFX_ECHO_PARAM>::value){
        this->setSFXEcho(static_cast< LIDL::SFX_ECHO_PARAM>(specialValue),value);
        return;
    }

    if (std::is_same<ParamType, LIDL::SFX_COMPRESSOR_PARAM>::value){
        this->setSFXCompressor(static_cast<LIDL::SFX_COMPRESSOR_PARAM>(specialValue),value);
        return;
    }

    if (std::is_same<ParamType, LIDL::SFX_CHORUS_PARAM >::value){
        this->setSFXChorus(static_cast<LIDL::SFX_CHORUS_PARAM>(specialValue),value);
        return;
    }

    if (std::is_same<ParamType, LIDL::SFX_FLANGER_PARAM >::value){
        this->setSFXFlanger(static_cast<LIDL::SFX_FLANGER_PARAM>(specialValue),value);
        return;
    }

    if (std::is_same<ParamType, LIDL::SFX_GARGLE_PARAM >::value){
        this->setSFXGargle(static_cast<LIDL::SFX_GARGLE_PARAM>(specialValue),value);
        return;
    }


    return;
}

#endif // CUSTOMLISTWIDGETITEM_H
