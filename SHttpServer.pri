INCLUDEPATH += $$PWD/SHttpServer

SOURCES += \
    $$PWD/SHttpServer/abstractmiddleware.cpp \
    $$PWD/SHttpServer/controller.cpp \
    $$PWD/SHttpServer/httpcommon.cpp \
    $$PWD/SHttpServer/router.cpp \
    $$PWD/SHttpServer/shttprequest.cpp \
    $$PWD/SHttpServer/shttpresponse.cpp \
    $$PWD/SHttpServer/shttpserver.cpp \
    $$PWD/SHttpServer/ssockethandler.cpp \
    $$PWD/SHttpServer/testcontroller.cpp

HEADERS += \
    $$PWD/SHttpServer/abstractmiddleware.h \
    $$PWD/SHttpServer/controller.h \
    $$PWD/SHttpServer/httpcommon.h \
    $$PWD/SHttpServer/router.h \
    $$PWD/SHttpServer/shttprequest.h \
    $$PWD/SHttpServer/shttpresponse.h \
    $$PWD/SHttpServer/shttpserver.h \
    $$PWD/SHttpServer/ssockethandler.h \
    $$PWD/SHttpServer/testcontroller.h
