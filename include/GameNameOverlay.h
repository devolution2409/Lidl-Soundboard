#ifndef GAMENAMEOVERLAY_H
#define GAMENAMEOVERLAY_H

#include <QObject>
#include <QWidget>

class GameNameOverlay : public QWidget
{
    Q_OBJECT
public:
    explicit GameNameOverlay(QWidget *parent = nullptr);

signals:

public slots:
};

#endif // GAMENAMEOVERLAY_H