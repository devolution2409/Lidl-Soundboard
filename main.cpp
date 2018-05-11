
#include <QApplication>
#include <QPushButton>
#include "soundboardMainUI.h"
#include <QFile>
#include <QString>



int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

//    Le conteneur
    SoundboardMainUI container;
  //  container.setFixedSize(400,600);
    container.setFixedWidth(400);
    container.setMinimumHeight(600);

    container.show();

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
