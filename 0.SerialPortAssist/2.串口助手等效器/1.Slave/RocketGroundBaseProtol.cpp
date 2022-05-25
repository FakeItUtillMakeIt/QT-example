#include "RocketGroundBaseProtol.h"

quint32 RocketGroundBaseProtol::parseRocketGroundProtol(const QByteArray input_buf, QByteArray& ret_buf) {
	byte dataLen = 0xff;

	QByteArray input_buf1;
	input_buf1.resize(263);
	for (int i=0;i<263;i++)
	{
		input_buf1[i] = input_buf[i];
	}

	quint32 retDataLen = 0;
	do
	{
		//
		if ((input_buf1 == nullptr))
		{
			retDataLen = 0;
			break;
		}
		//检查协议头
		byte headA = input_buf1[0];
		byte headB = input_buf1[1];
		if ((headA == HDA) && (headB== HDB))
		{
			dataLen = input_buf1[2];
			//读取检验和 CKB为高位，CKA为低位
			quint8 crcA = input_buf1[6 + 255];
			quint8 crcB = input_buf1[6 + 255 + 1];
			
			quint16 crcBC,crcAC;
			crcAC = crcA << 0;
			crcBC = crcB << 8;
			quint16 crc_ret = (crcBC | crcAC);
			//计算校验和
			CRC16 crc16(CRC16::eCCITT_FALSE);
			//只校验PAYLOAD
			quint16 crc_cal = crc16.crcCompute((char*)(input_buf1.data() + 6), 255);
			//比较两者校验和
			if (crc_ret != crc_cal)
			{
				retDataLen = 0;
				break;;

			}
			else
			{
				
				retDataLen = 255 + 8 ;
				ret_buf = input_buf1;
			}
		}
	} while (0);
	//返回解析后的有效长度数据
	return retDataLen;
}

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


