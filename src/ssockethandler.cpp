/*
 * Copyright (C) 2022 Sadeq Albana
 *
 * Licensed under the GNU Lesser General Public License v3.0 :
 * https://www.gnu.org/licenses/lgpl-3.0.html
 */

#include "ssockethandler.h"
#include <QDebug>
#include <QHash>
#include <QTcpSocket>
#include <QThread>
#include <QEventLoop>
#include "router.h"
#include "shttprequest.h"
#include "shttpresponse.h"
#include <QJsonArray>
#include <QHostAddress>
SSocketHandler::SSocketHandler(const qintptr &socketDescriptor, const Router &router,
                               const ConnectionRoutineCallBackList &cCallbacks,
                               const RequestRoutineCallBackList &rCallbacks,
                               const QSslConfiguration &sslConfig,
                               QObject *parent): QObject(parent),
    m_socketDescriptor(socketDescriptor)
  ,m_closeTimer(this),m_router(new Router(router)),m_sslConfig(sslConfig),
    m_connectionCallbacks(cCallbacks),
    m_requestCallbacks(rCallbacks)
{
    //be warned that the constructor is created on the main thread !
    m_closeTimer.setInterval(10*3000);
    m_closeTimer.setSingleShot(true);
}

SSocketHandler::~SSocketHandler()
{
    //qDebug()<<Q_FUNC_INFO;
    delete m_router;
    m_socket->deleteLater();
}

void SSocketHandler::run()
{
    if(!m_sslConfig.isNull()){
        QSslSocket *sslSocket= new QSslSocket();

        sslSocket->setSslConfiguration(m_sslConfig);
        //sslSocket->ignoreSslErrors();

//        QObject::connect( sslSocket, &QSslSocket::encrypted, [ this, sslSocket ]()
//        {
//            qDebug()<<"encrypted: !";
//        } );

//        QObject::connect( sslSocket, QOverload<const QList<QSslError> &>::of(&QSslSocket::sslErrors), [ this, sslSocket ](const QList<QSslError> &errors)
//        {
//            qDebug()<<"ssl errors !";
//        } );

        m_socket=sslSocket;

    }else{
        m_socket= new QTcpSocket();
    }
    connect(m_socket,&QTcpSocket::readyRead,this,&SSocketHandler::onReadyRead);
    connect(&m_closeTimer,&QTimer::timeout,this,&SSocketHandler::onTimeout);
    connect(this,&SSocketHandler::requestFinished,this,&SSocketHandler::onRequestFinished);
    connect(m_socket,&QTcpSocket::disconnected,this,&SSocketHandler::onDisconnected);
    connect(m_socket,&QTcpSocket::bytesWritten,this,&SSocketHandler::onBytesWritten);


    if(!m_socket->setSocketDescriptor(m_socketDescriptor)){

        qWarning()<<"failed to claim socket: " << m_socketDescriptor;
        //delete and ignore the socket if it failed to claim the socketDescriptor !
        emit finished();
        return;
    }

    if(!m_sslConfig.isNull()){
        QSslSocket *sslSocket = (QSslSocket*)m_socket;
        sslSocket->startServerEncryption();
    }

    //call routines
    bool passedRoutines=true;
    for(const ConnectionRoutineCallBack &routine : m_connectionCallbacks){
        passedRoutines=routine(this);
        if(!passedRoutines){
            //return some sort of error, maybe catch an exception !
            break;
        }
    }

    m_closeTimer.start();
    //no need to call an event loop since the default implementation of QThread::run calls QThread::exec()
//    QEventLoop eventLoop;
//    eventLoop.exec();
}

void SSocketHandler::onReadyRead()
{
    //qDebug()<<"Ready read !";
    m_closeTimer.stop();
    m_buffer.append(m_socket->readAll());
    handleBuffer();
    m_closeTimer.start();
}
void SSocketHandler::onTimeout()
{
    qDebug()<<"timeout";
    m_socket->close();
    //qDebug()<<"wait for   disconnect: "<<m_socket->waitForDisconnected();
    disconnect(m_socket,&QTcpSocket::readyRead,this,&SSocketHandler::onReadyRead);
    emit finished();
    //m_socket->deleteLater();
}

