
#include <QApplication>
#include <QPushButton>
#include "soundboardMainUI.h"
#include <QFile>
#include <QString>
#include <QCoreApplication>
#include "updater.h"
#include <QDebug>
#include <QStyleFactory>

//#include <winuser.h>
#include "framelesswindow.h"
#include "DarkStyle.h"
#include "windows.h"
int main(int argc, char *argv[])
{

    QApplication app(argc, argv);

     qDebug() << QApplication::libraryPaths();
//    Main UI LeBaited
     // create frameless window (and set windowState or title)
   // FramelessWindow framelessWindow;
    //framelessWindow.setWindowState(Qt::WindowMaximized);
    //framelessWindow.setWindowTitle("test title");
    //framelessWindow.setWindowIcon(a.style()->standardIcon(QStyle::SP_DesktopIcon));
    SoundboardMainUI container;

    // add the mainwindow to our custom frameless window
//    framelessWindow.setContent(&container);
//    framelessWindow.show();
//    framelessWindow.setWindowTitle( "LIDL Sounboard " + QString(VER_STRING));
//    framelessWindow.setWindowIcon(QIcon(":/icon/resources/forsenAim.png"));

    container.show();

//r    app.setStyle(new DarkStyle);
    //app.setStyle(QStyleFactory::create("plastique"));
    QFile css_dark(":/css/resources/darkorange.css");
    css_dark.open(QFile::ReadOnly);
    app.setStyleSheet(css_dark.readAll());
    css_dark.close();


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
