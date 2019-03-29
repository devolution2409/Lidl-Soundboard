#include "OverlayController.h"
namespace LIDL {



OverlayController* OverlayController::self = nullptr;

OverlayController::OverlayController(QWidget *parent) : QWidget(parent)
{
    // transparent widget smiley face :)
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    //this->setAttribute(Qt::WA_TranslucentBackground);


}


void OverlayController::ResizeToWindow(HWND hwnd)
{
    qDebug() << "I should be resized!";
    this->show();

    RECT rect;
    GetWindowRect(hwnd, &rect);
    this->setGeometry(rect.left, rect.top, rect.right - rect.left,
                            rect.bottom - rect.top - 100);
}
OverlayController * OverlayController::GetInstance()
{
    if (self == nullptr)
        self = new OverlayController();

    return self;
}

}
