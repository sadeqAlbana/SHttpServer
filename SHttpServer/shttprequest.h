#ifndef SHTTPREQUEST_H
#define SHTTPREQUEST_H

#include <QObject>

class SHttpRequest : public QObject
{
    Q_OBJECT
public:
    explicit SHttpRequest(QObject *parent = nullptr);

signals:

};

#endif // SHTTPREQUEST_H
