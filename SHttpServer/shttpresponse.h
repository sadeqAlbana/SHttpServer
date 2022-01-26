#ifndef SHTTPRESPONSE_H
#define SHTTPRESPONSE_H
#include "HttpCommon.h"
#include <QVariant>
class SHttpResponse
{
public:
    SHttpResponse(QVariant data, Http::StatusCode statusCode=Http::Ok, HttpHeaderList headers=HttpHeaderList());

    Http::StatusCode statusCode() const;
    QVariant data() const;
    HttpHeaderList headers() const;

private:
    QVariant m_data;
    Http::StatusCode m_statusCode;
    HttpHeaderList m_headers;
};

#endif // SHTTPRESPONSE_H
