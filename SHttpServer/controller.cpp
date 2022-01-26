#include "controller.h"
#include <QDebug>
#include <QThread>
Controller::Controller(QObject *parent) : QObject(parent)
{
    //qDebug()<<Q_FUNC_INFO << " " <<  QThread::currentThread();
}

Controller::~Controller()
{
    //qDebug()<<Q_FUNC_INFO << " " <<  QThread::currentThread();
}
