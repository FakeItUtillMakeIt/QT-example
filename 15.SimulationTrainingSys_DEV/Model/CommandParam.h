#pragma once
#include <qvariant.h>
#include <string> 
using namespace std;

union Param_Value
{
    char val_c;
    short val_s;
    int val_i;
    float val_f;
    unsigned char val_uc;
    unsigned short val_us;
    unsigned int val_ui;
};

//ָ�����
class CommandParam
{
public:
    CommandParam();
    ~CommandParam();
    int m_id;
	int m_iCommand_id; //ָ��ID
    string m_sName;//����
	int m_iCode;//��������,һ��ʮ��������
	int m_iIndex; //���к�
    int m_iLength;//�ֽڳ���
	string m_sResultType; //��������
    float m_dDefaultValue;//Ĭ��ֵ
    Param_Value m_val;
};




 
