#ifndef TESTCONTROLLER_H
#define TESTCONTROLLER_H

#include "controller.h"

class TestController : public Controller
{
    Q_OBJECT
public:
    explicit TestController(QObject *parent = nullptr);
     SHttpResponse test(SHttpRequest *request);
     virtual ~TestController();

signals:

};

#endif // TESTCONTROLLER_H
