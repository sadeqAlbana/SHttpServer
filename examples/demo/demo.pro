QT -= gui
QT += network
CONFIG += c++11 console
CONFIG -= app_bundle

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        main.cpp

RESOURCES += \
    $$PWD/key/key.qrc

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../SHttpServer/release/ -lSHttpServer
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../SHttpServer/debug/ -lSHttpServer
else:unix: LIBS += -L$$OUT_PWD/../../SHttpServer/ -lSHttpServer

INCLUDEPATH += $$PWD/../../SHttpServer
DEPENDPATH += $$PWD/../../SHttpServer

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../SHttpServer/release/libSHttpServer.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../SHttpServer/debug/libSHttpServer.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../SHttpServer/release/SHttpServer.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../SHttpServer/debug/SHttpServer.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../../SHttpServer/libSHttpServer.a
