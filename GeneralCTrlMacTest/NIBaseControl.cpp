#include "NIBaseControl.h"
#include "iostream"
using namespace std;


namespace NIControl {

	NIBaseControl::NIBaseControl() {
		 error = 0;
		 powerOnHandle = nullptr;
		 powerOffHandle = nullptr;
		 powerOnData = 0;
		 powerOffData = 1;
		 for (int i=0;i<2048;i++)
		 {
			 errBuff[i] = 0;
		 }
		 read=0x00;

	}

	NIBaseControl::~NIBaseControl() {
		powerOffHandle = nullptr;
		powerOnHandle = nullptr;
	}

	int NIBaseControl::digitalPortWriteForPowerOn() {
		powerOnHandle = nullptr;
		//DAQmx 配置  创建任务、创建通道
		DAQmxErrChk(DAQmxCreateTask("powerOn", &powerOnHandle));
		DAQmxErrChk(DAQmxCreateDOChan(powerOnHandle, "Dev1/port5/line7", "", DAQmx_Val_ChanForAllLines));

		//DAQmx start
		DAQmxErrChk(DAQmxStartTask(powerOnHandle));

		int ret=(DAQmxWriteDigitalU32(powerOnHandle, 1, 1, 10.0, DAQmx_Val_GroupByChannel, &powerOnData, &written, NULL));
		if (ret==0)
		{
			DAQmxStopTask(powerOnHandle);
			DAQmxClearTask(powerOnHandle);
			return 0;
		}
	Error:
		if (DAQmxFailed(error))
			DAQmxGetExtendedErrorInfo(errBuff, 2048);
		if (powerOnHandle != 0) {
			/*********************************************/
			// DAQmx Stop Code
			/*********************************************/
			DAQmxStopTask(powerOnHandle);
			DAQmxClearTask(powerOnHandle);
		}
		if (DAQmxFailed(error))
			printf("DAQmx Error: %s\n", errBuff);
		printf("End of program, press Enter key to quit\n");
		getchar();
		return -1;
		
	}

	int NIBaseControl::digitalPortWriteForPowerOff() {
		powerOffHandle = nullptr;
		//DAQmx 配置  创建任务、创建通道
		DAQmxErrChk(DAQmxCreateTask("powerOff", &powerOffHandle));
		DAQmxErrChk(DAQmxCreateDOChan(powerOffHandle, "Dev1/port5/line6", "", DAQmx_Val_ChanForAllLines));

		//DAQmx start
		DAQmxErrChk(DAQmxStartTask(powerOffHandle));

		int ret = (DAQmxWriteDigitalU32(powerOffHandle, 1, 1, 10.0, DAQmx_Val_GroupByChannel, &powerOffData, &written, NULL));
		if (ret == 0)
		{
			DAQmxStopTask(powerOffHandle);
			DAQmxClearTask(powerOffHandle);
			return 0;
		}
	Error:
		if (DAQmxFailed(error))
			DAQmxGetExtendedErrorInfo(errBuff, 2048);
		if (powerOffHandle != 0) {
			/*********************************************/
			// DAQmx Stop Code
			/*********************************************/
			DAQmxStopTask(powerOffHandle);
			DAQmxClearTask(powerOffHandle);
		}
		if (DAQmxFailed(error))
			printf("DAQmx Error: %s\n", errBuff);
		printf("End of program, press Enter key to quit\n");
		getchar();
		return -1;
		
	}

