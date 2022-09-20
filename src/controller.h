/*
 * Copyright (C) 2022 Sadeq Albana
 *
 * Licensed under the GNU Lesser General Public License v3.0 :
 * https://www.gnu.org/licenses/lgpl-3.0.html
 */

#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include "shttprequest.h"
#include "shttpresponse.h"
class Controller : public QObject
{
    Q_OBJECT
public:
    Q_INVOKABLE Controller(QObject *parent = nullptr);
    Q_INVOKABLE virtual ~Controller();

};

#endif // CONTROLLER_H
