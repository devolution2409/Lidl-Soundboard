#ifndef OVERLAYWIDGET_H
#define OVERLAYWIDGET_H
#include <QWidget>
#include <QGraphicsEffect>
#include <QPainter>
#include <QVector>
#include <QPoint>
#include <QRadialGradient>
#include <QLinearGradient>
#include <QColor>
//class OverlayWidget : public QWidget
//{
//   void newParent();
//public:
//   explicit OverlayWidget(QWidget * parent = nullptr);

//protected:
//   //! Catches resize and child events from the parent widget
//   bool eventFilter(QObject * obj, QEvent * ev) override;

//   //! Tracks parent widget changes
//   bool event(QEvent* ev) override;

//};



class DarkenEffect : public QGraphicsEffect
{
public:
    void draw( QPainter* painter ) override;
    void appendCircleCenter(QPoint);
    void appendRect(QRect);
private:
    QVector<QPoint> circleCenters;
    QVector<QRect> rects;

};
#endif // OVERLAYWIDGET_H
