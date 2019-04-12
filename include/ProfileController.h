#ifndef PROFILECONTROLLER_H
#define PROFILECONTROLLER_H

#include <QObject>
#include "Profile.h"
#include "EnumsAndStructs.h"
#include <QDebug>
#include <QObject>


/*!
 * \file ProfileController.h
 * \brief File describing the Profile Controller.
 * The class dealing with storing and swapping profile
 *
 * \author Devolution
 * \version 1.0.0
 * \since 1.9.0
 *
 */

namespace LIDL {

namespace Controller {



class ProfileController : public QObject
{
    Q_OBJECT
public:
    static ProfileController* GetInstance();

    /*!
     * \brief GetProfiles
     * \return An array of profile (see Profile.h)
     */
    std::vector<Profile*> GetProfiles() const;

    /*!
     * \brief AddProfile
     * Add a profile to the profiles array.
     * Copy mode is either nothing, copy sounds from another profile, or mirror sounds from another profile
     * \param profile
     */
    void AddProfile(Profile* profile, LIDL::PROFILE_COPY_MODE copyMode = LIDL::PROFILE_COPY_MODE::NO_COPY);

    /*!
     * \brief ReplaceProfiles
     * Empty this
     * \param profiles
     */
    void ReplaceProfiles(std::vector<Profile*> profiles);

    /*!
     * \brief GetForExe
     * \param exe
     * \return A pointer to the profile if it was found, nullptr otherwise.
     */
    Profile * GetProfileForExe(QString exe);


    /*!
     * \brief AutoGameConfigurationChange
     * \param executable
     *
     * Changes the PTT and loaded sound based on the autodetected game
     */
 //   void AutoGameConfigurationChange(QString executable);

    /*!
     * \brief ManualGameConfigurationChanged
     * \param name
     *
     * Changes the same stuff but based on the selected thing by the user in the combobox
     */
    void ManualGameConfigurationChanged(const QString &name);

    /*!
     * \brief GetActiveProfile
     * \return The active profile
     */
    Profile* GetActiveProfile() const;


private:
    ProfileController();
    static ProfileController *self;
    std::vector<Profile*> _profiles; /*!< Array of available profile, not a QVector because it doesnt work kek*/
    Profile * _activeProfile;
signals:

    void ProfileConfigurationChanged();
    void  AddSoundsToMainUI(QVector<std::shared_ptr<SoundWrapper>>);

};

}// end namespace controller
}//end namespace LIDL
#endif // PROFILECONTROLLER_H
