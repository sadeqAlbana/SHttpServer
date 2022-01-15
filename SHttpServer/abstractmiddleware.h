#ifndef ABSTRACTMIDDLEWARE_H
#define ABSTRACTMIDDLEWARE_H

#include <QList>
class SHttpRequest;
class AbstractMiddleware
{
public:
    AbstractMiddleware();
    //virtual ~AbstractMiddleware();

    virtual bool handle(SHttpRequest *request)=0;
};

typedef QList<AbstractMiddleware *> MiddlewareList;

#endif // ABSTRACTMIDDLEWARE_H
