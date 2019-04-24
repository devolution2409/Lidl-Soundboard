

QT += widgets \
      multimedia \
      core \
      gui \
      multimedia \
      network
#      autoupdatergui \


SOURCES += \
       $$files(source/*.cpp) \


HEADERS += \
    $$files(include/*.h) \


# Use Precompiled headers (PCH)
PRECOMPILED_HEADER  +=  \
    pch/bitmask_operators.h  \

FORMS += \
    $$files(forms/*.ui)



RESOURCES +=  \
    resources.qrc \



RC_FILE = lidlsoundboard.rc
CONFIG += c++17

DEFINES += VER_STRING=\\\"1.9.0\\\"

DEFINES += VER=1.9.0

TARGET = Lidl_Soundboard
#trying to staticly link libgcc
QMAKE_CXXFLAGS += -static-libgcc -Wno-error=date-time

#win32:CONFIG(release, debug|release):
# LIBS += -L$$PWD/lib/ -lbass -lbassflac -lole32    #-lbassmix
#else:win32:CONFIG(debug, debug|release):

LIBS += -L$$PWD/lib/ -lbass -lbassflac   -lole32 -loleaut32 -limm32 -lwinmm -lpsapi#-lbassmix

#else:unix:
#LIBS += -L$$PWD/lib/ -lbass

INCLUDEPATH += $$PWD/include \
            $$PWD/pch
DEPENDPATH += $$PWD/.
VPATH +=  $$PWD/include \
            $$PWD/source \
            $$PWD/pch
#Including the auto-updater lib
#include($$PWD/lib/QSimpleUpdater-2.0/QSimpleUpdater.pri)

#Trying to setup nice build/release and build/debug folder :)
BUILD_ROOT = $${PWD}/build
#/$$TARGET-$$TEMPLATE
DIST_ROOT = $${PWD}/dist

CONFIG(debug, debug|release) {
    BUILD_ROOT = $${BUILD_ROOT}/debug
    DIST_ROOT = $${DIST_ROOT}/debug
} else {
    BUILD_ROOT = $${BUILD_ROOT}/release
    DIST_ROOT = $${DIST_ROOT}/release
}

DESTDIR = $${DIST_ROOT}
OBJECTS_DIR = $${BUILD_ROOT}/obj
MOC_DIR = $${BUILD_ROOT}/moc
RCC_DIR = $${BUILD_ROOT}/rcc
UI_DIR = $${BUILD_ROOT}/ui