	int NIBaseControl::actucalWriteSamples() {
		return written;
	}
 /**
     @brief  写数据  格式为 设备/端口/line
     @param  writeData 
     @retval           
 **/
	int NIBaseControl::writeData2PortBit(uInt8 writeData,const char* taskName,const char* writePortBit) {

        TaskHandle Handle = nullptr;
		//DAQmx 配置  创建任务、创建通道
        DAQmxErrChk(DAQmxCreateTask(taskName, &Handle));
        DAQmxErrChk(DAQmxCreateDOChan(Handle, writePortBit, "", DAQmx_Val_ChanPerLine));

		//DAQmx start
        DAQmxErrChk(DAQmxStartTask(Handle));

        //int ret = (DAQmxWriteDigitalU8(Handle, 1, 1, 10, DAQmx_Val_GroupByChannel, &writeData, &written, NULL));
        int ret=DAQmxWriteDigitalLines(Handle,1,1,10.0,DAQmx_Val_GroupByChannel,&writeData,&written,NULL);
        if (ret == 0)
		{

            DAQmxStopTask(Handle);
            DAQmxClearTask(Handle);
			return 0;
		}
	Error:
		if (DAQmxFailed(error))
			DAQmxGetExtendedErrorInfo(errBuff, 2048);
        if (Handle != 0) {
			/*********************************************/
			// DAQmx Stop Code
			/*********************************************/
            DAQmxStopTask(Handle);
            DAQmxClearTask(Handle);
		}
		if (DAQmxFailed(error))
			printf("DAQmx Error: %s\n", errBuff);
		printf("End of program, press Enter key to quit\n");
		getchar();
		return -1;
	}

    int NIBaseControl::writeData2PortBit32(uInt32 writeData, const char* taskName, const char* writePortBit){
        TaskHandle Handle = nullptr;
        //DAQmx 配置  创建任务、创建通道
        DAQmxErrChk(DAQmxCreateTask(taskName, &Handle));
        DAQmxErrChk(DAQmxCreateDOChan(Handle, writePortBit, "", DAQmx_Val_ChanForAllLines));

        //DAQmx start
        DAQmxErrChk(DAQmxStartTask(Handle));

        //int ret = (DAQmxWriteDigitalU8(Handle, 1, 1, 10, DAQmx_Val_GroupByChannel, &writeData, &written, NULL));
        int ret=DAQmxWriteDigitalU32(Handle,1,1,10.0,DAQmx_Val_GroupByChannel,&writeData,&written,NULL);
        if (ret == 0)
        {

            DAQmxStopTask(Handle);
            DAQmxClearTask(Handle);
            return 0;
        }
    Error:
        if (DAQmxFailed(error))
            DAQmxGetExtendedErrorInfo(errBuff, 2048);
        if (Handle != 0) {
            /*********************************************/
            // DAQmx Stop Code
            /*********************************************/
            DAQmxStopTask(Handle);
            DAQmxClearTask(Handle);
        }
        if (DAQmxFailed(error))
            printf("DAQmx Error: %s\n", errBuff);
        printf("End of program, press Enter key to quit\n");
        getchar();
        return -1;
    }

 /**
     @brief  读数据  格式为 设备/端口/line
     @param  readData    
     @param  taskName    
     @param  readPortBit 
     @retval             
 **/
	int NIBaseControl::readDataFromPortBit(uInt8* readData, const char* taskName, const char* readPortBit) {
        TaskHandle readHandle = nullptr;
		//DAQmx 配置  创建任务、创建通道
        DAQmxErrChk(DAQmxCreateTask(taskName, &readHandle));
        DAQmxErrChk(DAQmxCreateDIChan(readHandle, readPortBit, "", DAQmx_Val_ChanPerLine));

		//DAQmx start
        DAQmxErrChk(DAQmxStartTask(readHandle));

        int ret = (DAQmxReadDigitalU8(readHandle, 1, 10.0, DAQmx_Val_GroupByChannel, readData,1, &read, NULL));
		if (ret == 0)
		{
            DAQmxStopTask(readHandle);
            DAQmxClearTask(readHandle);
			return 0;
		}
	Error:
		if (DAQmxFailed(error))
			DAQmxGetExtendedErrorInfo(errBuff, 2048);
        if (readHandle != 0) {
			/*********************************************/
			// DAQmx Stop Code
			/*********************************************/
            DAQmxStopTask(readHandle);
            DAQmxClearTask(readHandle);
		}
		if (DAQmxFailed(error))
			printf("DAQmx Error: %s\n", errBuff);
		printf("End of program, press Enter key to quit\n");
		getchar();
		return -1;

	}
}
