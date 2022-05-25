#pragma once
#ifndef ROCKET_GROUND_PROTOL_FORMAT_H
#define ROCKET_GROUND_PROTOL_FORMAT_H

#include <QtCore>
#define byte unsigned char

/**

    @class   RocketGroundProtolDataFormat
    @brief   箭地通信通用协议基本数据格式
	有线RS422通信协议为固定长度263Byte（255+8）
	标准协议帧数据含义如下:
	HDA（1Byte）：头标识A，定值，0x55
	HDB（1Byte）：头标识B，定值，0xAA
	LEN（1Byte）：数据（PAYLOAD）长度，常值0xFF
	SEQ（1Byte）：帧序号，该序号指特定消息下发送该消息的次数，由0开始，到255，大于255时翻转变为0，范围0~255
	DEV（1Byte）：设备号，指发出该序号的设备ID，范围0~255
	MSG（1Byte）：消息ID，范围0x00~0xff
	PAYLOAD（非固定长度）：通信数据，各Byte范围0x00~0xFF
	CKA（1Byte）：校验和A，范围0x00~0xFF
	CKB（1Byte）：校验和B，校验和A和B组成16位CRC校验和算法结果（CKA为低字节，CKB为高字节），详细算法代码见附件，范围0x00~0xFF（计算从PAYLOAD第一位开始，至PAYLOAD最后一位）
	绿色部分代表消息数据内容，其长度为255Byte

    @details ~

**/
class RocketGroundProtolDataFormat
{
public:
	byte HDA = 0x55;
	byte HDB = 0xAA;
	byte LEN = 0XFF;
	byte SEQ;//帧序号
	byte DEV;//设备号
	byte MSG;//消息ID
	byte PAYLOAD[255];//通信数据 总长度为255 字节
	byte CKA;
	byte CKB;

};

/**

    @class   DataFormatTransform
    @brief   数据类型转换
    @details ~

**/
class DataFormatTransform
{
public:


 /**
     @brief  字节型数据转QByteArray数据，方便串口传输
     @param  input 待发送的字节型数据 数据结构为RocketGroundProtolDataFormat  协议数据
     @retval       返回协议数据，以QByteArray数据表示  ASCII码表示16进制字节数据     
 **/
	QByteArray byte2QByteArray(RocketGroundProtolDataFormat input);
 /**
     @brief  QByteArray数据转字节数据
     @param   input 已接收的QByteArray型数据 
	 @retval  返回类型数据结构为RocketGroundProtolDataFormat  协议数据

 **/
	RocketGroundProtolDataFormat qbyteArray2byte(const QByteArray& input);
protected:
private:

	char byte2ascii(byte data);

};

/**
    @enum  RocketGroundDeviceID
    @brief 箭地设备ID地址分配  枚举
**/
enum RocketGroundDeviceID
{
	GENERAL_CONTROL_MACHINE=0x01,
	SERVO_DEVICE=0x02,
	POWER_MANAGE_MODULE=0x03
};


/**
    @enum  RocketGroundMessageID
    @brief 消息ID表
	存储消息ID类型   枚举
	详细信息见OS-X1B箭地测控通信协议
**/
enum RocketGroundMessageID {
	GENERAL_CONTROL_MACHINE_POWER_ON_REQUEST = 0x01,
	GENERAL_CONTROL_MACHINE_POWER_OFF_REQUEST = 0x02,
	GENERAL_CONTROL_MACHINE_SELF_CHECK_REQUEST = 0x14,
	GENERAL_CONTROL_MACHINE_SELF_CHECK_RESULT = 0x15,
	FLIGHT_CONTROL_SOFTWARE_STATUS_QUERY = 0x16,
	FLIGHT_CONTROL_SOFTWARE_STATUS_RESULT = 0x17,
	SINGLE_DISTRIBUTION_REQUEST = 0x20,
	SINGLE_DISTRIBUTION_RESULT = 0x21,
	INTERTIAL_GROUP_DATA_MEASURE_REQUEST = 0x22,
	INTERTIAL_GROUP_DATA_MEASURE_RESULT = 0x23,
	GPS_DATA_MEASURE_REQUEST = 0x24,
	GPS_DATA_MEASURE_RESULT = 0x25,
	SERVO_ZERO_UPLOAD_REQUEST = 0x32,
	SERVO_ZERO_UPLOAD_RESULT = 0x33,
	SERVO_MEASURE_REQUEST = 0x40,
	SERVO_MEASURE_RESULT = 0x41,
	SERVO_SMALL_LOOP_MEASURE_REQUEST = 0x42,
	SERVO_SMALL_LOOP_MEASURE_RESULT = 0x43,
	SERVO_BATTERY_ACTIVATE_REQUEST = 0x44,
	SERVO_BATTERY_ACTIVATE_RESULT = 0x45,
	META_DATA_CONTROL_REQUEST = 0x50,
	META_DATA_CONTROL_RESULT = 0x51,
	FLIGHT_CONTROL_SOFTWARE_UPLOAD_CONTROL = 0x5a,
	FLIGHT_CONTROL_SOFTWARE_UPLOAD_STATUS = 0x5b,
	FLIGHT_CONTROL_SOFTWARE_DATA_UPLOAD_REQUEST = 0x5c,
	FLIGHT_CONTROL_SOFTWARE_DATA_UPLOAD_RESULT = 0x5d,
	FLIGHT_CONTROL_SOFTWARE_DATA_CHECK_REQUEST = 0x5e,
	FLIGHT_CONTROL_SOFTWARE_DATA_CHECK_RESULT = 0x5f,
	META_DATA_BINDING_REQUEST = 0x60,
	META_DATA_BINDING_RESULT = 0x61,
	META_DATA_DOWNLOAD_REQUEST = 0x62,
	META_DATA_DOWNLOAD_RESULT = 0x63,
	META_DATA_CHECK_REQUEST = 0x64,
	META_DATA_CHECK_RESULT = 0x65,
	META_DATA_LOAD_REQUEST = 0x66,
	META_DATA_LOAD_RESULT = 0x67,
	SIMULATION_FLIGHT_DATA_UPLOAD_REQUEST = 0x68,
	SIMULATION_FLIGHT_DATA_UPLOAD_RESULT = 0x69,
	EXPLOSIVE_PORT_CHECK_REQUEST = 0x6a,
	EXPLOSIVE_PORT_CHECK_RESULT = 0x6b,
	EXPLOSIVE_RESIST_MEASURE_REQUEST = 0x70,
	EXPLOSIVE_RESIST_MEASURE_RESULT = 0x71,
	LEVEL_COMPUTE_REQUEST = 0x72,
	LEVEL_COMPUTE_RESULT = 0x73,
	TRUN_NAVIGATE_REQUEST = 0xe0,
	TRUN_NAVIGATE_RESULT = 0xe1,
	PRE_LUANCH_COMMAND = 0xf6,
	PRE_LUANCH_COMMAND_RESULT = 0xf7,
	LUANCH_FIRE_COMMAND = 0xf8,
	SAFE_CONTROL_PRE_COMMAND = 0xf9,
	SAFE_CONTROL_ACTIVATE_COMMAND = 0xfa


};

void test();

#endif