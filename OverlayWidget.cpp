#include "OverlayWidget.h"
//class OverlayWidget : public QWidget
//{
//   void newParent() {
//      if (!parent()) return;
//      parent()->installEventFilter(this);
//      raise();
//   }
//public:
//   OverlayWidget(QWidget * parent = {}) : QWidget{parent} {
//      setAttribute(Qt::WA_NoSystemBackground);
//      setAttribute(Qt::WA_TransparentForMouseEvents);
//      newParent();
//   }
//protected:
//   //! Catches resize and child events from the parent widget
//   bool eventFilter(QObject * obj, QEvent * ev) override {
//      if (obj == parent()) {
//         if (ev->type() == QEvent::Resize)
//            resize(static_cast<QResizeEvent*>(ev)->size());
//         else if (ev->type() == QEvent::ChildAdded)
//            raise();
//      }
//      return QWidget::eventFilter(obj, ev);
//   }
//   //! Tracks parent widget changes
//   bool event(QEvent* ev) override {
//      if (ev->type() == QEvent::ParentAboutToChange) {
//         if (parent()) parent()->removeEventFilter(this);
//      }
//      else if (ev->type() == QEvent::ParentChange)
//         newParent();
//      return QWidget::event(ev);
//   }
//};

void DarkenEffect::appendCircleCenter(QPoint center)
{
    this->circleCenters.append(center);
}

void DarkenEffect::draw( QPainter* painter )
{

    QPixmap pixmap;
    QPoint offset;
    if( sourceIsPixmap() ) // No point in drawing in device coordinates (pixmap will be scaled anyways)
        pixmap = sourcePixmap( Qt::LogicalCoordinates, &offset );
    else // Draw pixmap in device coordinates to avoid pixmap scaling;
    {
        pixmap = sourcePixmap( Qt::DeviceCoordinates, &offset );
        painter->setWorldTransform( QTransform() );
    }


    //QRadialGradient::QRadialGradient(const QPointF & center, qreal radius, const QPointF & focalPoint)
   // QRadialGradient gradient;
  //  painter->setBrush( QColor( 0, 0, 0, 128 ) ); // black bg
   // painter->drawRect( pixmap.rect() );
   // painter->setOpacity( 0.25 );
          //  painter->drawPixmap( offset, pixmap );
    //int count = 0;
    for(auto i: rects)
    {
        /*
        // pen determines the border color
        painter->setPen( QPen(QColor("#000000")));
        // Creating gradient everytime so it is centered on the circle's center
        painter->setOpacity(1);
        QRadialGradient gradient(i,11);
        gradient.setColorAt(0,QColor(252,102,0,255)); // orange
        gradient.setColorAt(1, QColor(249,166,2,255)); // gold

        painter->setBrush(QBrush(gradient));
        painter->drawEllipse(i,10,10);

        // Adding the text

        painter->setFont(QFont("Arial", 8, QFont::Bold));
        painter->setPen( QPen(QColor("#FFFFFF")));

        painter->drawText(i + QPoint(-2,3) ,QString::number(++count));
        */
//        painter->setBrush(QColor(0,0,0,0));
//        painter->setPen(QPen(QColor("#FC6600")));
//        painter->drawRect(i);

    }


}

void DarkenEffect::appendRect(QRect rect)
{
    this->rects.append(rect);
}