/*
this method is called everytime an http request is fully received
you must handle the request content and write the reply here
*/
void SSocketHandler::onRequestFinished()
{
    //qInfo()<<"Request receiving finished, starting processing !";
    //qInfo()<<"body: " << m_currentRequest.m_body;

    //m_bytesToWrite=-1;

    SHttpRequest request;
    request.m_url=m_currentRequest.m_path;
    request.m_body=m_currentRequest.m_body;
    request.m_headers=m_currentRequest.m_headersPairs;
    request.m_operation=Http::methodtoEnum(m_currentRequest.m_method);
    request.m_remoteAddress=m_socket->peerAddress().toString();


    //call routines
    bool passedRoutines=true;
    for(const RequestRoutineCallBack &routine : m_requestCallbacks){
        passedRoutines=routine(&request);
        if(!passedRoutines){
            //return some sort of error, maybe catch an exception !
            break;
        }
    }


    SHttpResponse res = m_router->route(&request);
    QByteArray m_body=QByteArray();
    QByteArray replyData=toRawData(res.data());
    QString contentType=res.headers().contains("content-type")? res.headers().value("Content-Type") : mapContentType(res.data().type());
    QString replyTextFormat=QString(
           "HTTP/1.1 %1 OK\r\n"
           "Content-Type: %2\r\n"
           "Content-Length: %3\r\n"
           "Connection: keep-alive\r\n"
           "Keep-Alive: timeout=30\r\n"
           "Access-Control-Allow-Origin: *\r\n"
           "Access-Control-Allow-Headers: Content-Type,X-Requested-With\r\n"
           "\r\n"
       ).arg(res.statusCode())
        .arg(contentType)
        .arg(replyData.size());
    QByteArray reply=replyTextFormat.toUtf8();
    reply.append(replyData);

//    m_bytesToWrite=replyTextFormat.toUtf8().size();
   m_socket->write(reply);
   //now we wait for the bytes to be written, new requests will only be accepted when bytes are written
}

void SSocketHandler::onDisconnected()
{
    //qDebug()<<"disconnected called";
    //qDebug()<<QString("socket %1 disconnected").arg(m_socket->socketDescriptor());
    emit finished();
}

void SSocketHandler::onBytesWritten(qint64 bytes)
{

    if(m_socket->bytesToWrite()<=0){ //>=
        // if(response if fully written, then check the Connection header to determine wether to close the connection or not

        if(m_currentRequest.m_headersPairs.contains("Connection")){
            QString connection=m_currentRequest.m_headersPairs["Connection"];
            if(connection==QStringLiteral("close")){
                m_socket->close(); //close calls disconnectFromHost() internally !
                emit finished();

                //db commit will be called from a routine around here !
            }
        }


        m_currentRequest=SHttpRequestManifest();
        //qDebug()<<"m_buffer: " << m_buffer;
        //m_buffer=QByteArray(); //removing this line causes a problem in multiple requests !
        //start accepting another request?
        //qInfo()<<"finished request";
    }
}


void SSocketHandler::handleBuffer()
{
    //qDebug()<<"Handle buffer called !";
    if(m_currentRequest.m_invalid){
        //qDebug()<<"invalid request !";
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
            emit finished();
            //this->deleteLater();
            return;
        }else{
           m_currentRequest.m_method=method;
            //qInfo()<<"method: " << m_method;
        }
        QString path=firstLineItems[1];
        if(path.contains(' ')){
            m_currentRequest.m_invalid=true;
            emit finished();
            //this->deleteLater();
            return;
        }else{
            m_currentRequest.m_path=path;
            //qInfo()<<"path: " << m_currentRequest.m_path;
        }

        QString httpVersion=firstLineItems[2];
        if(httpVersion!=QStringLiteral("HTTP/1.1")){
            m_currentRequest.m_invalid=true;
            emit finished();
            //this->deleteLater();
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
            QStringList splitted=header.split(':');

            m_currentRequest.m_headersPairs.insert(splitted[0].toUtf8(),splitted[1].mid(1).toUtf8());
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
            m_buffer.clear();
            m_currentRequest.m_finished=true;
            m_closeTimer.stop();
            emit requestFinished();
        }
    }else{
        m_currentRequest.m_finished=true;
        m_closeTimer.stop();
        emit requestFinished();
    }
    //qDebug()<<"Body: "<<m_currentRequest.m_body;
}

