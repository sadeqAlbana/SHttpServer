#include "router.h"
#include <QUrl>
#include <QDebug>

#include <exception>
#include "shttpresponse.h"
#include "shttprequest.h"
#include "testcontroller.h"
#include <QThread>
Router::Router()
{

}

//Router::Router(const Router &other) : getRoutes(other.getRoutes),postRoutes(other.postRoutes),headRoutes(other.headRoutes),putRoutes(other.putRoutes),DeleteRoutes(other.DeleteRoutes)
//{

//}

Router::~Router()
{

}

SHttpResponse Router::route(SHttpRequest *request)
{
    //qDebug()<<"Url:" <<request->url().toString();
    //qDebug()<<"method: " <<request->methodString();
    if(request->operation()==Http::OptionsOperation)
    {
        return SHttpResponse(QVariant(),Http::NotFound);
        //request->end(ESTATUS_NOT_FOUND);
        //return;
    }

    MemberCallbacks *callBacks;
    Http::Operation op=request->operation();
    switch(op)
    {
    case Http::GetOperation:    callBacks=&getRoutes    ; break;
    case Http::PostOperation:   callBacks=&postRoutes   ; break;
    case Http::PutOperation:    callBacks=&putRoutes    ; break;
    case Http::DeleteOperation: callBacks=&DeleteRoutes ; break;
    case Http::HeadOperation:   callBacks=&headRoutes;    break;
    //default: qDebug()<<"invalid operation"; request->end();           return;
    }

    QString rawUrl=request->url().toString(QUrl::RemoveQuery);
    qDebug()<<rawUrl;

    if(callBacks->contains(rawUrl))
    {
        CallbackInfo cb= callBacks->value(rawUrl);
        if(!cb.middlewares.isEmpty())
        {
            for (AbstractMiddleware *middleware : cb.middlewares) {

                if(!middleware->handle(request))
                {
                    try {
                        return SHttpResponse(QString("Unauthorized access"),Http::Unauthorized);
                    } catch (std::exception &exception) {
                        return SHttpResponse(QJsonObject{{"error","Internal Server Error"},
                                                   {"message",exception.what()}},Http::InternalServerError);
                    }

                }

            }
        }
        try{
            QMetaObject *metaObject= cb.controller;

            //SHttpResponse res =
            if(metaObject){
                //qDebug()<<"Reached here !";
                Controller *controller = (Controller *)metaObject->newInstance(Q_ARG(QObject *,nullptr));
                //TestController *controller = new TestController();
                //qDebug()<<"created instance !";
                SHttpResponse res = (controller->*cb.ptr)(request);
                //request->reply(res.data,res.statusCode);
                //qDebug()<<"and here !";
                controller->deleteLater();
                //delete controller;
                qDebug()<<"controller should be deleted !";

                return res;


            }else{
                SHttpResponse res = cb.callback(request);
                return res;
                //request->reply(res.data,res.statusCode);

            }
            //Response res = metaObject ? (metaObject->newInstance(->*cb.ptr)(request) : cb.callback(request);
            qDebug()<<"reached here !!!!";
        }
        catch(std::exception &exception){

            return SHttpResponse(QJsonObject{{"error","Internal Server Error"},
                                       {"message",exception.what()}},Http::InternalServerError);
        }
    }
    else {
        return SHttpResponse(QString("Invalid path"),Http::NotFound);
    }
}


