#include "shttpserver.h"
#include <QTcpSocket>
#include <QSslSocket>
#include <QDebug>
#include <shttprequest.h>
#include "ssockethandler.h"
SHttpServer::SHttpServer(QObject *parent) : QObject(parent), m_server(new STcpServer(this))
{
    connect(m_server,&QTcpServer::newConnection,this,&SHttpServer::onNewConnection);
    m_server->setMaxPendingConnections(100);
}

bool SHttpServer::listen(QHostAddress host, qint16 port)
{
    return m_server->listen(host,port);
}

void SHttpServer::onNewConnection()
{
    QTcpSocket *socket=m_server->nextPendingConnection();
    SSocketHandler *handler=new SSocketHandler(socket);

    //socket->write("test");
    //socket->close();
}


