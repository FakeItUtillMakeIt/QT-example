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
    int isVirtual; //0��ʾ��ʵ�豸��1��ʾ�����豸
    int m_iRocketId; //����ͺ�
    string m_sRocketName; //����ͺ�����

    string m_sCurStatus;//��ǰ״̬����硢�ϵ硢����
};
 
