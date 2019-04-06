#ifndef PROFILE_H
#define PROFILE_H

#include <QString>
#include <QVector>
#include <QMap>

//TODO is having a game "nice name" really necessary or not?
// in the end, we will just display the profile i think

class Profile
{
public:
    Profile();
    Profile(QString name, QVector<QString> exe);
    QString GetName() const;
    //QVector<std::pair<QString,QString>> GetGameList() const;
    QVector<QString> GetGameList() const;
    void RemoveGame(QString exe);
    void AddGame(QString exe);

private:
    QString _name;
   // QVector<std::pair<QString,QString>> _gameList; /*!< Array of <executable name, nice name> */
    QVector<QString> _exeList; /*!< Array of executables */

};

#endif // PROFILE_H
