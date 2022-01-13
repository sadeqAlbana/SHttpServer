#include "ssockethandler.h"
#include <QDebug>
#include <QHash>
SSocketHandler::SSocketHandler(QTcpSocket *socket, QObject *parent) : QObject(parent),m_socket(socket)
{
    connect(m_socket,&QTcpSocket::readyRead,this,&SSocketHandler::onReadyRead);
    connect(&m_closeTimer,&QTimer::timeout,this,&SSocketHandler::onTimeout);
    connect(this,&SSocketHandler::finished,this,&SSocketHandler::onFinished);
    m_closeTimer.setInterval(30*1000);
    m_closeTimer.start();
}

void SSocketHandler::onReadyRead()
{
    //qDebug()<<"on ready read";
    m_closeTimer.stop();
    m_buffer.append(m_socket->readAll());
    //qDebug()<<" buffer: " <<m_buffer;
    handleBuffer();
    m_closeTimer.start();
}
void SSocketHandler::onTimeout()
{
    disconnect(m_socket,&QTcpSocket::readyRead,this,&SSocketHandler::onReadyRead);
    m_socket->deleteLater();
}

void SSocketHandler::onFinished()
{
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
   qDebug()<<"written";
}


void SSocketHandler::handleBuffer()
{
    if(m_invalid)
        return;

    if(!m_passedFirst){
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
        if(method != QStringLiteral("GET") &&
                method != QStringLiteral("POST") &&
                method != QStringLiteral("PUT") &&
                method != QStringLiteral("DELETE"))
        {
            m_invalid=true;
            this->deleteLater();
            return;
        }else{
            m_method=method;
            qInfo()<<"method: " << m_method;
        }
        QString path=firstLineItems[1];
        if(path.contains(' ')){
            m_invalid=true;
            this->deleteLater();
            return;
        }else{
            m_path=path;
            qInfo()<<"path: " << m_path;
        }

        QString httpVersion=firstLineItems[2];
        if(httpVersion!=QStringLiteral("HTTP/1.1")){
            m_invalid=true;
            this->deleteLater();
            return;
        }else{
            m_httpVersion=httpVersion;
            qInfo()<<"http version: " << m_httpVersion;
        }
        //remove the first line of buffer now ?
        m_buffer.remove(0,crlfIndex+2);
        qDebug()<<m_buffer;
        m_passedFirst=true;
    }

    if(!m_passedHeaders){

        int crlfIndex=m_buffer.indexOf("\r\n\r\n");

        if(crlfIndex==-1){
            return;
        }

        QString headersPart=m_buffer.mid(0,crlfIndex);

        QStringList headersList=headersPart.split("\r\n");
        for(const QString &header : headersList){
            QStringList splitted=header.split(' ');
            m_headersPairs.insert(splitted[0].remove(':').toUtf8(),splitted[1].toUtf8());
        }
        //qDebug()<<"headers";
        qDebug()<<m_headersPairs;
        m_buffer.remove(0,crlfIndex+4);
        //qDebug()<<m_buffer;
        m_passedHeaders=true;
    }
    //now that serializing the headers is complete, let's
    if(m_headersPairs.contains("Content-Length")){
        int contentLength=m_headersPairs["Content-Length"].toInt();
        if(m_buffer.size()>=contentLength){
            m_body=m_buffer.mid(0,contentLength);
            m_finished=true;
            m_closeTimer.stop();
            emit finished();
        }
    }else{
        m_finished=true;
        m_closeTimer.stop();
        emit finished();
    }
    qDebug()<<"Body: "<<m_body;
}

