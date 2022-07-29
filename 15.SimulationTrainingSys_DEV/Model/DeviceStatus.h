#pragma once
#include <string>
#include <map>
using namespace std;
class DeviceStatus
{
public:
    DeviceStatus();
    ~DeviceStatus(); 
     
    int m_id;
    int m_deviceId; //设备ID
    string m_statusName; //状态名称
    string m_dataPath; //实时值文件路径 
};
 
