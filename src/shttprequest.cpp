/*
 * Copyright (C) 2022 Sadeq Albana
 *
 * Licensed under the GNU Lesser General Public License v3.0 :
 * https://www.gnu.org/licenses/lgpl-3.0.html
 */

#include "shttprequest.h"
#include <QJsonArray>
SHttpRequest::SHttpRequest()
{

}

SHttpRequest::~SHttpRequest()
{
    //qDebug()<<Q_FUNC_INFO;
}

QString SHttpRequest::remoteAddress()
{
    return m_remoteAddress;
}

Http::Operation SHttpRequest::operation() const
{
    return m_operation;
}



QString SHttpRequest::operationString()
{
    Http::Operation operation = this->operation();
    if(operation==Http::GetOperation)
        return QStringLiteral("GET");
    if(operation==Http::PostOperation)
        return QStringLiteral("POST");
    if(operation==Http::PutOperation)
        return QStringLiteral("PUT");
    if(operation==Http::HeadOperation)
        return QStringLiteral("HEAD");
    if(operation==Http::DeleteOperation)
        return QStringLiteral("DELETE");

    return QString();
}

QUrl SHttpRequest::url() const
{
    return m_url;
}

QString SHttpRequest::urlParameter(QString key)
{
    return QString();
}

QJsonValue SHttpRequest::json(QString key)
{
    QJsonDocument doc=QJsonDocument::fromJson(m_body);
    return doc.object()[key];
}

QJsonValue SHttpRequest::json()
{
    QJsonDocument doc=QJsonDocument::fromJson(m_body); //just a temp fix
    if(doc.isObject())
       return doc.object();
    else{
        return doc.array();
    }
    return QJsonValue();
}

QByteArray SHttpRequest::header(QByteArray key)
{
    return m_headers[key];
}

bool SHttpRequest::isJson()
{
    return header("content-type")=="application/json";

}

HttpHeaderList SHttpRequest::headers() const
{
    return m_headers;
}
