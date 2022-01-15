#include "shttpresponse.h"

SHttpResponse::SHttpResponse(QVariant data, Http::StatusCode statusCode, HttpHeaderList headers) : m_data(data),
    m_statusCode(statusCode),
    m_headers(headers)
{

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
