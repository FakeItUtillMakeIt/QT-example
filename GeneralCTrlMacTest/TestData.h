#pragma once

#ifndef TEST_DATA_H
#define TEST_DATA_H


#include "BaseProtocolFormat.h"
#include "crc_onespace.h"
#include <QByteArray>

#include <QtGlobal>


namespace TestData {


/**

    @class   GeneralMacInsideVoltAndExplsResTestData
    @brief   综控机内部电压及火工品阻值测试数据
    @details ~

**/
class GeneralMacInsideVoltAndExplsResTestData
{
public:
	GeneralMacInsideVoltAndExplsResTestData();
private:
	BaseProtocolFormat* testData;
public:
	BaseProtocolFormat* getTestData();

};


/**

    @class   SerialPortCommTestData
    @brief   串口通信测试数据
    @details ~

**/
class SerialPortCommTestData
{
public:
	SerialPortCommTestData();
private:
	BaseProtocolFormat* testData;
public:
	BaseProtocolFormat* getTestData();

};

/**

    @class   SerialPortCommTestData
    @brief   串口通信测试数据
    @details ~

**/
class MutliSerialPort3CommTestData
{
public:
    MutliSerialPort3CommTestData();
private:
    BaseProtocolFormat* testData;
public:
    BaseProtocolFormat* getTestData();

};

/**

    @class   SerialPortCommTestData
    @brief   串口通信测试数据
    @details ~

**/
class MutliSerialPort1CommTestData
{
public:
    MutliSerialPort1CommTestData();
private:
    BaseProtocolFormat* testData;
public:
    BaseProtocolFormat* getTestData();

};

/**

    @class   SerialPortCommTestData
    @brief   串口通信测试数据
    @details ~

**/
class MutliSerialPort2CommTestData
{
public:
    MutliSerialPort2CommTestData();
private:
    BaseProtocolFormat* testData;
public:
    BaseProtocolFormat* getTestData();

};

/**

    @class   AnalogCaptureTestData
    @brief   模拟量采集测试数据
    @details ~

**/
class AnalogCaptureTestData
{
public:
	AnalogCaptureTestData();
private:
	BaseProtocolFormat* testData;
public:
	BaseProtocolFormat* getTestData();

};

/**

    @class   SwitchOutputTestData
    @brief   开关量输出测试数据
    @details ~

**/
class SwitchOutputTestData
{
public:
	SwitchOutputTestData();
private:
	BaseProtocolFormat* testDataA;
	BaseProtocolFormat* testDataB;
public:
	BaseProtocolFormat* getTestDataA();
	BaseProtocolFormat* getTestDataB();

};

/**

    @class   SwitchInputTestData
    @brief   开关量输入测试数据
    @details ~

**/
class SwitchInputTestData
{
public:
	SwitchInputTestData();
private:
	BaseProtocolFormat* testData;
public:
	BaseProtocolFormat* getTestData();

};

/**

    @class   SequentialOutputTestData
    @brief   时序输出测试数据
    @details ~

**/
class SequentialOutputTestData
{
public:
    SequentialOutputTestData();
private:

    BaseProtocolFormat* sequentialOutputTest1ONData;
    BaseProtocolFormat* sequentialOutputTest1OFFData;
    BaseProtocolFormat* sequentialOutputTest2ONData;
    BaseProtocolFormat* sequentialOutputTest2OFFData;
public:
    BaseProtocolFormat* getSequentialOutputTest1ONData();
    BaseProtocolFormat* getSequentialOutputTest1OFFData();
    BaseProtocolFormat* getSequentialOutputTest2ONData();
    BaseProtocolFormat* getSequentialOutputTest2OFFData();

};

/**

    @class   SatelliteNavgAndRemoteMeasTestData
    @brief   卫导模块及遥测模块测试数据
    @details ~

**/
class SatelliteNavgAndRemoteMeasTestData
{
public:
	SatelliteNavgAndRemoteMeasTestData();
private:
	BaseProtocolFormat* testData;
public:
	BaseProtocolFormat* getTestData();

};


class NoSourceInputTestData{

public:
    NoSourceInputTestData();
private:
    BaseProtocolFormat* testData;
public:
    BaseProtocolFormat* getTestData();
};


/**

    @class   __unnamed_class_0344_1
    @brief   返回测试结果验证
    @details ~

**/
class RetTestResultValidate
{
public:
	static void retTestDataValidate(BaseProtocolFormat* data, int& testType) ;
};

enum TestMSGRetValue
{
	GENERAL_MAC_INSIDE_VOLT_AND_EXPLOSIVE_RESIST_MSG = 0x06,
    SERIAL_PORT_MSG = 0x08,
	ANOLOG_CAPTURE_A_MSG = 0x0a,
	ANOLOG_CAPTURE_B_MSG = 0x0b,
	SWITCH_OUTPUT_MSG = 0x0d,
	SWITCH_INPUT_MSG = 0x0f,
	SEQUENTIAL_OUTPUT_MSG = 0xd1,
    NO_SOURCE_INPUT_MSG=0xd1,
	SATELLITE_NAG_AND_REMOTE_MEASURE_MSG = 0xd3
};

enum TestDevRetValue
{
	GENERAL_MAC_INSIDE_VOLT_AND_EXPLOSIVE_RESIST_DEV = 0x01,
    SERIAL_PORT_DEV = 0x04,
	ANOLOG_CAPTURE_DEV = 0x01,
	SWITCH_OUTPUT_DEV = 0x03,
    SWITCH_INPUT_DEV = 0x11,
    SEQUENTIAL_OUTPUT_DEV = 0x13,
    NO_SOURCE_INPUT_DEV=0x13,
	SATELLITE_NAG_AND_REMOTE_MEASURE_DEV = 0x03
};

enum TestType
{
	GENERAL_MAC_INSIDE_VOLT_AND_EXPLOSIVE_RESIST = GENERAL_MAC_INSIDE_VOLT_AND_EXPLOSIVE_RESIST_DEV * GENERAL_MAC_INSIDE_VOLT_AND_EXPLOSIVE_RESIST_MSG,
	SERIAL_PORT = SERIAL_PORT_DEV * SERIAL_PORT_MSG,
	ANOLOG_CAPTURE_A = ANOLOG_CAPTURE_DEV * ANOLOG_CAPTURE_A_MSG,
	ANOLOG_CAPTURE_B = ANOLOG_CAPTURE_DEV * ANOLOG_CAPTURE_B_MSG,
	SWITCH_OUTPUT = SWITCH_OUTPUT_DEV*SWITCH_OUTPUT_MSG,
	SWITCH_INPUT = SWITCH_INPUT_DEV*SWITCH_INPUT_MSG,
	SEQUENTIAL_OUTPUT = SEQUENTIAL_OUTPUT_DEV*SEQUENTIAL_OUTPUT_MSG,
	SATELLITE_NAG_AND_REMOTE_MEASURE = SATELLITE_NAG_AND_REMOTE_MEASURE_DEV*SATELLITE_NAG_AND_REMOTE_MEASURE_MSG
};


}
#endif
