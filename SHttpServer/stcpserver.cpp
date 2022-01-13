#include "stcpserver.h"
#include <QTcpSocket>
#include <QDebug>
STcpServer::STcpServer(QObject *parent) : QTcpServer(parent)
{

}
/*Note: If you want to handle an incoming connection as a new QTcpSocket object in another thread
 *  you have to pass the socketDescriptor to the other thread and create the QTcpSocket object there
 *   and use its setSocketDescriptor() method.*/

void STcpServer::incomingConnection(qintptr socketDescriptor)
{
    qInfo()<<"incoming connection: " << socketDescriptor;
    QTcpSocket *socket=new QTcpSocket;
    if(socket->setSocketDescriptor(socketDescriptor)){
        addPendingConnection(socket);
    }else{
        qWarning()<<"failed to claim socket: " << socketDescriptor;
        delete socket; //delete and ignore the socket if it failed to claim the socketDescriptor !
    }
}
