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
QString getXmlGroupName(ControlType ctrtype)
{
    switch (ctrtype)
    {
    case cConfigButton:
        return "Buttons";
    case cConfigPairLabel:
        return "PairLabels";
    case cConfigGroup:
        return "Groups";
    case cConfigCurve:
        return "Curves";
    default:
        return "null";
    }
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
    case eConfigDataSource:
    case eConfigCommand:
    case eConfigString:
    case eConfigStyleInfo:
        {
        result  = QString::fromLocal8Bit((char*)value);

        }
        break;
    case eConfigMultiDataSource:
       {
        result =  ((QList<QString>*)value)->join(',');
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
    QStringList  strlist;
    switch (valuetype) {
        case eConfigInt:
        {
         (*(int*)value)  = strvalue.toInt();
        }
       break;
        case eConfigDataSource:
        case eConfigCommand:
        case eConfigStyleInfo:
        case eConfigString:
        {
            tmpAim  = (char*)value;
            memset(tmpAim,0,MaximumPathLength);
            memcpy(tmpAim,strvalue.toLocal8Bit().data(),strlen(strvalue.toLocal8Bit().data()));

        }
        break;
        case eConfigMultiDataSource:
        {
             strlist =  strvalue.split(",");
             for(auto str: strlist)
                ((QList<QString>*)value)->append(str);
        }
        break;
    
    default:
        break;
    }
}


ConfigNameSpaceEnd
