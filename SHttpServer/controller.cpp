/*
 * Copyright (C) 2022 Sadeq Albana
 *
 * Licensed under the GNU Lesser General Public License v3.0 :
 * https://www.gnu.org/licenses/lgpl-3.0.html
 */

#include "controller.h"
#include <QDebug>
#include <QThread>
Controller::Controller(QObject *parent) : QObject(parent)
{
    //qDebug()<<Q_FUNC_INFO << " " <<  QThread::currentThread();
}

Controller::~Controller()
{
    //qDebug()<<Q_FUNC_INFO;
}
