#ifndef CONFIGGLOBAL_H
#define CONFIGGLOBAL_H
#include "configdefine.h"
#include "../Model/DeviceParam.h"
#include "../Model/Command.h"
#include <map>
using namespace std;
ConfigNameSpaceStart
class PropertySet;
class StyleManager;
class ConfigCurve;
class ConfigInterface;
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
    static map<int, DeviceParam*>* m_allDeviceParamPtr;//设备参数
    static bool  VerifyNumber(QString str);

};

ConfigNameSpaceEnd
#endif // CONFIGGLOBAL_H
