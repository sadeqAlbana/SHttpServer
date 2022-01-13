#ifndef SHTTPSERVER_H
#define SHTTPSERVER_H
#include <QObject>
#include "stcpserver.h"

class SHttpServer : public QObject
{
public:
    SHttpServer(QObject *parent = nullptr);
    bool listen(QHostAddress host=QHostAddress::Any, qint16 port=80);


protected:
    void onNewConnection();

protected:
    STcpServer *m_server;


};

#endif // SHTTPSERVER_H
