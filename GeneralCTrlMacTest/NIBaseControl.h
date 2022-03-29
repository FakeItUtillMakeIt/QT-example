#pragma once

#ifndef NI_BASE_CONTROL_H
#define NI_BASE_CONTROL_H

#include "NIDAQmx.h"
#include "nisyscfg.h"
#include <QTimer>
#define DAQmxErrChk(functionCall) if( DAQmxFailed(error=(functionCall)) ) goto Error; else

namespace NIControl {


//	enum ControlMachine2GeneralMacPortBit
//	{
//		PreDestroy=55,
//		Destroy=54,
//		FLY=53,
//		POWER_ON=57,
//		POWER_OFF=26
//	};

//	enum GeneralMac2ControlMachineSeqOutPortBit
//	{
//		CONTL_01 = 17,
//		CONTL_02 = 16,
//		CONTL_03 = 15,
//		CONTL_04 = 14,
//		CONTL_05 = 13,
//		CONTL_06 = 12,
//		CONTL_07 = 11,
//		CONTL_08 = 10,
//		CONTL_09 = 07,
//		CONTL_10 = 06,
//		CONTL_11 = 05,
//		CONTL_12 = 04,
//		CONTL_13 = 03,
//		CONTL_14 = 02,
//		CONTL_15 = 17,
//		CONTL_16 = 16,
//		CONTL_17 = 15,
//		CONTL_18 = 14,
//		CONTL_19 = 13,
//		CONTL_20 = 12,
//		CONTL_21 = 11,
//		CONTL_22 = 10,
//		CONTL_23 = 07,
//		CONTL_24 = 06,
//		CONTL_25 = 05,
//		CONTL_26 = 04,
//		CONTL_27 = 03,
//		CONTL_28 = 02
//	};

//	enum ControlMac2GeneralMacNonSourceCapturePortBit
//	{
//		STATE_1 = 52,
//		STATE_2 = 51,
//		STATE_3 = 50,
//		STATE_4 = 47,
//		STATE_5 = 46,
//		STATE_6 = 45,
//		STATE_7 = 44,
//		STATE_8 = 43,
//		lingmiao = 27,
//		rmDestroy = 26,
//	};

 /**

     @class   NIBaseControl
     @brief   PXI6528¶Ë¿Ú¿ØÖÆ
     @details ~

 **/
	class NIBaseControl
	{
		int32 error;
		TaskHandle powerOnHandle;
		TaskHandle powerOffHandle;
		uInt32 powerOnData,powerOffData;
		char errBuff[2048];
		int32 read;
		uInt32 arraySizeSamp;
		int32 written;

        QTimer* NITimer;
		
	public:
		NIBaseControl();
		~NIBaseControl();
		int  digitalPortWriteForPowerOn();
		int  digitalPortWriteForPowerOff();

		int writeData2PortBit(uInt8 writeData, const char* taskName, const char* writePortBit);
		int readDataFromPortBit(uInt8* readData, const char* taskName, const char* readPortBit);

        int writeData2PortBit32(uInt32 writeData, const char* taskName, const char* writePortBit);

		int actucalWriteSamples();


	};
}



#endif
