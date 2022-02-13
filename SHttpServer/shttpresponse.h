/*
 * Copyright (C) 2022 Sadeq Albana
 *
 * Licensed under the GNU Lesser General Public License v3.0 :
 * https://www.gnu.org/licenses/lgpl-3.0.html
 */

#ifndef SHTTPRESPONSE_H
#define SHTTPRESPONSE_H
#include "httpcommon.h"
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
