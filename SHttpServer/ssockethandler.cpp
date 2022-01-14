#include "ssockethandler.h"
#include <QDebug>
#include <QHash>
#include <QTcpSocket>
#include <QThread>
#include <QEventLoop>
SSocketHandler::SSocketHandler(const qintptr &socketDescriptor, QObject *parent) : QObject(parent),m_socketDescriptor(socketDescriptor)
  ,m_closeTimer(this)
{
    //qDebug()<<"SSocketHandler::SSocketHandler: " <<QThread::currentThread();
    m_closeTimer.setInterval(5*1000);
    m_closeTimer.setSingleShot(true);

}

SSocketHandler::~SSocketHandler()
{
    //m_socket->deleteLater();
}

void SSocketHandler::run()
{
    m_socket= new QTcpSocket();

    //qDebug()<<Q_FUNC_INFO<< " " <<QThread::currentThread();

    if(!m_socket->setSocketDescriptor(m_socketDescriptor)){
        qWarning()<<"failed to claim socket: " << m_socketDescriptor;
        this->deleteLater(); //delete and ignore the socket if it failed to claim the socketDescriptor !
        return;
    }

    connect(m_socket,&QTcpSocket::readyRead,this,&SSocketHandler::onReadyRead);
    connect(&m_closeTimer,&QTimer::timeout,this,&SSocketHandler::onTimeout);
    connect(this,&SSocketHandler::requestFinished,this,&SSocketHandler::onRequestFinished);
    connect(m_socket,&QTcpSocket::disconnected,this,&SSocketHandler::onDisconnected);
    connect(m_socket,&QTcpSocket::bytesWritten,this,&SSocketHandler::onBytesWritten);

    m_closeTimer.start();
    QEventLoop eventLoop;
    eventLoop.exec();
}

void SSocketHandler::onReadyRead()
{
    m_closeTimer.stop();
    //qDebug()<<Q_FUNC_INFO<< " " <<QThread::currentThread();
    m_buffer.append(m_socket->readAll());
    handleBuffer();
    m_closeTimer.start();
}
void SSocketHandler::onTimeout()
{
    qDebug()<<"timeout";
    disconnect(m_socket,&QTcpSocket::readyRead,this,&SSocketHandler::onReadyRead);
    m_socket->deleteLater();
}
/*
this method is called everytime an http request is fully received
you must handle the request content and write the reply here
*/
void SSocketHandler::onRequestFinished()
{
    m_currentRequest=SHttpRequestManifest();
    m_bytesToWrite=-1;

    //must use the router here !
    QByteArray m_body=QByteArray();
    QString replyTextFormat(
           "HTTP/1.1 %1 OK\r\n"
           "Content-Type: %2\r\n"
           "Content-Length: %3\r\n"
           "Access-Control-Allow-Origin: *\r\n"
           "Access-Control-Allow-Headers: Content-Type,X-Requested-With\r\n"
           "\r\n"
           "%4"
       );
    QString reply=R"({"test":1})";
    replyTextFormat=replyTextFormat.arg(200).arg("application/json;charset=UTF-8").arg(reply.size()).arg(reply);
   m_socket->write(replyTextFormat.toUtf8());
}

void SSocketHandler::onDisconnected()
{
    qInfo()<<QString("socket %1 disconnected").arg(m_socket->socketDescriptor());
    this->deleteLater();
}

void SSocketHandler::onBytesWritten(qint64 bytes)
{
    m_bytesWritten+=bytes;
    if(m_bytesWritten>=m_bytesToWrite){ //>=
        // if(response if fully written, then check the Connection header to determine wether to close the connection or not

        if(m_currentRequest.m_headersPairs.contains("Connection")){
            QString connection=m_currentRequest.m_headersPairs["Connection"];
            if(connection==QStringLiteral("close")){
                m_socket->close(); //close calls disconnectFromHost() internally !
                emit finished();
            }
        }

        m_bytesToWrite=-1;
        m_bytesWritten=0;

        //start accepting another request?
    }
}


void SSocketHandler::handleBuffer()
{
    if(m_currentRequest.m_invalid){
        qInfo()<<"invalid request !";
        emit finished();
        return;
    }

    if(!m_currentRequest.m_passedFirst){
        if(m_buffer.size()<3)
            return;

        int crlfIndex=m_buffer.indexOf("\r\n");
        if(crlfIndex==-1){
            return;
        }
        //first line is complete
        //parse and validate the method, path and protocol version
        QString firstLine=m_buffer.mid(0,crlfIndex);
        QStringList firstLineItems=firstLine.split(' ');
        QString method=firstLineItems[0];
        method=method.trimmed();
        if(method != QStringLiteral("GET")     &&
           method != QStringLiteral("POST")    &&
           method != QStringLiteral("PUT")     &&
           method != QStringLiteral("DELETE")  &&
           method != QStringLiteral("OPTIONS") &&
           method != QStringLiteral("HEAD"))
        {
            m_currentRequest.m_invalid=true;
            this->deleteLater();
            return;
        }else{
           m_currentRequest.m_method=method;
            //qInfo()<<"method: " << m_method;
        }
        QString path=firstLineItems[1];
        if(path.contains(' ')){
            m_currentRequest.m_invalid=true;
            this->deleteLater();
            return;
        }else{
            m_currentRequest.m_path=path;
           // qInfo()<<"path: " << m_path;
        }

        QString httpVersion=firstLineItems[2];
        if(httpVersion!=QStringLiteral("HTTP/1.1")){
            m_currentRequest.m_invalid=true;
            this->deleteLater();
            return;
        }else{
            m_currentRequest.m_httpVersion=httpVersion;
            //qInfo()<<"http version: " << m_httpVersion;
        }
        //remove the first line of buffer now ?
        m_buffer.remove(0,crlfIndex+2);
        //qDebug()<<m_buffer;
        m_currentRequest.m_passedFirst=true;
    }

    if(!m_currentRequest.m_passedHeaders){

        int crlfIndex=m_buffer.indexOf("\r\n\r\n");

        if(crlfIndex==-1){
            return;
        }

        QString headersPart=m_buffer.mid(0,crlfIndex);

        QStringList headersList=headersPart.split("\r\n");
        for(const QString &header : headersList){
            QStringList splitted=header.split(' ');
            m_currentRequest.m_headersPairs.insert(splitted[0].remove(':').toUtf8(),splitted[1].toUtf8());
        }
        //qDebug()<<"headers";
        //qDebug()<<m_headersPairs;
        m_buffer.remove(0,crlfIndex+4);
        //qDebug()<<m_buffer;
        m_currentRequest.m_passedHeaders=true;
    }
    //now that serializing the headers is complete, let's
    if(m_currentRequest.m_headersPairs.contains("Content-Length")){
        int contentLength=m_currentRequest.m_headersPairs["Content-Length"].toInt();
        if(m_buffer.size()>=contentLength){
            m_currentRequest.m_body=m_buffer.mid(0,contentLength);
            m_currentRequest.m_finished=true;
            m_closeTimer.stop();
            emit requestFinished();
        }
    }else{
        m_currentRequest.m_finished=true;
        m_closeTimer.stop();
        emit requestFinished();
    }
    qDebug()<<"Body: "<<m_currentRequest.m_body;
}

