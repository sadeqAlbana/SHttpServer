#include <QCoreApplication>
#include <shttpserver.h>
#include <QHostAddress>
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    SHttpServer server;
    if(!server.listen(QHostAddress::Any,8080)){
        exit(1);
    }


    return a.exec();
}
