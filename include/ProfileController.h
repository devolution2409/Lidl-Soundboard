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


/*!
 * \brief The ProfileController class
 * Basically when a profile is loaded, the array of sounds QVector<shared_ptr<SoundWrapper>>, contained by the Profile
 * Will be copied to a temporary array temp.
 * Then, the profile sounds will be cleared.
 * Then, mainUI's QVector<shared_ptr<SoundWrapper>> which contains pointer to the currently load profile will be cleared.
 * Finally, the mainUI ProfileSwitched method will be invoked, passing the temporary array as an argument.
 *
 * It will re-add the sounds to the profile, and to the UI.
 * Temporary array goes out of scope, ref count decreases.
 * Reference count should be like this:
 *  1 for the UI (IF profile is currently active)
 *  1 for the profile
 *  + 1 for which ever profile mirrors it
 *  + temporary ones for editing widgets (like editing profiles etc)
 * To see the ref count while in a "still" mode, just play a sound.
 * It will display ref count in qDebug() output.
 */
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
     * \brief DeleteProfile
     * \param profile*
     * Will delete the profile identified by the pointer.
     */
    void DeleteActiveProfile();



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
