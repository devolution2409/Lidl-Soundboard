#include <QApplication>
#include <QPushButton>
#include "soundboardMainUI.h"
#include <QFile>



//#include "stdafx.h"
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

//    Le conteneur
    SoundboardMainUI container;
  //  container.setFixedSize(400,600);
    container.setFixedWidth(400);
    container.setMinimumHeight(600);
    container.setWindowTitle("Lidl soundboard v0.2");
    container.show();

    container.setWindowIcon(QIcon(":/icon/resources/forsenAim.png"));

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

        //  if (msg.wParam == 1) qDebug() << "Hot Key activated : ALT + B";
        // if (msg.wParam == 2) qDebug() << "Hot Key activated : ALT + D";
    }

    return app.exec();
}
