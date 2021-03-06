/*
 * Copyright (C) 2022 Sadeq Albana
 *
 * Licensed under the GNU Lesser General Public License v3.0 :
 * https://www.gnu.org/licenses/lgpl-3.0.html
 */

#include <QCoreApplication>
#include <QHostAddress>
#include <shttpserver.h>
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    SHttpServer server;
    qDebug()<<server.setSSlConfiguration(":/server.crt", ":/server.key");
    if(!server.listen(QHostAddress::Any,8080)){
        exit(1);
    }

    server.get("/test",[](SHttpRequest *request){return SHttpResponse(QJsonObject{{"method",Q_FUNC_INFO}});});


    return a.exec();
}
