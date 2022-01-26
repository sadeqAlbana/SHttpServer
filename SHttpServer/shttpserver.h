#ifndef SHTTPSERVER_H
#define SHTTPSERVER_H

#include <QTcpServer>
#include <QSslConfiguration>

//these are functions that are called inside the request thread prior to socket creation !
using ServerCallBack = std::function<bool ()>;
using ServerCallBackList = QList<ServerCallBack>;

class SHttpServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit SHttpServer(QObject *parent = nullptr);
    bool setSSlConfiguration(const QString &crtFilePath,
                             const QString &keyFilePath,
                             const QList< QPair< QString, QSsl::EncodingFormat > > &caFileList = {},
                             const QSslSocket::PeerVerifyMode &peerVerifyMode=QSslSocket::VerifyNone);

signals:

private:
    void incomingConnection(qintptr socketDescriptor) final;
    QSslConfiguration m_sslConfig;
    ServerCallBackList m_cbList;


};

#endif // SHTTPSERVER_H
