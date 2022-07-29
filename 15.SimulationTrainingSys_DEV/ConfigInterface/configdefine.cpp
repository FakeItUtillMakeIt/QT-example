#include "configdefine.h"
#include "QUuid"
#include <QDebug>
ConfigNameSpaceStart


QString  get_uuid()
{
    QString uuid =  QUuid::createUuid().toString();
    qDebug() << "preuuid:" << uuid;
    uuid.remove('{').remove('}');
    return uuid;
}



ConfigValue::ConfigValue(void *ivalue, ConfigValueType ivaluetype, QString ivaluename, int iuplimit, int idownlimit)
{
    valueid = get_uuid();
    value  = ivalue;
    valuetype = ivaluetype;
    valuename    = ivaluename;
    uplimit = iuplimit;
    downlimit = idownlimit;
}

QString ConfigValue::getStrValue()
{
    QString result;
    switch (valuetype) {
        case eConfigInt:
        {
         result  = QString::number(*(int*)value);
        }
       break;
        case eConfigString:
        {
           result  = QString::fromLocal8Bit((char*)value);
        }
        break;
    case eConfigDataSource:
    case eConfigCommand:
        {
        result  = QString::fromLocal8Bit((char*)value);

        }
        break;
    case eConfigStyleInfo:
        {
        result  = QString::fromLocal8Bit((char*)value);

        }
        break;
    default:
        break;
    }
    return result;

}

void ConfigValue::SetWidthStrValue(QString strvalue)
{
    if(strvalue == "")
        return;
    char tmpSrc[MaximumPathLength];
    char* tmpAim = nullptr;
    int  tmpint,tmplen;
    switch (valuetype) {
        case eConfigInt:
        {
         (*(int*)value)  = strvalue.toInt();
        }
       break;
        case eConfigString:
        {
            tmpAim  = (char*)value;
            memset(tmpAim,0,MaximumPathLength);
            memcpy(tmpAim,strvalue.toLocal8Bit().data(),strlen(strvalue.toLocal8Bit().data()));
        }
        break;
    case eConfigDataSource:
    case eConfigCommand:
        {
            tmpAim  = (char*)value;
            memset(tmpAim,0,MaximumPathLength);
            memcpy(tmpAim,strvalue.toLocal8Bit().data(),strlen(strvalue.toLocal8Bit().data()));

        }
        break;
    case eConfigStyleInfo:
        {
            tmpAim  = (char*)value;
            memset(tmpAim,0,MaximumPathLength);
            memcpy(tmpAim,strvalue.toLocal8Bit().data(),strlen(strvalue.toLocal8Bit().data()));
        }
        break;
    default:
        break;
    }
}


ConfigNameSpaceEnd
