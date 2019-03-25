
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

int main(int argc, char *argv[])
{
qDebug() << "gneeeee";
    QApplication app(argc, argv);
qDebug() << "gneeeee";
     qDebug() << QApplication::libraryPaths();
   qDebug() << "gneeeee";
    SoundboardMainUI container;
   qDebug() << "gneeeee";
    container.show();

//r    app.setStyle(new DarkStyle);
    //app.setStyle(QStyleFactory::create("plastique"));
    QFile css_dark(":/css/resources/darkorange.css");
    css_dark.open(QFile::ReadOnly);
    app.setStyleSheet(css_dark.readAll());
    css_dark.close();




//    HRESULT hres;

//      // Step 1: --------------------------------------------------
//      // Initialize COM. ------------------------------------------

//      hres =  CoInitializeEx(0, COINIT_MULTITHREADED);
//      if (FAILED(hres))
//      {
//          qDebug() << "Failed to initialize COM library. Error code = 0x"
//               <<  hres;

//      }
//      else
//      {
//        qDebug() << "COM object initialized correctly.";
//      }

      MSG msg;
      // Proccess the system events
      QApplication::processEvents();
      // Send a signal to the mainUI with the int parm
      // check if the parm is inside a table
      // if it is, play the associated sound forsenT
      // JUST BUILD A L OMEGALUL OMEGALUL P
      while(GetMessage(&msg,NULL,0,0))
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
