/*
 * Copyright (C) 2022 Sadeq Albana
 *
 * Licensed under the GNU Lesser General Public License v3.0 :
 * https://www.gnu.org/licenses/lgpl-3.0.html
 */

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
