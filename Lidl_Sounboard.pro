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
    wrapperproperties.h \
    soundwrapper.h \
    soundboardMainUI.h \
    CustomTableView.h \
    CustomShortcutEdit.h \
    CustomPlayer.h \
    CustomListWidget.h


RESOURCES += \
    resources.qrc

VERSION = 1.1.1

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/lib/ -lbass #-lbassmix
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/lib/ -lbass #-lbassmix
else:unix: LIBS += -L$$PWD/lib/ -lbass

INCLUDEPATH += $$PWD/.
DEPENDPATH += $$PWD/.

#Including the auto-updater lib
include($$PWD/lib/QSimpleUpdater-2.0/QSimpleUpdater.pri)