QByteArray SSocketHandler::mapContentType(const int &type)
{
    //Q_D(Request);

//    if(d->m_response->headers().contains("content-type"))
//        return d_ptr->m_response->headers().value("content-type");

    QByteArray contentType;
    //QMetaType::Type type=static_cast<QMetaType::Type>(type.type());
    switch (type) {
    case QMetaType::QJsonObject  :
    case QMetaType::QJsonValue   :
    case QMetaType::QJsonArray   :
    case QMetaType::QJsonDocument: contentType = "application/json;charset=UTF-8"; break;
    case QMetaType::QImage       : contentType = "image/png";        break;
    case QMetaType::QString      : contentType = "text/plain";       break;


    default                      :                                   break;
    }
    return contentType;
}

QByteArray SSocketHandler::toRawData(const QVariant &data)
{
    QMetaType::Type type=static_cast<QMetaType::Type>(data.type());

    /**************************json**************************/
    if(type==QMetaType::QJsonObject)
    {
        QJsonObject object=data.toJsonObject();
        QJsonDocument document;
        document.setObject(object);
        return document.toJson(QJsonDocument::Compact);
    }

    if(type==QMetaType::QJsonArray)
    {
        QJsonArray array=data.toJsonArray();
        QJsonDocument document;
        document.setArray(array);
        return document.toJson(QJsonDocument::Compact);
    }

    if(type==QMetaType::QJsonDocument)
    {
        return data.toJsonDocument().toJson(QJsonDocument::Compact);
    }

    if(type==QMetaType::QJsonValue)
    {
        QJsonValue jsonValue=data.toJsonValue();

        if(jsonValue.type()==QJsonValue::Array)
        {
            QJsonArray array=jsonValue.toArray();
            QJsonDocument document;
            document.setArray(array);
            return document.toJson(QJsonDocument::Compact);
        }

        if(jsonValue.type()==QJsonValue::Object)
        {
            QJsonObject object=jsonValue.toObject();
            QJsonDocument document;
            document.setObject(object);
            return document.toJson(QJsonDocument::Compact);
        }
        if(jsonValue.type()==QJsonValue::String)
        {
            return jsonValue.toString().toUtf8();
        }
        if(jsonValue==QJsonValue::Double)
        {
            return QString::number(jsonValue.toDouble()).toUtf8();
        }
        if(jsonValue.type()==QJsonValue::Bool)
        {
            return jsonValue.toBool() ? QByteArray("1") : QByteArray("0");
        }

    }

    /**********************end json**************************/

    if(type==QMetaType::QString)
        return data.toString().toUtf8();

    if(type==QMetaType::Int)
        return QString::number(data.toInt()).toUtf8();

    if(type==QMetaType::Double)
        return QString::number(data.toDouble()).toUtf8();

    if(type==QMetaType::Float)
        return QString::number(data.toFloat()).toUtf8();

    if(type==QMetaType::Long || type==QMetaType::LongLong)
        return QString::number(data.toLongLong()).toUtf8();

    if(type==QMetaType::UInt)
        return QString::number(data.toUInt()).toUtf8();

    if(type==QMetaType::ULongLong)
        return QString::number(data.toULongLong()).toUtf8();

    if(type==QMetaType::QByteArray)
        return data.toByteArray();
#ifdef QT_HAVE_GUI
    if(type==QMetaType::QImage)
    {
        QImage image=data.value<QImage>();
        QByteArray imageData;
        QBuffer buffer(&imageData);
        buffer.open(QIODevice::WriteOnly);
        image.save(&buffer,"PNG");
        buffer.close();
        return imageData;
    }
#endif


    qInfo()<<"SSocketHandler::toRawData : unsupported QVariant type";

    return QByteArray();
}

