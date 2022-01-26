#include "shttprequest.h"

SHttpRequest::SHttpRequest()
{

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
    return m_mappedData.toJsonObject()[key];
}

QJsonValue SHttpRequest::json()
{
    return m_mappedData.toJsonValue();
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
