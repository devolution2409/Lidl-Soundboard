#include "OverlayController.h"

namespace LIDL {



OverlayController* OverlayController::self = nullptr;


OverlayController::OverlayController(QWidget *parent) : QWidget(parent)
{
    // transparent widget smiley face :)

    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::SplashScreen);

    this->setAttribute(Qt::WA_TranslucentBackground);

    this->setWindowTitle("LIDL Overlay");

   // _layout = new QGridLayout(this);


    _gameOverlay = new GameNameOverlay(this);

    _timerFade = new QTimer(this);
    _timerFade->setInterval(5000);

    _radialVirtualKey = -1;
    _radialScanCode = -1;
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
    //_gameOverlay->setGeometry(rect.right - 200, rect.bottom - 100, rect.right, rect.bottom);


}
OverlayController * OverlayController::GetInstance()
{
    if (self == nullptr)
        self = new OverlayController();

    return self;
}

void OverlayController::ShowGameOverlay(HWND hwnd)
{
    //If the timer is already running, it will be stopped and restarted.
        _timerFade->start(5000);
        connect(_timerFade, &QTimer::timeout, this,  [=]{
            _gameOverlay->FadeOut(1000);
        });


        unsigned int len = static_cast<unsigned int>(GetWindowTextLength(hwnd) + 1);
        std::vector<wchar_t> buf(len);
        GetWindowText(hwnd, &buf[0], static_cast<int>(len));
        std::wstring stxt = &buf[0];

         _gameOverlay->SetText(QString::fromStdWString(stxt));

        _gameOverlay->show();

        _gameOverlay->move(this->x(),this->y());



}

void OverlayController::SetRadialScanCode(int sc)
{
    this->_radialScanCode = sc;
}

void OverlayController::SetRadialVirtualKey(int vk)
{
    this->_radialVirtualKey = vk;
}

} // end namespace
