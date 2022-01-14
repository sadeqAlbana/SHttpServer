#include "stcpserver.h"
#include <QTcpSocket>
#include <QDebug>
#include <QThread>
#include <QThreadPool>
#include "ssockethandler.h"
#include <QtConcurrent>
STcpServer::STcpServer(QObject *parent) : QTcpServer(parent)
{

}
/*Note: If you want to handle an incoming connection as a new QTcpSocket object in another thread
 *  you have to pass the socketDescriptor to the other thread and create the QTcpSocket object there
 *   and use its setSocketDescriptor() method.*/

void STcpServer::incomingConnection(qintptr socketDescriptor)
{
    //qInfo()<<"incoming connection: " << socketDescriptor;

    SSocketHandler *handler=new SSocketHandler(socketDescriptor);
    QThread *thread = new QThread();
    handler->moveToThread(thread);
    connect(thread,&QThread::started,handler,&SSocketHandler::run,Qt::QueuedConnection);
    //connect(handler,&SSocketHandler::finished,thread,&QThread::quit,Qt::QueuedConnection);
    //connect(thread,&QThread::finished,thread,&QThread::deleteLater);
    thread->start();
}
