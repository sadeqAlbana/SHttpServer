#include <QCoreApplication>
#include <QHostAddress>
#include <stcpserver.h>
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    STcpServer server;
    if(!server.listen(QHostAddress::Any,8080)){
        exit(1);
    }


    return a.exec();
}
