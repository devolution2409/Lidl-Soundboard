#ifndef PROFILE_H



#define PROFILE_H

//#include "soundwrapper.h"
#include <QObject>
#include <QString>
#include <QVector>
#include <QSet>
#include <QKeySequence>
#include <memory>
#include <QJsonObject>
#include <QDebug>

/*!
 * \file Profile.h
 * \brief Class describe a profile, which its associated sound, and PTT key
 *
 * \author Devolution
 * \version 1.0
 * \since 1.9.0
 *
 */

//forward declaration instead of including because somehow it fucks everything up
class SoundWrapper;




/*!
 * \brief The Profile class
 * Describe a profile:
 * Name
 * List of linked executable that will make this profile switched to when the executable is detected
 * List of sounds as a QVector<shared_ptr<SoundWrapper>>
 * The owner ship of those element is shared between the profile and the main ui.
 * So reference count should be like this:
 * + 1 for the UI (IF profile is currently active)
 * + 1 for the profile
 * + 1 for which ever profile mirrors it
 * + temporary ones for editing widgets (like editing profiles etc)
 * To see the ref count while in a "still" mode, just play a sound.
 * It will display ref count in qDebug() output.
 */
class Profile
{

    public:
        // use this class to buuld our profiles
        class Builder;

        /*!
         * \brief GetName
         * \return Profile name as a QString
         */
        QString GetName() const;

        /*!
         * \brief GetGameList
         * \return The game list as a QSet<QString>
         */
        QSet<QString> GetGameList() const;
        /*!
         * \brief RemoveGame
         * \param exe
         * Removes a game from the list
         */
        void RemoveGame(QString exe);

        /*!
         * \brief AddGame
         * \param exe
         * Add a game to the list HYPERBRUH
         */
        void AddGame(QString exe);

        /*!
         * \brief GetConfig
         * \return The config as a string, mostly used for debug
         */
        QString GetConfigAsString() const;

        /*!
         * \brief IsExeHere
         * Search for an executable in _exeList.
         * \return true is found, false otherwise
         */
        bool IsContainingExe(QString exe) const;

        /*!
         * \brief GetSounds
         * \return The sounds associated with this profile
         */
        QVector<std::shared_ptr<SoundWrapper>> GetSounds() const;

        /*!
         * \brief AddSound
         * \param The wrapper to append to _sounds
         */
        void AddSound(std::shared_ptr<SoundWrapper> wrapper);


        /*!
         * \brief RemoveSoundFromSharedPtr
         * \param wrapper a shared_ptr
         *
         * Will remove a shared_ptr from the QVector (when the sound was deleted or replaced in the main UI)
         */
        void RemoveSoundFromSharedPtr(std::shared_ptr<SoundWrapper> wrapper);


        /*!
         * \brief SwapSound
         * \param oldWrapper
         * \param newWrapper
         *
         * Will swap oldWrapper by newWrapper using shared_ptr<>::reset()
         * Decreasing ref count of oldWrapper in the proccess
         */
        void SwapSound(std::shared_ptr<SoundWrapper> oldWrapper,std::shared_ptr<SoundWrapper> newWrapper);

        /*!
         * \brief GetParent
         * \return The parent profile that sound should be mirrored from
         */
        Profile * GetParent() const;

        /*!
         * \brief ClearSounds
         * Remove every sound from the array
         */
        void ClearSounds();

        /*!
         * \brief SetPttScanCode
         * \param sc
         * Sets this profile Push To Talk Scan Code
         */
        void SetPttScanCode(int sc);

        /*!
         * \brief SetPttVirtualKey
         * \param vk
         * Set this profile Push To Talk Virtual Key
         */
        void SetPttVirtualKey(int vk);

        /*!
         * \brief GetPttScanCode
         * \return
         * Gets this profile Push To Talk Scan Code
         */
        int GetPttScanCode() const;

        /*!
         * \brief GetPttVirtualKey
         * \return
         * Gets this profile Push To Talk Virtual Key
         */
        int GetPttVirtualKey() const;

        /*!
         * \brief SetPttKeySequence
         * \param sequence
         * Sets this profile Push To Talk QKeySequence
         */
        void SetPttKeySequence(QKeySequence sequence);

        /*!
         * \brief SetPttKeySequence
         * \param sequence
         * Gets this profile Push To Talk QKeySequence
         */
        QKeySequence GetPttKeySequence() const;

        /*!
         * \brief GetProfileAsObject
         * \return A JSON Object representing this profile
         *
         */
        QJsonObject GetProfileAsObject() const;


        ~Profile();

    private:


        Profile(QString name, QSet<QString> exe, Profile* parent,
                QVector<std::shared_ptr<SoundWrapper>> wrappers,
                int PttVirtualKey,
                int PttScanCode,
                QKeySequence PttKeySequence);


        QString _name;
       // QVector<std::pair<QString,QString>> _gameList; /*!< Array of <executable name, nice name> */
        QSet<QString> _exeList; /*!< Array of executables */
        Profile* _parent;




        QVector<std::shared_ptr<SoundWrapper>> _sounds;
        int _PTTVirtualKey = -1;
        int _PTTScanCode = -1;
        QKeySequence _PTTKeySequence;


};



class Profile::Builder{
    private:

        QSet<QString> _exeList; /*!< Array of executables */
        QString _name  = "Default";

        Profile* _parent = nullptr;


        //PepeS ?

        QVector<std::shared_ptr<SoundWrapper>> _wrappers;

        int _pttVirtualKey = -1;
        int _pttScanCode = -1;
        QKeySequence _pttKeySequence = QKeySequence();

    public:
        // create Builder with default values assigned
        // (in C++11 they can be simply assigned above on declaration instead)
        Builder()
        {
            _wrappers.clear();
        }

        // sets custom values for Product creation
        // returns Builder for shorthand inline usage (same way as cout <<)
        Builder& setName( QString name )
        {
            this->_name = name;
            return *this;
        }
        Builder& addExe( QString exe )
        {

            this->_exeList.insert(exe);
            return *this;
        }

        Builder& setParent(Profile * parent)
        {
            this->_parent = parent;
            return *this;
        }

        Builder & setSounds(QVector<std::shared_ptr<SoundWrapper>> wrappers )
        {
            this->_wrappers = wrappers;
            return *this;
        }

        Builder & setPttVirtualKey(int vk)
        {
            this->_pttVirtualKey = vk;
            return *this;
        }
        Builder & setPttScanCode(int sc)
        {
            this->_pttScanCode = sc;
            return *this;
        }
        Builder & setPttKeySequence (QKeySequence ks)
        {
            this->_pttKeySequence = ks;
            return *this;
        }


        // produce desired Product
        Profile* Build(){
            // Here, optionaly check variable consistency
            // and also if Product is buildable from given information
            qDebug() << "[Profile::Builder] Building profile: " << _name
                        << " with " << _wrappers.size() << " wrappers";
            return new Profile(_name,_exeList, _parent,_wrappers, _pttVirtualKey,_pttScanCode,_pttKeySequence);
        }
};

#endif // PROFILE_H
