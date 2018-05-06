QT += widgets \
      multimedia \
      core \
      gui \
      testlib \
      multimedia

SOURCES += \
    main.cpp \
    wrapperproperties.cpp \
    soundwrapper.cpp \
    soundboardMainUI.cpp \
    CustomTableView.cpp \
    CustomShortcutEdit.cpp \
    CustomPlayer.cpp \
    Utility.cpp

HEADERS += \
    wrapperproperties.h \
    soundwrapper.h \
    soundboardMainUI.h \
    CustomTableView.h \
    CustomShortcutEdit.h \
    CustomPlayer.h \
    Utility.h

RESOURCES += \
    resources.qrc


win32:CONFIG(release, debug|release): LIBS += -L$$PWD/lib/ -lbass
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/lib/ -lbass
else:unix: LIBS += -L$$PWD/lib/ -lbass

INCLUDEPATH += $$PWD/.
DEPENDPATH += $$PWD/.
