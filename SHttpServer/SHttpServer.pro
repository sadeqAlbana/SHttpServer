QT -= gui
QT += network concurrent
TEMPLATE = lib
CONFIG += staticlib

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    abstractmiddleware.cpp \
    controller.cpp \
    router.cpp \
    shttprequest.cpp \
    shttpresponse.cpp \
    ssockethandler.cpp \
    stcpserver.cpp \
    testcontroller.cpp

HEADERS += \
    HttpCommon.h \
    abstractmiddleware.h \
    controller.h \
    router.h \
    shttprequest.h \
    shttpresponse.h \
    ssockethandler.h \
    stcpserver.h \
    testcontroller.h

# Default rules for deployment.
unix {
    target.path = $$[QT_INSTALL_PLUGINS]/generic
}
!isEmpty(target.path): INSTALLS += target
