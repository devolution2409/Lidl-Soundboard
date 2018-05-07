#include "Utility.h"

Utility::Utility()
{

}


int Utility::GetKeyAsVirtualKey(std::string key)
{
    // Check out if key is a regular key
    std::map<std::string,unsigned int> hexValue
    {
        // letter keys
        {"A",0x41},
        {"B",0x42},
        {"C",0x43},
        {"D",0x44},
        {"E",0x45},
        {"F",0x46},
        {"G",0x47},
        {"H",0x48},
        {"I",0x49},
        {"J",0x4A},
        {"K",0x4B},
        {"L",0x4C},
        {"M",0x4D},
        {"N",0x4E},
        {"O",0x4F},
        {"P",0x50},
        {"Q",0x51},
        {"R",0x52},
        {"S",0x53},
        {"T",0x54},
        {"U",0x55},
        {"V",0x56},
        {"W",0x57},
        {"X",0x58},
        {"Y",0x59},
        {"Z",0x5A},
        // function keys
        {"F1",0x70},
        {"F2",0x71},
        {"F3",0x72},
        {"F4",0x73},
        {"F5",0x74},
        {"F1",0x70},
        {"F2",0x71},
        {"F3",0x72},
        {"F4",0x73},
        {"F5",0x74},
        {"F6",0x75},
        {"F7",0x76},
        {"F8",0x77},
        {"F9",0x78},
        {"F10",0x79},
        {"F11",0x7A},
        {"F12",0x7B},

        {"Backspace",0x08},
        {"Tab",0x09},
        {"Return",0x0D},
        {"Menu",0x12},
        {"Pause",0x13},
        {"CapsLock",0x14},
        {"Esc",0x1B},
        {"Space",0x20},
        {"PgUp",0x21},
        {"PgDown",0x22},
        {"Home",0x24},
        {"End",0x23},
        {"Home",0x24},
        {"Left",0x25},
        {"Up",0x26},
        {"Right",0x27},
        {"Down",0x28},
        {"Ins",0x2D},
        {"Del",0x2C},
        {"NumLock",0x90},




                                };
    // search for value associated with the key
    std::map<std::string,unsigned int>::iterator it = hexValue.find(key);
    // if we found it we return
    if (it != hexValue.end())
        return it->second;
    else
        return -1;

}
int Utility::GetKeyAsVirtualKey(QString key)
{
    return Utility::GetKeyAsVirtualKey(key.toStdString());


}

int Utility::GetKeyAsScanCode(QString key)
{
    return MapVirtualKey(GetKeyAsVirtualKey(key),MAPVK_VK_TO_VSC);
}
