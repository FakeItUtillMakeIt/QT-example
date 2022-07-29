#pragma once
#include <string>
#include <map>
#include <vector>

using namespace std;

namespace DataBase
{

	class FaultRocketInfo
	{
	public:
		FaultRocketInfo() {};
		~FaultRocketInfo() {};

		int m_id;
		string m_name;
		int m_code;
		string m_createTime;
		string m_lastUpdateTime;

	private:

	};

	class FaultCommandInfo
	{
	public:
		FaultCommandInfo() {};
		~FaultCommandInfo() {};

		int m_id;
		int m_rocketID;
		int m_backID;
		string m_name;
		int m_code;
		int m_type;
		//int m_faultType;
		int m_prefix;
		string m_createTime;
		string m_lastUpdateTime;

	private:

	};


	class FaultCommandParamInfo
	{
	public:
		FaultCommandParamInfo() {};
		~FaultCommandParamInfo() {};

		int m_id;
		int m_commandID;
		string m_name;
		int m_code;
		int m_index;
		int m_length;
		int m_resultType;
		float m_defaultValue;
		string m_createTime;
		string m_lastUpdateTime;

	private:

	};


	/// <summary>
	/// ���ϲ����󶨱� fault_param_info
	/// </summary>
	class FaultFaultParamInfo
	{
	public:
		FaultFaultParamInfo() {};
		~FaultFaultParamInfo() {};

		int m_id;
		string m_name;
		int m_faultType;//1:���ơ�2:������3:����
		int m_FaultCommandID; //command_info�� ����ָ���id
		int m_deviceParamInfoID;
		string m_createTime;
		string m_lastUpdateTime;

	private:

	};

	/// <summary>
	/// ����ָ��󶨱� fault_command_info
	/// </summary>
	class FaultFaultCommandInfo
	{
	public:
		FaultFaultCommandInfo() {};
		~FaultFaultCommandInfo() {};

		int m_id;
		string m_name;
		int m_faultType;//1:���ơ�2:������3:����
		int m_FaultCommandID;//command_info�� ����ָ���id
		int m_responseCommandID;//command_info�� �ָⷢ���id  //���ܴ��ڶ��
		string m_createTime;
		string m_lastUpdateTime;

	private:
	};


	/// <summary>
	/// ������Ϣ�� parameter_info
	/// </summary>
	class FaultParameterInfo
	{
	public:
		FaultParameterInfo() {};
		~FaultParameterInfo() {};

		int m_id;
		string m_name;
		int m_type;
		int m_unit;
		string m_createTime;
		string m_lastUpdateTime;

	private:
	};

	/// <summary>
	/// �豸��Ϣ�� device_info
	/// </summary>
	class FaultDeviceInfo
	{
	public:
		FaultDeviceInfo() {};
		~FaultDeviceInfo() {};

		int m_id;
		int m_rocketID;
		string m_name;
		int m_isVirtual;
		string m_createTime;
		string m_lastUpdateTime;

	private:
	};



	/// <summary>
	/// �豸������ device_param_info (�м��)
	/// </summary>
	class FaultDeviceParamInfo
	{
	public:
		FaultDeviceParamInfo() {};
		~FaultDeviceParamInfo() {};

		int m_id;
		int m_deviceID;
		int m_parameterID;
		string m_createTime;
		string m_lastUpdateTime;

	private:
	};



	/// <summary>
	/// ÿ����ӹ��ϵĹ�����Ϣ
	/// </summary>
	class AddOneFaultInfo
	{
	public:
		AddOneFaultInfo() {};
		~AddOneFaultInfo() {};

		string m_name;//��������
		int m_Type;//�������� 1���������ϣ�2��ָ�����
		int m_FaultCommandID;//����ָ��id
		vector<int> m_responseCommandID;//��Ӱ��ָ��id
		vector<int> m_deviceParamInfoID;//����id  device_param_info_id
		//int deviceID;//�豸id

	private:
	};
}


