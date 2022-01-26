#include "shttpserver.h"
#include <QTcpSocket>
#include <QDebug>
#include <QThread>
#include <QThreadPool>
#include "ssockethandler.h"
#include <QtConcurrent>
#include <QSslKey>
#include <QSslConfiguration>
SHttpServer::SHttpServer(QObject *parent) : QTcpServer(parent)
{
    QThread::currentThread()->setObjectName("main thread");
}

bool SHttpServer::setSSlConfiguration(const QString &crtFilePath, const QString &keyFilePath, const QList<QPair<QString, QSsl::EncodingFormat> > &caFileList, const QSslSocket::PeerVerifyMode &peerVerifyMode)
{
    QFile fileForCrt( crtFilePath );
    if ( !fileForCrt.open( QIODevice::ReadOnly ) )
    {
        qDebug() << "SslServerManage::listen: error: can not open file:" << crtFilePath;
        return false;
    }

    QFile fileForKey( keyFilePath );
    if ( !fileForKey.open( QIODevice::ReadOnly ) )
    {
        qDebug() << "SslServerManage::listen: error: can not open file:" << keyFilePath;
        return false;
    }

    QSslCertificate sslCertificate( fileForCrt.readAll(), QSsl::Pem );
    QSslKey sslKey( fileForKey.readAll(), QSsl::Rsa, QSsl::Pem, QSsl::PrivateKey );

    QList< QSslCertificate > caCertificates;
    for ( const auto &caFile: caFileList )
    {
        QFile fileForCa( caFile.first );
        if ( !fileForCa.open( QIODevice::ReadOnly ) )
        {
            qDebug() << "SslServerManage::listen: error: can not open file:" << caFile.first;
            return false;
        }

        caCertificates.push_back( QSslCertificate( fileForCa.readAll(), caFile.second ) );
    }

    m_sslConfig=QSslConfiguration();
    m_sslConfig.setPeerVerifyMode( peerVerifyMode );
    m_sslConfig.setPeerVerifyDepth( 1 );
    m_sslConfig.setLocalCertificate( sslCertificate );
    m_sslConfig.setPrivateKey( sslKey );
    m_sslConfig.setProtocol( QSsl::TlsV1_1OrLater );
    m_sslConfig.setCaCertificates( caCertificates );

    return true;
}
/*Note: If you want to handle an incoming connection as a new QTcpSocket object in another thread
 *  you have to pass the socketDescriptor to the other thread and create the QTcpSocket object there
 *   and use its setSocketDescriptor() method.*/

void SHttpServer::incomingConnection(qintptr socketDescriptor)
{
    SSocketHandler *handler=new SSocketHandler(socketDescriptor,m_sslConfig);
    QThread *thread = new QThread();
    handler->moveToThread(thread);
    connect(thread,&QThread::started,handler,&SSocketHandler::run,Qt::QueuedConnection);
    connect(handler,&SSocketHandler::finished,thread,&QThread::quit,Qt::QueuedConnection);
    connect(thread,&QThread::finished,thread,&QObject::deleteLater);
    connect(thread,&QThread::finished,handler,&QObject::deleteLater);
    connect(thread,&QThread::finished,[](){qDebug()<<"thread finished";});
    thread->start();
}