/**
    @brief  //解析通用协议中的具体协议要求，根据MSG消息指令进行相应响应
    @param  frameData 
    @retval           
**/
QByteArray RocketGroundBaseProtol::parseProtolRequest(QByteArray& frameData) {

	RocketGroundProtolDataFormat responseData;
	LEN = frameData[2];
	SEQ = frameData[3];
	DEV = frameData[4];
	MSG = frameData[5];
	responseData.LEN = frameData[2];
	responseData.SEQ = frameData[3];
	responseData.DEV = frameData[4];
	responseData.MSG = frameData[5];
	//retFrameData->CKA = frameData[0];
	//retFrameData->CKB = frameData[1];
	//retFrameData->LEN = frameData[2];
	//retFrameData->SEQ = frameData[3];
	//retFrameData->DEV
	for (int i=0;i<255;i++)
	{
		PAYLOAD[i] = frameData[6 + i];
		responseData.PAYLOAD[i] = frameData[6 + i];
	}
	switch (MSG)
	{
	case GENERAL_CONTROL_MACHINE_POWER_ON_REQUEST:
		generalControlMachine.generalMachinePowerOn(responseData);
		return nullptr;
		break;
	case GENERAL_CONTROL_MACHINE_POWER_OFF_REQUEST:
		generalControlMachine.generalMachinePowerOff(responseData);
		return nullptr;
		break;
	case GENERAL_CONTROL_MACHINE_SELF_CHECK_REQUEST:
		generalControlMachine.generalMachineSelfCheck(responseData);
		
		retFrameData=&generalControlMachine.generalMachineSelfCheckResult(responseData);
		break;
	case FLIGHT_CONTROL_SOFTWARE_STATUS_QUERY:
		generalControlMachine.flightControlSoftwareStatusQuery(responseData);
		
		retFrameData = &generalControlMachine.flightControlSoftwareStatusQueryResult(responseData);
		break;
	case SINGLE_DISTRIBUTION_REQUEST:
		generalControlMachine.singleDistribution(responseData);
		
		retFrameData = &generalControlMachine.singleDistributionResult(responseData);
		break;
	case INTERTIAL_GROUP_DATA_MEASURE_REQUEST:
		generalControlMachine.intertialGroupDataMeasure(responseData);
		
		retFrameData = &generalControlMachine.intertialGroupDataMeasureResult(responseData);
		break;
	case GPS_DATA_MEASURE_REQUEST:
		generalControlMachine.gpsDataMeasure(responseData);
		
		retFrameData = &generalControlMachine.gpsDataMeasureResult(responseData);
		break;
	case SERVO_ZERO_UPLOAD_REQUEST:
		generalControlMachine.servoZeroBinding(responseData);
		
		retFrameData = &generalControlMachine.servoZeroBindingResult(responseData);
		break;
	case SERVO_MEASURE_REQUEST:
		generalControlMachine.servoMeasure(responseData);
		
		retFrameData = &generalControlMachine.servoMeasureResult(responseData);
		break;
	case SERVO_SMALL_LOOP_MEASURE_REQUEST:
		generalControlMachine.servoSmallLoopMeasure(responseData);
		
		retFrameData = &generalControlMachine.servoSmallLoopMeasureResult(responseData);
		break;
	case SERVO_BATTERY_ACTIVATE_REQUEST:
		generalControlMachine.servoBatteryActivate(responseData);
		
		retFrameData = &generalControlMachine.servoBatteryActivateResult(responseData);
		break;
	case META_DATA_CONTROL_REQUEST:
		generalControlMachine.metaDataControl(responseData);
		
		retFrameData = &generalControlMachine.metaDataControlResult(responseData);
		break;
	case FLIGHT_CONTROL_SOFTWARE_UPLOAD_CONTROL:
		generalControlMachine.flightControlSoftwareUploadControl(responseData);
		
		retFrameData = &generalControlMachine.flightControlSoftwareUploadStatus(responseData);
		break;
	case FLIGHT_CONTROL_SOFTWARE_DATA_UPLOAD_REQUEST:
		generalControlMachine.flightControlSoftwareDataUpload(responseData);
		
		retFrameData = &generalControlMachine.flightControlSoftwareDataUploadResult(responseData);
		break;
	case FLIGHT_CONTROL_SOFTWARE_DATA_CHECK_REQUEST:
		generalControlMachine.flightControlSoftwareDataCheck(responseData);
		
		retFrameData = &generalControlMachine.flightControlSoftwareDataCheckResult(responseData);
	case META_DATA_BINDING_REQUEST:
		generalControlMachine.metaDataUpload(responseData);
		
		retFrameData = &generalControlMachine.metaDataUploadResult(responseData);
		break;
	case META_DATA_DOWNLOAD_REQUEST:
		generalControlMachine.metaDataDownload(responseData);
		
		retFrameData = &generalControlMachine.metaDataDownloadResult(responseData);
		break;
	case META_DATA_CHECK_REQUEST:
		generalControlMachine.metaDataCheck(responseData);
		
		retFrameData = &generalControlMachine.metaDataCheckResult(responseData);
		break;
	case META_DATA_LOAD_REQUEST:
		generalControlMachine.metaDataLoading(responseData);
		
		retFrameData = &generalControlMachine.metaDataLoadingResult(responseData);
		break;
	case SIMULATION_FLIGHT_DATA_UPLOAD_REQUEST:
		generalControlMachine.simulationFlightDataUpload(responseData);
		
		retFrameData = &generalControlMachine.simulationFlightDataUploadResult(responseData);
		break;
	case EXPLOSIVE_PORT_CHECK_REQUEST:
		generalControlMachine.explosivePortTest(responseData);
		
		retFrameData = &generalControlMachine.explosivePortTestResult(responseData);
		break;
	case EXPLOSIVE_RESIST_MEASURE_REQUEST:
		generalControlMachine.explosiveResistMeasure(responseData);
		
		retFrameData = &generalControlMachine.explosiveResistMeasureResult(responseData);
		break;
	case LEVEL_COMPUTE_REQUEST:
		generalControlMachine.levelCompute(responseData);
		
		retFrameData = &generalControlMachine.levelComputeResult(responseData);
		break;
	case TRUN_NAVIGATE_REQUEST:
		generalControlMachine.trunNavigate(responseData);
		
		retFrameData = &generalControlMachine.trunNavigateResult(responseData);
		break;
	case PRE_LUANCH_COMMAND:
		generalControlMachine.preLaunchCommand(responseData);
		
		retFrameData = &generalControlMachine.preLaunchCommandResult(responseData);
		break;
	case LUANCH_FIRE_COMMAND:
		generalControlMachine.launchFireCommand(responseData);
		return nullptr;
		break;
	case SAFE_CONTROL_PRE_COMMAND:
		generalControlMachine.safeControlPreCommand(responseData);
		return nullptr;
		break;
	case SAFE_CONTROL_ACTIVATE_COMMAND:
		generalControlMachine.safeControlActivateCommand(responseData);
		return nullptr;
		break;
		
	default:
		return nullptr;
	}
	
	QByteArray retFrameDataByte;
	retFrameDataByte.append(retFrameData->HDA);
	retFrameDataByte.append(retFrameData->HDB);
	retFrameDataByte.append(retFrameData->LEN);
	retFrameDataByte.append(retFrameData->SEQ);
	retFrameDataByte.append(retFrameData->DEV);
	retFrameDataByte.append(retFrameData->MSG);
	for (int i=0;i<255;i++)
	{
		retFrameDataByte.append(retFrameData->PAYLOAD[i]);
	}
	retFrameDataByte.append(retFrameData->CKA);
	retFrameDataByte.append(retFrameData->CKB);

	return retFrameDataByte;

}

