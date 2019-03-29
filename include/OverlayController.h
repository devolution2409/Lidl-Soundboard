#ifndef OVERLAYCONTROLLER_H
#define OVERLAYCONTROLLER_H

#include <QMainWindow>
#include <QObject>
#include <QWidget>
#include "windows.h"
#include <QDebug>

namespace LIDL{

class OverlayController : public QWidget
{
    Q_OBJECT


public:
    static OverlayController * GetInstance();

private:
    /*!
     * \brief OverlayController default constructor.
     * \param parent
     */
    explicit OverlayController(QWidget *parent = nullptr);

    static OverlayController * self; /*!< A pointer to the only existing OverlayController.*/



signals:

public slots:
    /*!
     * \brief Resize the overlay to the topmost window. Should be called whenever topmost window changes, or is resized.
     * \param hwnd a handler to a window
     */
    void ResizeToWindow(HWND hwnd);
};

}
#endif // OVERLAYCONTROLLER_H
