#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include "shttprequest.h"
#include "shttpresponse.h"
class Controller : public QObject
{
    Q_OBJECT
public:
    Controller(QObject *parent = nullptr);
};

#endif // CONTROLLER_H
