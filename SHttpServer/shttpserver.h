#ifndef SHTTPSERVER_H
#define SHTTPSERVER_H

#include <QTcpServer>
#include <QSslConfiguration>
#include "httpcommon.h"
#include "router.h"
//these are functions that are called inside the request thread prior to socket creation !


class SHttpServer : public QTcpServer, public Router
{
    Q_OBJECT
public:
    explicit SHttpServer(QObject *parent = nullptr);
    bool setSSlConfiguration(const QString &crtFilePath,
                             const QString &keyFilePath,
                             const QList< QPair< QString, QSsl::EncodingFormat > > &caFileList = {},
                             const QSslSocket::PeerVerifyMode &peerVerifyMode=QSslSocket::VerifyNone);

    void addRoutine(ServerCallBack routine);

signals:

private:
    void incomingConnection(qintptr socketDescriptor) final;
    QSslConfiguration m_sslConfig;
    ServerCallBackList m_routines;


};

#endif // SHTTPSERVER_H
