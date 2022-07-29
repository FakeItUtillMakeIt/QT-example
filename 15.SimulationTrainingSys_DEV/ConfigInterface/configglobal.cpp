#include "configglobal.h"
#include "propertyset.h"
#include "stylemanager.h"
#include "configcurve.h"

ConfigNameSpaceStart

PropertySet* ConfigGlobal::gpropeetyset;
StyleManager* ConfigGlobal::gstylemanager;
ConfigInterface* ConfigGlobal::gconfiginterface;

bool  ConfigGlobal::isEditing = false;

map<int, Command*>*  ConfigGlobal::m_allCommadPrt;//测发指令
map<int, DeviceParam*>*  ConfigGlobal::m_allDeviceParamPtr;//设备参数

ConfigGlobal::ConfigGlobal()
{
    ConfigCurve curve;
}

void ConfigGlobal::updateCurveStyle(QList<ConfigCurve *> styleusers)
{
    for(ConfigCurve* curve: styleusers)
    {
        curve->UpdatePropertyByStyle();
    }
}
bool  ConfigGlobal::VerifyNumber(QString str)
{
    std::string temp = str.toStdString();
    for (int i = 0; i < str.length(); i++)
    {
        if (temp[i] < '0' || temp[i]>'9')
        {
            return false;
        }
    }

    return true;
}
ConfigNameSpaceEnd
