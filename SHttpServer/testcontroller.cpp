#include "testcontroller.h"

TestController::TestController(QObject *parent) : Controller(parent)
{

}

SHttpResponse TestController::test(SHttpRequest *request)
{
    qDebug()<<"invoked !";
    return SHttpResponse(QJsonObject{{"method",Q_FUNC_INFO}});
}
