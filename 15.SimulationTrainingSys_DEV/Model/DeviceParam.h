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
    int m_id;//�����Ψһ��ʶ�������ڰ���̬�ؼ�
    int m_deviceId;
    string m_deviceName;  
    int m_subParameterId;//�������parameter_info.id
    string m_subParameterName; 
    string m_unit;//��λ 
    double m_iCurValue;//������ʵʱֵ
    string m_iCurStr;//�������ı�ֵ
     
    string m_status;//����״̬
    map<string, vector<double>> m_statusRealData;//ÿ��״̬��Ӧ��ʵʱֵ�б�
};
 
