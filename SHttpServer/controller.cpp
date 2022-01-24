#include "controller.h"
#include <QDebug>
Controller::Controller(QObject *parent) : QObject(parent)
{
    qDebug()<<Q_FUNC_INFO;
}

Controller::~Controller()
{
    qDebug()<<"controller destroyed !";
}
