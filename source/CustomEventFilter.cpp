#include "CustomEventFilter.h"



CustomEventFilter::CustomEventFilter() :  QAbstractNativeEventFilter(), QObject()
{

}



bool CustomEventFilter::nativeEventFilter(const QByteArray &eventType, void *message, long *){

   // qDebug() << "event type:" << eventType << "message:" << message;

    MSG* msg = static_cast<MSG*>(message);
   // TranslateMessage(msg);

    if (msg->message == WM_HOTKEY ){
        qDebug() << "hotkey!";
        emit Keydown(msg->wParam);
    }

    return false; //allow everything to purse actually
}

/*
 *
 *

        QObject::connect(ytdl,&QProcess::started, [=]{
            qDebug() << "STARTED";
        });

//        QObject::connect(ytdl, &QProcess::readyReadStandardOutput, [=]{
//             qDebug() << ytdl->readAllStandardOutput();
//        });
//        QObject::connect(ytdl, &QProcess::readyReadStandardError, [=]{
//              qDebug() << ytdl->readAllStandardError();
//        });
        QObject::connect(ytdl, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), [=]{
            qDebug() << "Youtubedl ended nam";
            ytdl->deleteLater();
        });

        QObject::connect(ytdl,&QProcess::errorOccurred, [=](QProcess::ProcessError error){
            qDebug() << "Error:" << error;
        });

        QObject::connect(ytdl, &QProcess::stateChanged, [=](QProcess::ProcessState newState){
            qDebug() << "my state is now:" << newState;
        });
        QTemporaryDir dir;
          if (dir.isValid()) {
              ytdl->setStandardOutputFile(QString(dir.path() + "/youtube-dl_stdout.txt"));
              ytdl->setStandardErrorFile(QString( dir.path() + "/youtube-dl_stderr.txt"));
          }




                // TODO: prevent retards from entering blank text
                ytdl->setWorkingDirectory(QApplication::applicationDirPath());
               // ytdl->setProcessChannelMode( QProcess::MergedChannels ); // merge stderr and stdout


                ytdl->start("D:\\youtube-dl.exe");

                /, QStringList() <<) ;
                                 << "-x"
                                  << "--audio-format" << "wav"
                                  << "https://youtu.be/z3CA_HTvULc");*/

//    QWidget ntm;
//    QProcess test;
//    test.setWorkingDirectory("D:\\Qt\\Projects\\Lidl-Soundboard\\dist\\release");
//    test.setProcessChannelMode( QProcess::MergedChannels );

//    QObject::connect(&test, &QProcess::readyReadStandardOutput, &ntm, [&](){
//      qDebug() << test.readAllStandardOutput();
//    });

//    test.start("D:\\youtube-dl.exe", QStringList()
//               << "-x"
//               << "--audio-format" << "wav"
//               << "https://www.youtube.com/watch?v=8ZtW1ziF9u0");

    //If the process cannot be started, -2 is returned. If the process crashes, -1 is returned. Otherwise, the process' exit code is returned.
    //QProcess::FailedToStart
    //QProcess::UnknownError


//    if (test.state() == QProcess::NotRunning && test.error() == QProcess::FailedToStart){
//        qDebug() << "Please get youtube dl you fat fuck";

//    }else{
//        qDebug() << "Found youtubeDL!";
//        qDebug() << test.state();

//        test.waitForFinished();
//        qDebug() << test.readAllStandardOutput();
//    }
//  test.waitForFinished();*/
