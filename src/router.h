/*
 * Copyright (C) 2022 Sadeq Albana
 *
 * Licensed under the GNU Lesser General Public License v3.0 :
 * https://www.gnu.org/licenses/lgpl-3.0.html
 */

#ifndef ROUTER_H
#define ROUTER_H

#include <QMap>
#include "functional"
#include "abstractmiddleware.h"
#include "httpcommon.h"
#include "shttprequest.h"
#include "controller.h"
#include "shttpresponse.h"

#ifdef Q_CC_MSVC
#pragma pointers_to_members(full_generality,virtual_inheritance)
#endif

//class CNTRLR{}; //MSVC will throw C2440 when using reinterpret_cast with undefined classes
class Response;
template<class T> using MemberCallback = SHttpResponse (T::*)(SHttpRequest *);

using Callback = std::function<SHttpResponse (SHttpRequest *)>;

struct CallbackInfo
{
    QMetaObject *controller;
    MemberCallback<Controller> ptr;
    Callback callback;
    MiddlewareList middlewares;
};

typedef QMap<QString,CallbackInfo> MemberCallbacks;

class Router
{

public:
    Router();
    //Router(const Router &other);
    ~Router();
    SHttpResponse route(SHttpRequest *request);


    void registerRoute(Http::Operation op, const QString& action, Callback callback, MiddlewareList middlewares=MiddlewareList())
    {
        CallbackInfo callbackInfo={nullptr,nullptr,callback,middlewares};
        registerRoute(op,action,callbackInfo);
    }
    template <class T>
    void registerRoute(Http::Operation op, const QString& action, MemberCallback<T> memberCallback,MiddlewareList middlewares=MiddlewareList())
    {
        QMetaObject meta= T::staticMetaObject;
        if(!meta.inherits(&Controller::staticMetaObject)){

        }
        CallbackInfo callbackInfo={new QMetaObject(meta),reinterpret_cast<MemberCallback<Controller>>(memberCallback),nullptr,middlewares};
        registerRoute(op,action,callbackInfo);
    }

    void registerRoute(const Http::Operation &op, const QString& action, const CallbackInfo &callbackInfo)
    {
        MemberCallbacks *callBacks;
        switch(op)
        {
        case Http::GetOperation:    callBacks=&getRoutes    ; break;
        case Http::PostOperation:   callBacks=&postRoutes   ; break;
        case Http::PutOperation:    callBacks=&putRoutes    ; break;
        case Http::DeleteOperation: callBacks=&DeleteRoutes ; break;
        case Http::HeadOperation:   callBacks=&headRoutes;    break;
        default:                                    return;
        }
        callBacks->insert(action, callbackInfo);
    }

    template <class T>
    void get(const QString& action,  MemberCallback<T> ptr,MiddlewareList middlewares=MiddlewareList())
    {
        registerRoute(Http::GetOperation,action,ptr,middlewares);
    }

    template <class T>
    void post(const QString& action,  MemberCallback<T> ptr,MiddlewareList middlewares=MiddlewareList())
    {
        registerRoute(Http::PostOperation,action,ptr,middlewares);
    }

    template <class T>
    void head(const QString& action,  MemberCallback<T> ptr,MiddlewareList middlewares=MiddlewareList())
    {
        registerRoute(Http::HeadOperation,action,ptr,middlewares);
    }

    template <class T>
    void put(const QString& action,  MemberCallback<T> ptr,MiddlewareList middlewares=MiddlewareList())
    {
        registerRoute(Http::PutOperation,action,ptr,middlewares);
    }

    template <class T>
    void Delete(const QString& action,  MemberCallback<T> ptr,MiddlewareList middlewares=MiddlewareList())
    {
        registerRoute(Http::DeleteOperation ,action,ptr,middlewares);
    }


    void post(const QString& action, Callback cb)
    {
        registerRoute(Http::PostOperation,action,cb);
    }

    void get(const QString& action, Callback cb)
    {
        registerRoute(Http::GetOperation ,action,cb);
    }
    void put(const QString& action, Callback cb)
    {
        registerRoute(Http::PutOperation,action,cb);
    }

    void head(const QString& action, Callback cb)
    {
        registerRoute(Http::HeadOperation ,action,cb);
    }

    void Delete(const QString& action, Callback cb)
    {
        registerRoute(Http::DeleteOperation ,action,cb);
    }




protected:
    MemberCallbacks getRoutes;
    MemberCallbacks postRoutes;
    MemberCallbacks headRoutes;
    MemberCallbacks putRoutes;
    MemberCallbacks DeleteRoutes;
};

typedef Router Route;

#endif // ROUTER_H
