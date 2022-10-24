#include <QList>
#include <QMap>
#include <QString>
#include <QVector>
#include <QDebug>
#include <QApplication>
#pragma execution_character_set("utf-8")

#ifndef CONFIGDEFINE_H
#define CONFIGDEFINE_H

#define  ConfigNameSpaceStart   namespace ConfigNameSpace {

#define  ConfigNameSpaceEnd   }

ConfigNameSpaceStart


#define  MaximumPathLength   100
#define  WidgetFree  "free"
#define  WidgetNew  "new"

//class  PropertySet;
//PropertySet* g_propertyset;

enum  ControlOperateState
{
    CtrlEditableState,
    CtrlNormalState
};

enum  ControlRole
{
    cCreateEntry,
    cRealControl,
    cCopycontrol,
    cTabMove
};


enum ControlType
{
    cConfigLabel,
    cConfigButton,
    cConfigCurve,
    cConfigGroup,
    cConfigPairLabel,
    cConfigAlarm
};

QString  get_uuid();
QString getXmlGroupName(ControlType ctrtype);

enum  ConfigValueType
{
    eConfigDouble,
    eConfigFloat,
    eConfigInt,
    eConfigShort,
    eConfigString,
    eConfigDataSource,
    eConfigStyleInfo,
    eConfigMultiDataSource,
    eConfigCommand
};

struct ConfigSceneInfo
{
    QString name;
    QString id;
    QString path;
};


struct  ConfigValue
{
    void * value;
    ConfigValueType  valuetype;
    QString valueid;
    QString valuename;
    ConfigValue(void* ivalue,ConfigValueType ivaluetype,QString ivaluename,int iuplimit =1000,int idownlimit = 1);
    int  uplimit;
    int downlimit;
    QString  getStrValue();
    void SetWidthStrValue(QString value);
    bool  editEabled = true;
};

struct ConfigValueSet
{
    QMap<int,ConfigValue*>  valuelist;
    QString valueSetname;
};

struct GroupElementInfo
{
    QMap<QString,QString>   groupinfo;
    QList<QMap<QString,QString>>  buttonInfolist;
    QList<QMap<QString,QString>>  PairLabelInfolist;
    QList<QMap<QString, QString>>  alarmInfolist;
};

struct SceneInfo
{
  QString sceneid;
  QString scenename;
  QString scenepath;
  bool  useschedule;
  QList<QMap<QString,QString>>  buttonInfolist;
  QList<QMap<QString,QString>>  PairLabelInfolist;
  QList<QMap<QString,QString>>  curveInfolist;
  QList<QMap<QString, QString>>  alarmInfolist;

  QList<GroupElementInfo>  groupinfolist;
};


enum  RunState
{
    StateNormal,
    StateSuccess,
    StateFailed
};
class ConfigTools:public QObject
{
    Q_OBJECT
public:
        static  bool  GetInputDouble(QString  hint, double& value);
        static bool   GetInputString(QString  hint, QString& value);
        static bool   GetInputInt(QString  hint, int& value);
};
ConfigNameSpaceEnd
#endif // CONFIGDEFINE_H
