#ifndef UTILITY_H
#define UTILITY_H
#include <vector>
#include <map>
#include <string>
#include <QString>
#include "windows.h"
class Utility
{
public:
    Utility();
    static int GetKeyAsVirtualKey(std::string key);
    static int GetKeyAsVirtualKey(QString key);
    static int GetKeyAsScanCode(QString key);
};

#endif // UTILITY_H
