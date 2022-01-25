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
