QT += widgets \
      multimedia \
      core \
      gui \
      multimedia \
      network
#      autoupdatergui \

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
    SettingsController.cpp \
    Spoiler.cpp \
#    framelesswindow.cpp \
#    windowdragger.cpp \
#    DarkStyle.cpp
    SliderSpin.cpp \
    SfxSettingsWidget.cpp \
    LoadingWidget.cpp \
    LoadingWidgetWorker.cpp \
    PresetController.cpp \
    PresetWizard.cpp \
    PresetWizardIntroPage.cpp \
    OverlayController.cpp \
    GameNameOverlay.cpp \
    Profile.cpp \
    ProfileEdit.cpp \
    GameSelector.cpp \
    HookController.cpp


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
    include/SettingsController.h \
    include/Spoiler.h \
   # include/bitmask_operators.h \
#    include/framelesswindow.h \
#    include/windowdragger.h \
#    include/DarkStyle.h
    include/SliderSpin.h \
    include/SfxSettingsWidget.h \
    include/LoadingWidget.h \
    include/LoadingWidgetWorker.h \
    include/PresetController.h \
    include/PresetWizard.h \
    include/PresetWizardIntroPage.h \
    include/OverlayController.h \
    include/GameNameOverlay.h \
    include/Profile.h \
    include/ProfileEdit.h \
    include/GameSelector.h \
    include/HookController.h


# Use Precompiled headers (PCH)
PRECOMPILED_HEADER  +=  \
    include/bitmask_operators.h  \


#HEADERS+= \
# include/flags/allow_flags.hpp \
#          include/flags/flags.hpp \
#          include/flags/flagsfwd.hpp  \
#          include/flags/iterator.hpp
# do not need to add this to header as this does not contains Q_OBJECT macro
# actually we do or it doesn't add it into project tree forsenT
#include/CustomListWidgetItem.h
#TEMPLATE += app
FORMS += \
    Settings.ui \
    guideUI.ui \
    loadingJson.ui \
    GameSelector.ui \
    ProfileEdit.ui
   # framelesswindow.ui \e


RESOURCES +=  \
    resources.qrc \
#    framelesswindow.qrc \
#    darkstyle.qrc

#deprecated because we use .rc file
#VERSION = 1.4.0

RC_FILE = lidlsoundboard.rc
CONFIG += c++14

TARGET = Lidl_Soundboard
#trying to staticly link libgcc
QMAKE_CXXFLAGS += -static-libgcc -Wno-error=date-time

#win32:CONFIG(release, debug|release):
# LIBS += -L$$PWD/lib/ -lbass -lbassflac -lole32    #-lbassmix
#else:win32:CONFIG(debug, debug|release):

LIBS += -L$$PWD/lib/ -lbass -lbassflac   -lole32 -loleaut32 -limm32 -lwinmm #-lbassmix

#else:unix:
#LIBS += -L$$PWD/lib/ -lbass

INCLUDEPATH += $$PWD/include
DEPENDPATH += $$PWD/.

#Including the auto-updater lib
#include($$PWD/lib/QSimpleUpdater-2.0/QSimpleUpdater.pri)
