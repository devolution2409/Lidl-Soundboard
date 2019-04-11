#include "HookController.h"



namespace LIDL {

namespace Controller {

HookController * HookController::self = nullptr;

HookController *HookController::GetInstance()
{
    if (self == nullptr)
        self = new HookController();

    return self;


}

HookController::HookController()
{

}

void HookController::SetHooks()
{
    this->UnSetHooks();
    qDebug() << "Settings hooks";

    // WINEVENT_OUTOFCONTEXT:
    // The callback function is not mapped
    // into the address space of the process that generates the event
    // Because the hook function is called across process boundaries, the system must queue events.
    // Although this method is asynchronous, events are guaranteed to be in sequential order.
    // For more information, see Out-of-Context Hook Functions.

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
          WINEVENT_OUTOFCONTEXT | WINEVENT_SKIPOWNPROCESS )); // Flags.


    // when maximizing
    _hookHandles.append(
        SetWinEventHook(
                  EVENT_SYSTEM_MINIMIZESTART,   EVENT_SYSTEM_MINIMIZESTART,  // Range of events
          nullptr,                                          // Handle to DLL.
          LIDL::Callback::ShowOverlay,                                // The callback.
          0, 0,              // Process and thread IDs of interest (0 = all)
          WINEVENT_OUTOFCONTEXT | WINEVENT_SKIPOWNPROCESS)); // Flags.


}

void HookController::UnSetHooks()
{
    qDebug() << "Unhooking events";
    for (auto i: _hookHandles)
    {
        UnhookWinEvent(i);
    }

}

Profile* HookController::GetProfileForExe(HWND handle)
{
    //typedef unsigned long DWORD;
    // LUL unsigned long* != unsigned int*
    unsigned long processId;
    GetWindowThreadProcessId(handle, &processId);

    // MAX_PATH is defined in  minwindef.h ZULUL
    char filename[ MAX_PATH + 1];

    HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ | PROCESS_TERMINATE, FALSE, processId);

    if(GetModuleFileNameExA(hProcess, nullptr, filename, MAX_PATH) == 0)
    {
        qDebug() << "Error getting file path";
        // function failed
        CloseHandle(hProcess);

    }
    else
    {
        CloseHandle(hProcess);

        // qDebug() << "Program is: " << QString::fromUtf8(filename);
        // Using QFileInfo to transform the "windows style" path: "C:\\Windows\\explorer.exe"
        // To LINEX (forsenT) style path:  "C:/Windows/explorer.exe"
        // because those are the one listed in the profiles :)
        QString path = QFileInfo(QString::fromUtf8(filename)).absoluteFilePath();

        qDebug() << "Absolute path of the executable:" << path;

        // Now we search for it

        return LIDL::Controller::SettingsController::GetInstance()->GetProfileForExe(path);



    }



    return nullptr;

}

} // end name spacecontroller


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
    Q_UNUSED(hwnd);
//    int len = GetWindowTextLength(hwnd) + 1;
//    std::vector<wchar_t> buf(len);
//    GetWindowText(hwnd, &buf[0], len);
//    std::wstring stxt = &buf[0];
//    qDebug() << stxt;



    LIDL::OverlayController::GetInstance()->ResizeToWindow(GetForegroundWindow());

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
    Q_UNUSED(hwnd);
    //actually sending this:
    //HWND GetForegroundWindow()
    // which get the ACTIVE window

    // IF the active window is part of a profile
    Profile* profile = LIDL::Controller::HookController::GetProfileForExe(GetForegroundWindow());
    if (profile != nullptr)
    {

        LIDL::OverlayController::GetInstance()->ResizeToWindow(GetForegroundWindow());
        LIDL::OverlayController::GetInstance()->ShowGameOverlay(profile);

    }

}
// cant move callbacks to a class because KKona C functions, so we use namespace instead
// https://docs.microsoft.com/en-us/windows/desktop/ProcThread/thread-local-storage
// ppHopper

} //end namespace callback


} // end namespace LIDL
