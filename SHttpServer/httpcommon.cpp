/*
 * Copyright (C) 2022 Sadeq Albana
 *
 * Licensed under the GNU Lesser General Public License v3.0 :
 * https://www.gnu.org/licenses/lgpl-3.0.html
 */

#include "httpcommon.h"
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
