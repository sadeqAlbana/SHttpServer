#ifndef ACL_H
#define ACL_H
class SHttpRequest;
class QString;
class QJsonWebToken;
class QStringList;
class QJsonObject;
#include <QString>
#include <QJsonValue>
#include <QPair>
#include "httpcommon.h"
struct RequestParam{
    QString key;
    QJsonValue::Type type;
};
typedef QList<RequestParam> ParamList;


QString  user(SHttpRequest *request);
bool hasPermission(const QString &permission,SHttpRequest *request);
QString missingParam(QStringList requiredParams,QStringList requestParams);
QPair<bool,QString> validateParams(const ParamList &requiredParams, const QJsonObject &requestObject);



#define require(x) \
     if(!hasPermission(x,request)){ \
     return SHttpResponse(QString("you Don't have permission to view this page"),Http::Unauthorized);}

#define requireParams(x)\
    QString missedParam = missingParam(x,request->json().toObject().keys()); \
    if(!missedParam.isNull())\
    return SHttpResponse(QJsonObject{{"status",-100},{"message",QString("parameter '%1' is missing").arg(missedParam)}},Http::BadRequest);

#define requireParamsNew(...)\
    ParamList __request_param_list{__VA_ARGS__}; \
    QPair<bool,QString> paramsCompResult = validateParams(__request_param_list,request->json().toObject()); \
    if(paramsCompResult.first==false)\
        return SHttpResponse(QJsonObject{{"status",-100},{"message",paramsCompResult.second}},Http::BadRequest);



#endif // ACL_H
