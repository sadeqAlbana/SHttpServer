#include "testcontroller.h"
#include <QDebug>
#include <QThread>
TestController::TestController(QObject *parent) : Controller(parent)
{
    qDebug()<<Q_FUNC_INFO << " " <<  QThread::currentThread();

}

SHttpResponse TestController::test(SHttpRequest *request)
{
    qDebug()<<"test route invoked !";
    qDebug()<<QThread::currentThread();
    return SHttpResponse(QJsonObject{{"method",Q_FUNC_INFO}});
}

TestController::~TestController()
{
    qDebug()<<Q_FUNC_INFO << " " <<  QThread::currentThread();
}
