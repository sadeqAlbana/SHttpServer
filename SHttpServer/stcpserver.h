#ifndef STCPSERVER_H
#define STCPSERVER_H

#include <QTcpServer>

class STcpServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit STcpServer(QObject *parent = nullptr);


signals:

private:
    void incomingConnection(qintptr socketDescriptor) final;


};

#endif // STCPSERVER_H
