#ifndef SSOCKETHANDLER_H
#define SSOCKETHANDLER_H

#include <QObject>
#include <QTcpSocket>
#include <QPointer>
#include <QTimer>
class SSocketHandler : public QObject
{
    Q_OBJECT
public:
    explicit SSocketHandler(QTcpSocket *socket, QObject *parent = nullptr);

signals:
    void finished();
    void timeout();

private:
    void onReadyRead();
    void handleBuffer();
    void onTimeout();
    void onFinished(); //temp function for testing, use direct calls later
private:
    QPointer<QTcpSocket> m_socket;

    QByteArray m_buffer;
    QTimer m_closeTimer;

    int m_requestIndex;

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

#endif // SSOCKETHANDLER_H
