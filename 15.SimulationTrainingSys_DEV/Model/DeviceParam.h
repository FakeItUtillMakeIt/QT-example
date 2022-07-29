#pragma once
#include <string>
#include <map>
#include <vector>
using namespace std;
class DeviceParam
{
public:
    DeviceParam();
    ~DeviceParam(); 
    int m_id;//这个是唯一标识符，用于绑定组态控件
    int m_deviceId;
    string m_deviceName;  
    int m_subParameterId;//外键引用parameter_info.id
    string m_subParameterName; 
    string m_unit;//单位 
    double m_iCurValue;//连续量实时值
    string m_iCurStr;//开关量文本值
     
    string m_status;//参数状态
    map<string, vector<double>> m_statusRealData;//每个状态对应的实时值列表
};
 
