
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

void CALLBACK TopMostWindowChanged(HWINEVENTHOOK hook, DWORD event, HWND hwnd,
                             LONG idObject, LONG idChild,
                             DWORD dwEventThread, DWORD dwmsEventTime)
{
    //silence those warning forsenE
    Q_UNUSED(hook);
    Q_UNUSED(event);
    Q_UNUSED(idObject);
    Q_UNUSED(idChild);
    Q_UNUSED(dwEventThread);
    Q_UNUSED(dwmsEventTime);
//    int len = GetWindowTextLength(hwnd) + 1;
//    std::vector<wchar_t> buf(len);
//    GetWindowText(hwnd, &buf[0], len);
//    std::wstring stxt = &buf[0];
//    qDebug() << stxt;
    LIDL::OverlayController::GetInstance()->ResizeToWindow(hwnd);

}

void CALLBACK WindowResized(HWINEVENTHOOK hook, DWORD event, HWND hwnd,
                             LONG idObject, LONG idChild,
                             DWORD dwEventThread, DWORD dwmsEventTime)
{
    //silence those warning forsenE
    Q_UNUSED(hook);
    Q_UNUSED(event);
    Q_UNUSED(idObject);
    Q_UNUSED(idChild);
    Q_UNUSED(dwEventThread);
    Q_UNUSED(dwmsEventTime);
//    int len = GetWindowTextLength(hwnd) + 1;
//    std::vector<wchar_t> buf(len);
//    GetWindowText(hwnd, &buf[0], len);
//    std::wstring stxt = &buf[0];
//    qDebug() << stxt;
    LIDL::OverlayController::GetInstance()->ResizeToWindow(hwnd);

}



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

    //EVENT_OBJECT_CREATE
    //EVENT_OBJECT_DESTROY
    //EVENT_OBJECT_FOCUS ??
    //EVENT_OBJECT_IME_SHOW
    //EVENT_OBJECT_IME_CHANGE
    //EVENT_OBJECT_LOCATIONCHANGE interesting PogU
    //EVENT_SYSTEM_CAPTURESTART PogU?
    //EVENT_SYSTEM_DESKTOPSWITCH
    //EVENT_SYSTEM_FOREGROUND OMEGAPOGCHAMP?

    //probably don't need a variable to access those hooks as we will never unhook them?
      //HWINEVENTHOOK g_hook;
      //g_hook =
      SetWinEventHook(
            EVENT_SYSTEM_FOREGROUND, EVENT_SYSTEM_FOREGROUND,  // Range of events
            nullptr,                                          // Handle to DLL.
            TopMostWindowChanged,                                // The callback.
            0, 0,              // Process and thread IDs of interest (0 = all)
            WINEVENT_OUTOFCONTEXT | WINEVENT_SKIPOWNPROCESS); // Flags.

      SetWinEventHook(
                  EVENT_SYSTEM_MOVESIZEEND, EVENT_SYSTEM_MOVESIZEEND,  // Range of events
            nullptr,                                          // Handle to DLL.
            TopMostWindowChanged,                                // The callback.
            0, 0,              // Process and thread IDs of interest (0 = all)
            WINEVENT_OUTOFCONTEXT | WINEVENT_SKIPOWNPROCESS); // Flags.


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

