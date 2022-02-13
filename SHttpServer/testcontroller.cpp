/*
 * Copyright (C) 2022 Sadeq Albana
 *
 * Licensed under the GNU Lesser General Public License v3.0 :
 * https://www.gnu.org/licenses/lgpl-3.0.html
 */

#include "testcontroller.h"
#include <QDebug>
#include <QThread>
TestController::TestController(QObject *parent) : Controller(parent)
{
    //qDebug()<<Q_FUNC_INFO << " " <<  QThread::currentThread();

}

SHttpResponse TestController::test(SHttpRequest *request)
{
    qDebug()<<"test route invoked !";
    qDebug()<<QThread::currentThread();
    return SHttpResponse(QJsonObject{{"method",Q_FUNC_INFO}});
}

TestController::~TestController()
{
    //qDebug()<<Q_FUNC_INFO << " " <<  QThread::currentThread();
}
