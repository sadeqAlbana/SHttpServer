/*
 * Copyright (C) 2022 Sadeq Albana
 *
 * Licensed under the GNU Lesser General Public License v3.0 :
 * https://www.gnu.org/licenses/lgpl-3.0.html
 */

#include "httpcommon.h"
#include <QJsonObject>
Http::Operation Http::methodtoEnum(const QString &op)
{
        if(op=="GET")
            return GetOperation;
        if(op=="POST")
            return PostOperation;
        if(op=="HEAD")
            return HeadOperation;
        if(op=="PUT")
            return PutOperation;
        if(op=="DELETE")
            return DeleteOperation;
        if(op=="OPTIONS")
            return OptionsOperation;

        return GetOperation; //change this later !!!!!!
}

QString missingParam(QStringList requiredParams, QStringList requestParams)
{
    for (const QString &key : requiredParams) {
        if(!requestParams.contains(key))
            return key;
    }
    return QString();
}



QPair<bool,QString> validateParams(const ParamList &requiredParams, const QJsonObject &requestObject)
{
    const QStringList keys=requestObject.keys();
    for(const RequestParam &required : requiredParams){
        if(!keys.contains(required.key)){
            return QPair<bool,QString>(false,QString("parameter '%1' is missing").arg(required.key));
        }else{
            if(requestObject[required.key].type()!=required.type)
                return QPair<bool,QString>(false,QString("parameter '%1' type is Invalid").arg(required.key));
        }
    }

    return QPair<bool,QString>(true,QString());
}
