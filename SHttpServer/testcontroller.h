#ifndef TESTCONTROLLER_H
#define TESTCONTROLLER_H

#include "controller.h"

class TestController : public Controller
{
    Q_OBJECT
public:
    Q_INVOKABLE explicit TestController(QObject *parent = nullptr);
     SHttpResponse test(SHttpRequest *request);
     Q_INVOKABLE virtual ~TestController();

signals:

};

#endif // TESTCONTROLLER_H
