#ifndef SHTTPREQUEST_H
#define SHTTPREQUEST_H

#include <QObject>
#include "HttpCommon.h"
#include <QUrl>
#include <QVariant>
#include <QJsonObject>
#include <QJsonDocument>
class SHttpRequest
{

public:
    explicit SHttpRequest();
    Http::Operation operation() const;

    QString remoteAddress() ;
    QString operationString();
    //QJsonObject data(){return _object;}

    //QString urlParameter(QString key);
    //QJsonValue json(QString key);
    //QJsonValue json();

    //QJsonDocument jsonDocument(){return _document;}
    bool isJson();

    QUrl url() const;
    QByteArray header(QByteArray key);
    QByteArray mapContentType(const QVariant::Type type);
    QByteArray rawData(const QVariant &data);

    HttpHeaderList headers() const;

    Http::Operation m_operation;
    QByteArray m_body;
    QUrl m_url;
    HttpHeaderList m_headers;

signals:




};

#endif // SHTTPREQUEST_H
