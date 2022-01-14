#ifndef SSOCKETHANDLER_H
#define SSOCKETHANDLER_H

#include <QObject>
#include <QTcpSocket>
#include <QPointer>
#include <QTimer>
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
    QHash<QByteArray,QByteArray> m_headersPairs;
    QByteArray m_body;

};

class SSocketHandler : public QObject
{
    Q_OBJECT
public:
    explicit SSocketHandler(const qintptr &socketDescriptor, QObject *parent = nullptr);
    virtual ~SSocketHandler();
    void run();
signals:
    void finished();
    void timeout();

private:
    void onReadyRead();
    void handleBuffer();
    void onTimeout();
    void onFinished(); //temp function for testing, use direct calls later
    void onDisconnected();
    void onBytesWritten(qint64 bytes);

private:
    qintptr m_socketDescriptor;
    QTcpSocket *m_socket;
    QByteArray m_buffer;
    QTimer m_closeTimer;
    SHttpRequestManifest m_currentRequest;
    qint64 m_bytesToWrite=-1;




};

#endif // SSOCKETHANDLER_H