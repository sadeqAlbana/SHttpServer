/*
 * Copyright (C) 2022 Sadeq Albana
 *
 * Licensed under the GNU Lesser General Public License v3.0 :
 * https://www.gnu.org/licenses/lgpl-3.0.html
 */

#ifndef SHTTPSERVER_H
#define SHTTPSERVER_H

#include <QTcpServer>
#include <QSslConfiguration>
#include "httpcommon.h"
#include "router.h"
//these are functions that are called inside the request thread prior to socket creation !


class SHttpServer : public QTcpServer, public Router
{
    Q_OBJECT
public:
    explicit SHttpServer(QObject *parent = nullptr);
    ~SHttpServer();
    bool setSSlConfiguration(const QString &crtFilePath,
                             const QString &keyFilePath,
                             const QList< QPair< QString, QSsl::EncodingFormat > > &caFileList = {},
                             const QSslSocket::PeerVerifyMode &peerVerifyMode=QSslSocket::VerifyNone);

    /*parameter callback routine will be called after each new connection*/
    void installConnectionRoutine(ConnectionRoutineCallBack routine);

    /*parameter callback routine will be called after each new http request including the ones using the same connection*/
    void installRequestRoutine(RequestRoutineCallBack routine);



    void installafterRequestRoutine(RequestRoutineCallBack routine); //to be implemented
    void installafterConnectionRoutine(ConnectionRoutineCallBack routine); //to be implemented

signals:

private:
    void incomingConnection(qintptr socketDescriptor) final;
    QSslConfiguration m_sslConfig;
    ConnectionRoutineCallBackList m_connectionCallbacks;
    RequestRoutineCallBackList m_requestCallbacks;

    ConnectionRoutineCallBackList m_afterConnectionCallbacks;
    RequestRoutineCallBackList m_afterRequestCallbacks;


};

#endif // SHTTPSERVER_H
