# SHttpServer

a multithreaded HTTP server for Qt.

## Features
- Multithreaded.
- SSL support.
- Routing capabilities.
- the only dependency is Qt.

## Usage

```cpp
    SHttpServer server;
    qDebug()<<server.setSSlConfiguration(":/server.crt", ":/server.key");
    if(!server.listen(QHostAddress::Any,8080)){
        exit(1);
    }

    server.get("/test",[](SHttpRequest *request){return SHttpResponse(QJsonObject{{"method",Q_FUNC_INFO}});});
```

## Disclaimer
the project is in early development stages, it's API/public interface is subject to change without a prior notice.

## License
GNU Lesser General Public License v3.0. See LICENSE file