#ifndef SSOCKETHANDLER_H
#define SSOCKETHANDLER_H

#include <QObject>
#include <QTcpSocket>
#include <QPointer>
#include <QTimer>
#include "httpcommon.h"
#include <QSslSocket>
#include <QSslConfiguration>
class Router;
//https://developer.mozilla.org/en-US/docs/Web/HTTP/Headers/Connection

/*
this handler supports persistant connections, multiple subsequent requests using the same socket is supported
socket won't close unless the timer times out, Connection header is set to close or malformed data received


*/



struct SHttpRequestManifest{
    bool m_finished=false;
    bool m_passedFirst=false;
    bool m_passedHeaders=false;
    bool m_invalid=false;
    QString m_method;
    QString m_httpVersion;
    QString m_path;
    HttpHeaderList m_headersPairs;
    QByteArray m_body;

};

class SSocketHandler : public QObject
{
    Q_OBJECT
public:
    explicit SSocketHandler(const qintptr &socketDescriptor,
                            const Router &router,
                            const ConnectionRoutineCallBackList &cCallbacks=ConnectionRoutineCallBackList(),
                            const RequestRoutineCallBackList &rCallbacks = RequestRoutineCallBackList(),
                            const QSslConfiguration &sslConfig=QSslConfiguration(), QObject *parent = nullptr);
    virtual ~SSocketHandler();
    void run();
signals:
    void requestFinished();
    void timeout();
    void finished();

private:
    void onReadyRead();
    void handleBuffer();
    void onTimeout();
    void onRequestFinished(); //temp function for testing, use direct calls later
    void onDisconnected();
    void onBytesWritten(qint64 bytes);
    QByteArray mapContentType(const int &type);
    QByteArray toRawData(const QVariant &data);




private:
    qintptr m_socketDescriptor;
    QTcpSocket *m_socket;
    QByteArray m_buffer;
    QTimer m_closeTimer;
    SHttpRequestManifest m_currentRequest;
    qint64 m_bytesToWrite=-1;
    qint64 m_bytesWritten=0;
    Router *m_router;
    QSslConfiguration m_sslConfig;
    ConnectionRoutineCallBackList m_connectionCallbacks;
    RequestRoutineCallBackList m_requestCallbacks;
};

#endif // SSOCKETHANDLER_H
