#ifndef STCPSERVER_H
#define STCPSERVER_H

#include <QTcpServer>

class STcpServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit STcpServer(QObject *parent = nullptr);
    void incomingConnection(qintptr socketDescriptor) final;


signals:

};

#endif // STCPSERVER_H
