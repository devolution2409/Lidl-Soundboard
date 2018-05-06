#ifndef UTILITY_H
#define UTILITY_H
#include <vector>
#include <map>
#include <string>
#include <QString>
class Utility
{
public:
    Utility();
    static int GetKeyAsVK(std::string key);
    static int GetKeyAsVK(QString key);
};

#endif // UTILITY_H
