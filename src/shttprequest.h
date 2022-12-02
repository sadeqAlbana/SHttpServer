/*
 * Copyright (C) 2022 Sadeq Albana
 *
 * Licensed under the GNU Lesser General Public License v3.0 :
 * https://www.gnu.org/licenses/lgpl-3.0.html
 */

#ifndef SHTTPREQUEST_H
#define SHTTPREQUEST_H

#include <QObject>
#include "httpcommon.h"
#include <QUrl>
#include <QVariant>
#include <QJsonObject>
#include <QJsonDocument>
class SHttpRequest
{

public:
    explicit SHttpRequest();
    ~SHttpRequest();

    QString remoteAddress() ;
    QString operationString();
    Http::Operation operation() const;

    //QJsonObject data(){return _object;}

    //QString urlParameter(QString key);
    //QJsonValue json(QString key);
    //QJsonValue json();

    //QJsonDocument jsonDocument(){return _document;}

    bool isJson();

    QUrl url() const;

    QString urlParameter(QString key);
    QJsonValue json(QString key);
    QJsonValue json();


    QByteArray header(QByteArray key);
    QByteArray mapContentType(const QMetaType::Type type);
    //
    QByteArray toRawData(const QVariant &data);

    HttpHeaderList headers() const;

    Http::Operation m_operation;
    QByteArray m_body;
    QVariant m_mappedData;
    QUrl m_url;
    HttpHeaderList m_headers;
    QString m_remoteAddress;

signals:




};

#endif // SHTTPREQUEST_H
