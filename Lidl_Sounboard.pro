QT += widgets \
      multimedia \
      core \
      gui \
      multimedia \

SOURCES += \
    main.cpp \
    wrapperproperties.cpp \
    soundwrapper.cpp \
    soundboardMainUI.cpp \
    CustomTableView.cpp \
    CustomShortcutEdit.cpp \
    CustomPlayer.cpp \
    CustomListWidget.cpp \
    CustomListWidgetItem.cpp \
    FancySlider.cpp \
    CustomSoundFile.cpp \
    CustomTableModel.cpp \
    StyledDelegate.cpp \
    SettingsController.cpp


HEADERS += \
    include/wrapperproperties.h \
    include/soundwrapper.h \
    include/soundboardMainUI.h \
    include/CustomTableView.h \
    include/CustomShortcutEdit.h \
    include/CustomPlayer.h \
    include/CustomListWidget.h \
    include/EnumsAndStructs.h \
    include/FancySlider.h     \
    include/CustomListWidgetItem.h \
    include/CustomSoundFile.h \
    include/CustomTableModel.h \
    include/StyledDelegate.h \
    include/SettingsController.h

# do not need to add this to header as this does not contains Q_OBJECT macro
# actually we do or it doesn't add it into project tree forsenT
#include/CustomListWidgetItem.h
TEMPLATE += app

FORMS += \
    Settings.ui

RESOURCES += \
    resources.qrc

VERSION = 1.2.0

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/lib/ -lbass #-lbassmix
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/lib/ -lbass #-lbassmix
else:unix: LIBS += -L$$PWD/lib/ -lbass


INCLUDEPATH += $$PWD/include
DEPENDPATH += $$PWD/.

#Including the auto-updater lib
include($$PWD/lib/QSimpleUpdater-2.0/QSimpleUpdater.pri)