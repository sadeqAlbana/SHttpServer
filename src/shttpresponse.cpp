/*
 * Copyright (C) 2022 Sadeq Albana
 *
 * Licensed under the GNU Lesser General Public License v3.0 :
 * https://www.gnu.org/licenses/lgpl-3.0.html
 */

#include "shttpresponse.h"
#include <QDebug>
SHttpResponse::SHttpResponse(QVariant data, Http::StatusCode statusCode, HttpHeaderList headers) : m_data(data),
    m_statusCode(statusCode),
    m_headers(headers)
{

}

SHttpResponse::~SHttpResponse()
{
    //qDebug()<<Q_FUNC_INFO;
}

Http::StatusCode SHttpResponse::statusCode() const
{
    return m_statusCode;
}

QVariant SHttpResponse::data() const
{
    return m_data;
}

HttpHeaderList SHttpResponse::headers() const
{
    return m_headers;
}
