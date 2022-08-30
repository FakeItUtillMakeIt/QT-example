#pragma once
#include <string>
#include <map>
#include <vector>

using namespace std;

namespace DataBaseF
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

		int m_id;//唯一标识符
		int m_rocketID;//火箭型号
		int m_backID;//测发回令  测发指令与故障指令可设置绑定回令back_id，其他情况下back_id设置为0。
		string m_name;//指令名称
		int m_code;//指令编码  唯一
		int m_type;//指令类型:1表示测发指令、2表示测发回令、3表示故障指令
		int m_prefix;//指令前缀:通常也是一个十六进制数，例如0x55AA
		string m_createTime;//创建时间
		string m_lastUpdateTime;//最后更新时间

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
	/// 故障参数绑定表 fault_param_info
	/// </summary>
	class FaultFaultParamInfo
	{
	public:
		FaultFaultParamInfo() {};
		~FaultFaultParamInfo() {};

		int m_id;
		int m_rocketID;
		string m_name;
		int m_faultType;//1:控制、2:测量、3:动力
		int m_FaultCommandCode; //command_info中 故障指令的code
		int m_deviceParamInfoID;
		string m_createTime;
		string m_lastUpdateTime;

	private:

	};

	/// <summary>
	/// 故障指令绑定表 fault_command_info
	/// </summary>
	class FaultFaultCommandInfo
	{
	public:
		FaultFaultCommandInfo() {};
		~FaultFaultCommandInfo() {};

		int m_id;
		int m_rocketID;
		string m_name;
		int m_faultType;//1:控制、2:测量、3:动力
		int m_FaultCommandCode;//command_info中 故障指令的code
		int m_responseCommandID;//command_info中 测发指令的id  //可能存在多个
		string m_createTime;
		string m_lastUpdateTime;

	private:
	};


	/// <summary>
	/// 参数信息表 parameter_info
	/// </summary>
	class FaultParameterInfo
	{
	public:
		FaultParameterInfo() {};
		~FaultParameterInfo() {};

		int m_id;
		int m_rocketId;
		string m_name;
		int m_type;
		int m_unit;
		string m_createTime;
		string m_lastUpdateTime;

	private:
	};

	/// <summary>
	/// 设备信息表 device_info
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
	/// 设备参数表 device_param_info (中间表)
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
	/// 每次添加故障的故障信息
	/// </summary>
	class AddOneFaultInfo
	{
	public:
		AddOneFaultInfo() {};
		~AddOneFaultInfo() {};

		string m_name;//故障名称
		int m_Type;//故障类型 1：参数故障；2：指令故障
		int faultType;//1:控制、2:测量、3:动力
		int m_FaultCommandCode;//故障指令id
		vector<int> m_responseCommandID;//受影响指令id
		vector<int> m_deviceParamInfoID;//参数id  device_param_info_id
		//int deviceID;//设备id

	private:
	};
}


