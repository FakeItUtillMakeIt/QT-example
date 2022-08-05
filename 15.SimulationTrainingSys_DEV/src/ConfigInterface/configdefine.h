﻿#include <QList>
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
    cCopycontrol
};


enum ControlType
{
    cConfigLabel,
    cConfigButton,
    cConfigCurve,
    cConfigGroup,
    cConfigPairLabel
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
};

struct SceneInfo
{
  QString sceneid;
  QString scenename;
  QString scenepath;
  QList<QMap<QString,QString>>  buttonInfolist;
  QList<QMap<QString,QString>>  PairLabelInfolist;
  QList<QMap<QString,QString>>  curveInfolist;

  QList<GroupElementInfo>  groupinfolist;
};



ConfigNameSpaceEnd
#endif // CONFIGDEFINE_H