#pragma once
#ifndef ROCKET_GROUND_BASE_PROTOL_H
#define ROCKET_GROUND_BASE_PROTOL_H

#include <QtCore>
#include "CrcCompute.h"
#include "RocketGroundProtolDataFormat.h"
#include "RocketBaseDevice.h"

using namespace std;

//定义字节
#define byte unsigned char
//通用协议格式
//RS422箭地通信协议：固定长度  255+8-2-2 字节 除去帧头和检验和
class RocketGroundBaseProtol
{
public:
	byte HDA = 0x55;
	byte HDB = 0xAA;
	byte LEN = 0XFF;
	byte SEQ;//帧序号
	byte DEV;//发送此数据的设备号
	byte MSG;//消息ID
	byte PAYLOAD[255];//通信数据，非固定长度 总长度为255  
	byte CKA;
	byte CKB;

private:
	byte FRAME_TAB[4 + 255];

public:
	virtual void deviceOperate() {

	}

	
 /**
     @brief  //解析箭地协议并返回协议数据长度6-2+255+2-2
     @param  input_buf 输入数据
     @param  ret_buf  解析后的数据 
     @retval 协议数据长度          
 **/
	quint32 parseRocketGroundProtol(byte* input_buf, byte* ret_buf);
	quint32 parseRocketGroundProtol(const QByteArray input_buf, QByteArray& ret_buf);
 /**
     @brief  解析通用协议中的具体协议要求，
     @retval 
 **/
	QByteArray parseProtolRequest(QByteArray& frameData);

private:

	//设备
	GeneralControlMachineDevice generalControlMachine;

private:
	RocketGroundProtolDataFormat* retFrameData = nullptr;

};



//PAYLOAD部分 
class RocketDeviceProtol
{
public:
	byte EXECUTE_STATE;//no.7byte
	byte EXECUTE_RESULT;//no.8byte
	byte DEVICE_OVER_STATE;//no.9byte
	byte DEVICE_CHECK_RESULT;//no.10byte
	byte DEVICE_COM_STATE;//no.11byte

	byte EXPLOSIVE_DEVICE_TEST[2];//no.16-17
protected:
private:
};



#endif