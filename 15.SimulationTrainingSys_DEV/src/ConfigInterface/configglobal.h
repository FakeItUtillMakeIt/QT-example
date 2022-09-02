#ifndef CONFIGGLOBAL_H
#define CONFIGGLOBAL_H
#include "configdefine.h"
#include "../Model/DeviceParam.h"
#include "../Model/Command.h"
#include <map>
#include <QVector>
using namespace std;
//class CenterOperate;


ConfigNameSpaceStart
class PropertySet;
class StyleManager;
class ConfigCurve;
class ConfigInterface;
class ConfigButton;
class ConfigPairLabel;
class ConfigAlarm;
class ConfigGlobal
{
public:
    ConfigGlobal();
    static PropertySet* gpropeetyset;
    static StyleManager* gstylemanager;
    static ConfigInterface* gconfiginterface;
    static void updateCurveStyle(QList<ConfigCurve *> styleusers);
    static  bool  isEditing;
    static map<int, Command*>* m_allCommadPrt;//测发指令
    static map<int, Command*>* m_allFaultCommnd;
    static map<int, DeviceParam*>* m_allDeviceParamPtr;//设备参数
    static bool  VerifyNumber(QString str);
  //  static CenterOperate* m_pCenterOperate;
 //   static void processCMD(int cmd, ConfigButton* btn);
    static QMap<int,QList<ConfigPairLabel*>> labelmap;
    static QMap<int, QList<ConfigCurve*>>  curvemap;
    static QMap<int, QList<ConfigAlarm*>>  alarmmap;
    static QMap<int, ConfigButton*>  buttonmap;//测发指令
    
    static QMap<ConfigCurve*, QList<QString>>  reversecurvemap;//测发指令

    static void  updateLabelMap(int key, ConfigPairLabel*);
    static void  updateButtonMap(int key, ConfigButton*);
    static void  updateAlarmMap(int key, ConfigAlarm* alarm);
    static void  updateCurveMap(QList<QString>  keys, ConfigCurve*);
    static void  updateControlValue(int validity, int key,double itime, double value, int error = 0);
    static void  updateControlText(int key,QString  text,int error = 0);
    static bool  dataupdated;
    static void(*cmdhandler)(int, ConfigNameSpace::ConfigButton*);
    static void updateDefaultState();
    static QString currentRocket; 
    static int currentRocketID;
    static QString currentSoftWare;
    static int currentSoftWareID;
    static bool  monitorRunning;
    static QVector<int> mainSchedule;
}; 


ConfigNameSpaceEnd
#endif // CONFIGGLOBAL_H
