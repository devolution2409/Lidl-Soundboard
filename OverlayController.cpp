#include "OverlayController.h"

namespace LIDL {



OverlayController* OverlayController::self = nullptr;

OverlayController::OverlayController(QWidget *parent) : QWidget(parent)
{
    // transparent widget smiley face :)

    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::SplashScreen);

//    this->setAttribute(Qt::WA_TranslucentBackground);

    this->setWindowTitle("LIDL Overlay");

}


void OverlayController::ResizeToWindow(HWND hwnd)
{
    qDebug() << "I should be resized!";
    this->show();



    RECT rect;
    GetWindowRect(hwnd, &rect);
    //this->setGeometry(rect.left, rect.top, rect.right - rect.left,rect.bottom - rect.top - 100);
    this->setGeometry(rect.left, rect.top, rect.right - rect.left,rect.bottom - rect.top);
// old cast because somehow static_cast doesnt work
// HWND id = (HWND) this->winId();
//    ShowWindow(id, SW_HIDE);
//    SetWindowLong(id, GWL_EXSTYLE, GetWindowLong(id, GWL_EXSTYLE) | ~WS_EX_APPWINDOW);
//    ShowWindow(id, SW_SHOW);

}
OverlayController * OverlayController::GetInstance()
{
    if (self == nullptr)
        self = new OverlayController();

    return self;
}
}
