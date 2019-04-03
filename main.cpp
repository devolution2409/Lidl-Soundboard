
#include <QApplication>
#include <QPushButton>
#include "soundboardMainUI.h"
#include <QFile>
#include <QString>
#include <QCoreApplication>
//#include "updater.h"
#include <QDebug>
#include <QStyleFactory>

//#include <winuser.h>
//#include "framelesswindow.h"
//#include "DarkStyle.h"
#include "windows.h"
//#define _WIN32_DCOM
//#include <comdef.h>
//#include <Wbemidl.h>
//#pragma comment(lib, "wbemuuid.lib")
#include <vector>
#include "OverlayController.h"






int main(int argc, char *argv[])
{

    QApplication app(argc, argv);

     qDebug() << QApplication::libraryPaths();

    SoundboardMainUI container;

    container.show();

//r    app.setStyle(new DarkStyle);
    //app.setStyle(QStyleFactory::create("plastique"));
    QFile css_dark(":/css/resources/darkorange.css");
    css_dark.open(QFile::ReadOnly);
    app.setStyleSheet(css_dark.readAll());
    css_dark.close();




    //EVENT_OBJECT_CREATE
    //EVENT_OBJECT_DESTROY
    //EVENT_OBJECT_FOCUS ??
    //EVENT_OBJECT_IME_SHOW
    //EVENT_OBJECT_IME_CHANGE
    //EVENT_OBJECT_LOCATIONCHANGE interesting PogU
    //EVENT_SYSTEM_CAPTURESTART PogU?
    //EVENT_SYSTEM_DESKTOPSWITCH
    //EVENT_SYSTEM_FOREGROUND OMEGAPOGCHAMP?

      MSG msg;
      // Proccess the system events
      QApplication::processEvents();
      // Send a signal to the mainUI with the int parm
      // check if the parm is inside a table
      // if it is, play the associated sound forsenT
      // JUST BUILD A L OMEGALUL OMEGALUL P
      while(GetMessage(&msg,nullptr,0,0))
      {
          TranslateMessage(&msg);
          DispatchMessage(&msg);
          if (msg.message == WM_HOTKEY)
              container.winHotKeyPressed(static_cast<int>(msg.wParam));

      }

    // Re-send post quit message or the app runs as a daemon for some reason forsenT
    PostQuitMessage(0);

    return app.exec();
}

