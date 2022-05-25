#pragma once
#ifndef ROCKET_GROUND_PROTOL_FORMAT_H
#define ROCKET_GROUND_PROTOL_FORMAT_H

#include <QtCore>
#define byte quint8

class RocketGroundProtolDataFormat
{
	byte HDA = 0x55;
	byte HDB = 0xAA;
	byte LEN = 0XFF;
	byte SEQ;//帧序号
	byte DEV;//设备号
	byte MSG;//消息ID
	byte PAYLOAD[255];//通信数据，非固定长度 总长度为255  
	byte CKA;
	byte CKB;

};

#endif