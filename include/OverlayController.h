#ifndef OVERLAYCONTROLLER_H
#define OVERLAYCONTROLLER_H

#include <QMainWindow>
#include <QObject>
#include <QWidget>
#include "windows.h"
#include <QDebug>
#include "qt_windows.h"
#include "winuser.h"
#include <QTimer>
#include <QGridLayout>
#include <QSpacerItem>
#include "GameNameOverlay.h"
#include <QVector>
#include "Profile.h"

namespace LIDL{



/*!
  * \brief The OverlayController singleton.
  *
  * smth here
  * \author Devolution
  * \version 1.8.0
  *
  * \since 1.8.0
  */
class OverlayController : public QWidget
{
    Q_OBJECT


public:
    static OverlayController * GetInstance();
    /*!
     * \brief Resize the overlay to the topmost window. Should be called whenever topmost window changes, or is resized.
     * \param hwnd a handler to a window
     */
    void ResizeToWindow(HWND hwnd);
    /*!
     * \brief Show the smol overlay when topmost window (game) changes.
     * \param hwnd a handler to a window
     * Will get the name of the window from hwnd.
     * \deprecated
     */
    void ShowGameOverlay(HWND hwnd);

    /*!
     * \brief ShowGameOverlay \brief Show the smol overlay when topmost window (game) changes.
     * \param profile
     * Will display profile->name();
     */
    void ShowGameOverlay(Profile * profile);

    /*!
     * \brief SetRadialScanCode
     * \param sc
     */
    void SetRadialScanCode(int sc);
    /*!
     * \brief SetRadialVirtualKey
     * \param vk
     */
    void SetRadialVirtualKey(int vk);

private:
    /*!
     * \brief OverlayController default constructor.
     * \param parent
     */
    explicit OverlayController(QWidget *parent = nullptr);

    static OverlayController * self; /*!< A pointer to the only existing OverlayController.*/
    QTimer * _timerFade; /*!< Timer that controls the fade in/out*/
    GameNameOverlay * _gameOverlay; /*!< Main overlay*/
    QGridLayout * _layout ;/*!< The layout */

    int _radialScanCode;
    int _radialVirtualKey;



signals:

public slots:

};

}
#endif // OVERLAYCONTROLLER_H
