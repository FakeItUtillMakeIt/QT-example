#include "RocketGroundBaseProtol.h"



quint32 RocketGroundBaseProtol::parseRocketGroundProtol(byte* input_buf, byte* ret_buf) {
	byte dataLen = 0xff;
	quint32 retDataLen = 0;
	do 
	{
		//
		if ((input_buf == nullptr) || ret_buf == nullptr)
		{
			retDataLen = 0;
			break;
		}
		//检查协议头
		if ((input_buf[0]==HDA) && (input_buf[1]==HDB))
		{
			dataLen = input_buf[2];
			//读取检验和 CKB为高位，CKA为低位
			quint16 crc_ret = ((input_buf[6 + 255+1]) << 8 | (input_buf[6 + 255 ]) << 0);
			//计算校验和
			CRC16 crc16(CRC16::eCCITT_FALSE);
			//
			quint16 crc_cal = crc16.crcCompute((char*)(input_buf + 6), 255);
			//比较两者校验和
			if (crc_ret!=crc_cal)
			{
				retDataLen = 0;
				break;;

			}
			else
			{
				//除去帧头、检验和外的帧长度
				retDataLen = 255 + 8 - 2 - 2;
				for (int i=0;i<retDataLen;i++)
				{
					ret_buf[i] = input_buf[i + 2];
				}
			}
		}
	} while (0);
	//返回解析后的有效长度数据
	return retDataLen;
}

//解析通用协议中的具体协议要求，
bool RocketGroundBaseProtol::parseProtolRequest() {
	SEQ = FRAME_TAB[1];
	DEV = FRAME_TAB[2];
	MSG = FRAME_TAB[3];
	switch (DEV)
	{
	case 0x01://综控机
		//进入具体设备
		
		break;
	case 0x02://伺服
		break;
	case 0x03://电源管理
		break;
	default:
		break;
	}
	return true;
}

void RocketGroundBaseProtol::generalControlMachineDevice() {


}

void RocketGroundBaseProtol::servoSystemDevice() {


}

void RocketGroundBaseProtol::powerManageModule() {


}
