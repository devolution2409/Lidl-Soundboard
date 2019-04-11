#ifndef PROFILE_H
#define PROFILE_H

#include <QString>
#include <QVector>
#include <QSet>

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
        bool IsProfileContainingExe(QString exe) const;

    private:

        Profile(QString name, QSet<QString> exe);

        QString _name;
       // QVector<std::pair<QString,QString>> _gameList; /*!< Array of <executable name, nice name> */
        QSet<QString> _exeList; /*!< Array of executables */

};



class Profile::Builder{
    private:

        QSet<QString> _exeList; /*!< Array of executables */
        QString _name  = "Default";



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


        // produce desired Product
        Profile Build(){
            // Here, optionaly check variable consistency
            // and also if Product is buildable from given information

            return Profile(_name,_exeList);
        }
};

#endif // PROFILE_H
