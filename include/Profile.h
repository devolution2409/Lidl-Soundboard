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


        QString GetName() const;
        //QVector<std::pair<QString,QString>> GetGameList() const;
        QSet<QString> GetGameList() const;
        void RemoveGame(QString exe);
        void AddGame(QString exe);


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
