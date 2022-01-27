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
