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



      //TODO:
//      add event processing for maximizing (same as the previous two)
//      and also for minimizing (then we search for topmost window)
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

void OverlayController::SetHooks()
{
    qDebug() << "Settings hooks";
    //when a window becomes the foreground window (think popup)
    _hookHandles.append(
        SetWinEventHook(
          EVENT_SYSTEM_FOREGROUND, EVENT_SYSTEM_FOREGROUND,  // Range of events
          nullptr,                                          // Handle to DLL.
          LIDL::Callback::ShowOverlay,                                // The callback.
          0, 0,              // Process and thread IDs of interest (0 = all)
          WINEVENT_OUTOFCONTEXT | WINEVENT_SKIPOWNPROCESS)); // Flags.

    // when moving we don't need to showthe overlay
    _hookHandles.append(
        SetWinEventHook(
                EVENT_SYSTEM_MOVESIZEEND, EVENT_SYSTEM_MOVESIZEEND,  // Range of events
          nullptr,                                          // Handle to DLL.
          LIDL::Callback::ResizeToWindow,                                // The callback.
          0, 0,              // Process and thread IDs of interest (0 = all)
          WINEVENT_OUTOFCONTEXT | WINEVENT_SKIPOWNPROCESS)); // Flags.

    // when maximizing
    _hookHandles.append(
        SetWinEventHook(
                  EVENT_SYSTEM_MINIMIZEEND,   EVENT_SYSTEM_MINIMIZEEND,  // Range of events
          nullptr,                                          // Handle to DLL.
          LIDL::Callback::ShowOverlay,                                // The callback.
          0, 0,              // Process and thread IDs of interest (0 = all)
          WINEVENT_OUTOFCONTEXT | WINEVENT_SKIPOWNPROCESS)); // Flags.


}

void OverlayController::UnSetHooks()
{
    qDebug() << "Unhooking events";
    for (auto i: _hookHandles)
    {
        UnhookWinEvent(i);
    }

}

//stashing those here because it KINDA respect seperation of concerns.. forsenT
namespace Callback{

void CALLBACK ResizeToWindow(HWINEVENTHOOK hook, DWORD event, HWND hwnd,
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


void CALLBACK ShowOverlay(HWINEVENTHOOK hook, DWORD event, HWND hwnd,
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
    LIDL::OverlayController::GetInstance()->ShowGameOverlay(hwnd);

}
// cant move callbacks to a class because KKona C functions, so we use namespace instead
// https://docs.microsoft.com/en-us/windows/desktop/ProcThread/thread-local-storage
// ppHopper

} //end namespace callback

} // end namespace LIDL
