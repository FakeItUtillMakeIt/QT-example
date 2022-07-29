#pragma once
#include <string>
#include <map>
using namespace std;
class Device
{
public:
    Device();
    ~Device(); 
     
    int m_id;
    string m_deviceName; 
    int isVirtual; //0表示真实设备，1表示虚拟设备
    int m_iRocketId; //火箭型号
    string m_sRocketName; //火箭型号名称

    string m_sCurStatus;//当前状态：配电、断电、故障
};
 
