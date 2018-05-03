#include <QApplication>
#include <QPushButton>
#include "soundboardMainUI.h"
#include <QFile>
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

//    Le conteneur
    SoundboardMainUI container;
  //  container.setFixedSize(400,600);
    container.setFixedWidth(400);
    container.setMinimumHeight(600);
    container.setWindowTitle("Lidl soundboard v0.1");
    container.show();
    //QPushButton bouton("Salut les Zéros, la forme ?",&container);
    //bouton.show();
    container.setWindowIcon(QIcon(":/icon/resources/forsenAim.png"));

    QFile css_dark(":/css/resources/darkorange.css");
    css_dark.open(QFile::ReadOnly);
    app.setStyleSheet(css_dark.readAll());
    css_dark.close();
    return app.exec();
}
