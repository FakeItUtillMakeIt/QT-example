#include "configglobal.h"
#include "propertyset.h"
#include "stylemanager.h"

//#include "../ControlCommand/UI/CenterOperate.h"
#include "configdefine.h"
#include "configcurve.h"
#include "configbutton.h"
#include "configpairlabel.h"
#include "configalarm.h"
#include <qmessagebox.h>
ConfigNameSpaceStart

PropertySet* ConfigGlobal::gpropeetyset;
StyleManager* ConfigGlobal::gstylemanager;
ConfigInterface* ConfigGlobal::gconfiginterface;
QSize  ConfigGlobal::scenesize;

//CenterOperate* ConfigGlobal::m_pCenterOperate = nullptr;

bool  ConfigGlobal::isEditing = false;

map<int, Command*>*  ConfigGlobal::m_allCommadPrt;//测发指令
map<int, DeviceParam*>*  ConfigGlobal::m_allDeviceParamPtr = nullptr;
map<int, Command*>* ConfigGlobal::m_allFaultCommnd = nullptr;//当前故障指令


QMap<int, QList<ConfigPairLabel*>> ConfigGlobal::labelmap;
QMap<int, QList<ConfigCurve*>>  ConfigGlobal::curvemap;
QMap<int, QList<ConfigAlarm*>>  ConfigGlobal::alarmmap;
QMap<int, ConfigButton*>  ConfigGlobal::buttonmap;

QMap<ConfigCurve*, QList<QString>>  ConfigGlobal::reversecurvemap;//以curve 做key，记录其包含的所有参数
bool  ConfigGlobal::dataupdated = false;

QString ConfigGlobal::currentRocket;
int ConfigGlobal::currentRocketID;
QString ConfigGlobal::currentSoftWare = "大家的";
int ConfigGlobal::currentSoftWareID = 2;
bool ConfigGlobal::monitorRunning;
QVector<int> ConfigGlobal::mainSchedule;

void(*ConfigGlobal::cmdhandler)(int, ConfigNameSpace::ConfigButton*) = nullptr;

ConfigGlobal::ConfigGlobal()
{
    ConfigCurve curve;
}
//void ConfigGlobal::processCMD(int cmd, ConfigButton* btn)
//{
//    if (!m_pCenterOperate)
//    {
//        QMessageBox::warning(nullptr, ("错误"), ("命令处理接口未初始化"));
//        return;
//    }
//    m_pCenterOperate->sendCMDFromInterface(cmd, btn);
//}
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
void   ConfigGlobal::updateControlValue(int validity,int key,double itime,double value,int error)
{
    //if (key == 0x13)
    //{
    //    qDebug() << "value:" << value;
    //}
    if (labelmap.contains(key))
    {
        for (auto label : labelmap[key])
        {
            label->updateValue(value, error);
        }
    }
    if (curvemap.contains(key))
    {
        for (auto curve : curvemap[key])
        {
            curve->updateValue(key,itime,value);
        }
    }
}
void  ConfigGlobal::updateControlText(int key,QString  text, int error)
{
   
    if (labelmap.contains(key))
    {
        for (auto label : labelmap[key])
        {
            label->updateText(text, error);
        }
    }
    if (alarmmap.contains(key))
    {
        for (auto alarm : alarmmap[key])
        {
            RunState runstate = (error == 3?StateFailed:StateSuccess);
            alarm->setState(runstate);       
        }
    }
}
void   ConfigGlobal::updateDefaultState()
{
    if (m_allFaultCommnd == nullptr) return;
    if (isEditing)  return;
    for (auto key : buttonmap.keys())
    {
        if (m_allFaultCommnd->find(key) != m_allFaultCommnd->end())
        {
            buttonmap[key]->setDisabled(true);
        }
        else
        {
            buttonmap[key]->setEnabled(true);
        }
    }
}
void   ConfigGlobal::updateButtonMap(int key, ConfigButton* btn)
{

    if (buttonmap.contains(key) && buttonmap[key] != btn)
    {        
        buttonmap[key]->setState(StateNormal);
        buttonmap[key]->setDisabled(false);
    }
    buttonmap[key] = btn;   
}
void   ConfigGlobal::updateAlarmMap(int key, ConfigAlarm* alarm)
{
    for (auto key : alarmmap.keys())
    {
        for (int i = 0; i < alarmmap[key].size(); i++)
        {
            if (alarmmap[key][i] == alarm)
            {
                alarmmap[key].removeAt(i);
                break;
            }
        }
    }
    alarmmap[key].push_back(alarm);
}

void   ConfigGlobal::updateLabelMap(int key, ConfigPairLabel* label)
{   
    for (auto key : labelmap.keys())
    {
        for (int i = 0; i < labelmap[key].size(); i++)
        {
            if (labelmap[key][i] == label)
            {
                labelmap[key].removeAt(i);
                break;
            }
        }       
    }
    labelmap[key].push_back(label);
}
void   ConfigGlobal::updateCurveMap(QList<QString>  keys, ConfigCurve* curve)
{
    //QMap<int, QList<ConfigCurve*>>  ConfigGlobal::curvemap;//测发指令
     //QMap<ConfigCurve*, QList<QString>>  ConfigGlobal::reversecurvemap;//以curve 做key，记录其包含的所有参数

    if (reversecurvemap.contains(curve))
    {
        QList<QString>  oldkeylist = reversecurvemap[curve];//旧的全部删除，新的全量增加
        for (auto oldkey : oldkeylist)
        {
            curvemap[oldkey.toInt()].removeOne(curve);
        }
        reversecurvemap[curve].clear();

    }
    reversecurvemap[curve].append(keys);
    
    for (QString key : keys)
    {
        curvemap[key.toInt()].push_back(curve);
    }

}

ConfigNameSpaceEnd
