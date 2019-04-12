#ifndef PROFILE_H



#define PROFILE_H

//#include "soundwrapper.h"
#include <QObject>
#include <QString>
#include <QVector>
#include <QSet>
#include <QKeySequence>
#include <memory>

/*!
 * \file Profile.h
 * \brief Class describe a profile, which its associated sound, and PTT key
 *
 * \author Devolution
 * \version 1.0
 * \since 1.9.0
 *
 */

/*! \class SoundboardMainUI
  * \brief Inherits QMainWindow.
  *
  *  Deals with displaying sounds (main UUI) and intercepting shortcuts.
  *
  */


//forward declaration instead of including because somehow it fucks everything up
class SoundWrapper;

// TODO: add a QVector<SoundWrapper*> array here

//TODO is having a game "nice name" really necessary or not?
// in the end, we will just display the profile i think

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



        ~Profile();

    private:
        QVector<std::shared_ptr<SoundWrapper>> _sounds;

        Profile(QString name, QSet<QString> exe, Profile* parent);


        QString _name;
       // QVector<std::pair<QString,QString>> _gameList; /*!< Array of <executable name, nice name> */
        QSet<QString> _exeList; /*!< Array of executables */
        Profile* _parent;
        int _PTTScanCode = -1;
        int _PTTVirtualKey = -1;
        QKeySequence _PTTKeySequence;



};



class Profile::Builder{
    private:

        QSet<QString> _exeList; /*!< Array of executables */
        QString _name  = "Default";

        Profile* _parent = nullptr;

    public:
        // create Builder with default values assigned
        // (in C++11 they can be simply assigned above on declaration instead)
        Builder()
        {

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


        // produce desired Product
        Profile* Build(){
            // Here, optionaly check variable consistency
            // and also if Product is buildable from given information

            return new Profile(_name,_exeList, _parent);
        }
};

#endif // PROFILE_H
