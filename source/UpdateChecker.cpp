#include "UpdateChecker.h"
namespace LIDL {

namespace Controller {


UpdateChecker::UpdateChecker(QObject *parent) : QObject(parent)
{
//    qDebug() << "dont mind just testing shit";
//    QUrl url = QUrl("https://api.github.com/graphql");
    QString path = "cmd";
    QStringList commands;


    commands.clear();
    commands.append("/C");
    commands.append("[Net.ServicePointManager]::SecurityProtocol = 'tls12','tls11','tls'");

    QString token = std::getenv("GITHUB_OAUTH_TOKEN");

//    commands.append(  R"~(curl -H @{"Authorization"=bearer 15f55f1c38f68145c4231007cbd1b52a8622655d"} -Method POST -body '{"query":"{\n  repository(owner: \"devolution2409\", name: \"Lidl-Soundboard\") {\n    releases(last: 2) {\n      edges {\n        node {\n          description\n          isPrerelease\n          tag {\n            id\n            name\n          }\n        }\n      }\n    }\n  }\n}\n"}' https://api.github.com/graphql
//)~"            );




commands.append(
        R"~(curl -H @{"Authorization"=bearer 15f55f1c38f68145c4231007cbd1b52a8622655d"} -Method POST -body '{"query":"{\n  repository(owner: \"devolution2409\", name: \"Lidl-Soundboard\") {\n    releases(last: 2) {\n      edges {\n        node {\n          description\n          isPrerelease\n          tag {\n            id\n            name\n          }\n        }\n      }\n    }\n  }\n}\n"}' https://api.github.com/graphql
                          )~");
    QProcess *p = new QProcess();


    p->setReadChannel(QProcess::StandardOutput);
    connect(p, &QProcess::readyReadStandardOutput, this,
            [=](){
        qDebug() << "????????";
              qDebug() << p->readAllStandardError();
        qDebug() << p->readAllStandardOutput();
    });


    connect(p, &QProcess::readyReadStandardError,this,
            [=](){
        qDebug() << p->readAllStandardError();
    });
    connect(p, &QProcess::started, this , [=](){
           qDebug() << "istartedzulol";
    });


    p->start(path,commands);
    /*
     * curl -H "Authorization: bearer 15f55f1c38f68145c4231007cbd1b52a8622655d" -X POST -d '{"query":"{\n  repository(owner: \"devolution2409\", name: \"Lidl-Soundboard\") {\n    releases(last: 2) {\n      edges {\n        node {\n          description\n          isPrerelease\n          tag {\n            id\n            name\n          }\n        }\n      }\n    }\n  }\n}\n"}' https://api.github.com/graphql
*/

//    QNetworkAccessManager * manager = new QNetworkAccessManager(this);


//  //  QNetworkRequest request(url);


//    QNetworkRequest request(url);
//    request.setRawHeader("Authorization", "bearer 15f55f1c38f68145c4231007cbd1b52a8622655d");
//    request.setRawHeader("Content-Type", "application/json");
//    //QString content = "{\"query\":\"query {  repository(owner: \"devolution2409\", name: \"Lidl-Soundboard\") {    id  }   }\",\"variables\":{}}";

//   // QString content = "{\"query\":\"query {  repository(owner: \"devolution2409\"
//                        //name: \"Lidl-Soundboard\") {    id  }   }\"}";

////    QString content("{\\\"query\\\":\\\"{\\n  repository(owner: \\\"devolution2409\\\", name: \\\"Lidl-Soundboard\\\") {\\n    releases(last: 2) {\\n      edges {\\n        node {\\n          description\\n          isPrerelease\\n          tag {\\n            id\\n            name\\n          }\\n        }\\n      }\\n    }\\n  }\\n}\\n\\\",\\\"variables\\\":{},\\\"operationName\\\":null}" );

////THIS WORKS IN CURL '{"query":"{\n  repository(owner: \"devolution2409\", name: \"Lidl-Soundboard\") {\n    releases(last: 2) {\n      edges {\n        node {\n          description\n          isPrerelease\n          tag {\n            id\n            name\n          }\n        }\n      }\n    }\n  }\n}\n","variables":{},"operationName":null}' https://api.github.com/graphql
////this too
//   // {"query":"{\n  repository(owner: \"devolution2409\", name: \"Lidl-Soundboard\") {\n    releases(last: 2) {\n      edges {\n        node {\n          description\n          isPrerelease\n          tag {\n            id\n            name\n          }\n        }\n      }\n    }\n  }\n}\n"}

////    QString content = QStringLiteral(R"~({"query":"{
////                       repository(owner: \"devolution2409\", name: \"Lidl-Soundboard\") {
////                        releases(last: 2) {
////                            edges {
////                                node {
////                                    description
////                                    isPrerelease
////                                    tag {
////                                    id
////                                    name
////                                    }
////                                }
////                            }
////                        }
////                       }
////                       }
////                                     "}


////)~") ;


//QString content = QStringLiteral( R"~({"query":"{
//                                  repository(login: \"devolution2409\", name: \"Lidl-Soundboard\") {
//                                  releases(last: 2) {
//                                  edges {
//                                  node {
//                                  description
//                                  isPrerelease
//                                  tag {
//                                  id
//                                  name
//                                  }
//                                  }
//                                  }
//                                  }
//                                  }
//                                  }
//                                  ","variables":{},"operationName":null})~"
//            );

//  //  manager->connectToHost(ipAddress, port);
//    manager->post(request,  content.toLocal8Bit()  );
//qDebug().noquote() << content.toLocal8Bit() ;

//    connect(manager, &QNetworkAccessManager::finished,this,[=](QNetworkReply * reply){
//        qDebug() << reply->readAll();
//    });

//    auto LUL = [=](QNetworkRequest request, QByteArray data = QByteArray())
//    {

//      qDebug().noquote() << request.url().toString();
//      const QList<QByteArray>& rawHeaderList(request.rawHeaderList());
//      foreach (QByteArray rawHeader, rawHeaderList) {
//        qDebug().noquote() << request.rawHeader(rawHeader);
//      }
//      qDebug().noquote() << data;

//    };

//    LUL(request, content.toLocal8Bit());

}

}
}
