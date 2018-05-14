QT += widgets \
      multimedia \
      core \
      gui \
      multimedia

SOURCES += \
    main.cpp \
    wrapperproperties.cpp \
    soundwrapper.cpp \
    soundboardMainUI.cpp \
    CustomTableView.cpp \
    CustomShortcutEdit.cpp \
    CustomPlayer.cpp \
    CustomListWidget.cpp


HEADERS += \
    include/wrapperproperties.h \
    include/soundwrapper.h \
    include/soundboardMainUI.h \
    include/CustomTableView.h \
    include/CustomShortcutEdit.h \
    include/CustomPlayer.h \
    include/CustomListWidget.h \
    include/EnumsAndStructs.h


RESOURCES += \
    resources.qrc

VERSION = 1.1.4

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/lib/ -lbass #-lbassmix
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/lib/ -lbass #-lbassmix
else:unix: LIBS += -L$$PWD/lib/ -lbass


INCLUDEPATH += $$PWD/include
DEPENDPATH += $$PWD/.

#Including the auto-updater lib
include($$PWD/lib/QSimpleUpdater-2.0/QSimpleUpdater.pri)
