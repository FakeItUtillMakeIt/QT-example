#pragma once 
#include "CommandParam.h"
#include <qvariant.h>
#include <string>
#include <vector>
using namespace std;
class Command
{
public:
    Command();
    ~Command();
    int m_id;
    string m_sName;// ����
    int m_iType; //ָ�����ͣ�1��ʾ�ָⷢ�2��ʾ�ⷢ���3��ʾ����ָ��
    int m_iCode;//ָ�����,һ��ʮ��������
    int m_iRocketId; //����ͺ�
    string m_sRocketName; //����ͺ�����
    int m_iBackId;//�ⷢ����ID
    vector<CommandParam*> m_vCommandParam;//�����б�
};
Q_DECLARE_METATYPE(Command*);
 



 
