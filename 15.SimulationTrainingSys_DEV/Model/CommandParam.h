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

//指令参数
class CommandParam
{
public:
    CommandParam();
    ~CommandParam();
    int m_id;
	int m_iCommand_id; //指令ID
    string m_sName;//名称
	int m_iCode;//参数编码,一个十六进制数
	int m_iIndex; //序列号
    int m_iLength;//字节长度
	string m_sResultType; //参数类型
    float m_dDefaultValue;//默认值
    Param_Value m_val;
};




 
