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



    _gameOverlay = new QWidget(this, Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::SplashScreen);

   // _layout->addItem(new QSpacerItem(100,100, QSizePolicy::Expanding, QSizePolicy::Expanding),0,0 );
 //   _layout->addWidget(_gameOverlay,0,0);

   _gameOverlay->resize(200,100);
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

        _gameOverlay->show();

        _gameOverlay->move(this->x(),this->y());

        QTimer::singleShot(5000, _gameOverlay, &QWidget::close);
}


} // end namespace
