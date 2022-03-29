#include "TestData.h"
#include "crc_onespace.h"

namespace TestData {


GeneralMacInsideVoltAndExplsResTestData::GeneralMacInsideVoltAndExplsResTestData(){
	testData = new BaseProtocolFormat;

	testData->HDA = 0x55;
	testData->HDB = 0xaa;
    testData->LEN = 0xff;
	testData->DEV = 0x03;
	testData->MSG = 0x05;
	for (int i = 0; i < 255; i++)
	{
		testData->PAYLOAD[i] = 0xbb;
	}

    QByteArray m_data;

    unsigned char currentData[259];
    currentData[0]=testData->LEN;
    currentData[1]=testData->SEQ;
    currentData[2]=testData->DEV;
    currentData[3]=testData->MSG;
    for(int i=0;i<255;i++){
        currentData[4+i]=testData->PAYLOAD[i];
    }


    unsigned short crc_cal_onespace=0;

    crc_cal_onespace=CRC_ONESPACE::cal_CRC((char*)currentData,259);


    testData->CKA = crc_cal_onespace >> 0;//低八位
    testData->CKB = crc_cal_onespace >> 8;//高八位

}

BaseProtocolFormat* GeneralMacInsideVoltAndExplsResTestData::getTestData() {
	return testData;
}


SerialPortCommTestData::SerialPortCommTestData() {
	testData = new BaseProtocolFormat;
	testData->HDA = 0x55;
	testData->HDB = 0xaa;
    testData->LEN = 0xff;
	testData->DEV = 0x03;
	testData->MSG = 0x07;
	testData->PAYLOAD[0] = 0x11;
    testData->PAYLOAD[1] = 0x22;
    testData->PAYLOAD[2] = 0x33;
    testData->PAYLOAD[3] = 0x44;
    testData->PAYLOAD[4] = 0x55;
	for (int i = 5; i < 255; i++)
	{
		testData->PAYLOAD[i] = 0xbb;
	}

    QByteArray currentData;
    currentData.append(testData->LEN);
    currentData.append(testData->SEQ);
    currentData.append(testData->DEV);
    currentData.append(testData->MSG);
    for(int i=0;i<255;i++){
        currentData.append(testData->PAYLOAD[i]);
    }


    unsigned short crc_cal_onespace;
    crc_cal_onespace=CRC_ONESPACE::cal_CRC((char*)(currentData.data()),259);

    testData->CKA = crc_cal_onespace >> 0;//低八位
    testData->CKB = crc_cal_onespace >> 8;//高八位
}

BaseProtocolFormat* SerialPortCommTestData::getTestData() {
	return testData;
}

MutliSerialPort1CommTestData::MutliSerialPort1CommTestData() {
    testData = new BaseProtocolFormat;
    testData->HDA = 0x55;
    testData->HDB = 0xaa;
    testData->LEN = 0xff;
    testData->DEV = 0x03;
    testData->MSG = 0x07;
    testData->PAYLOAD[0] = 0x11;
    testData->PAYLOAD[1] = 0x22;
    testData->PAYLOAD[2] = 0x33;
    testData->PAYLOAD[3] = 0x44;
    testData->PAYLOAD[4] = 0x55;
    testData->PAYLOAD[5] = 0x10;
    for (int i = 6; i < 255; i++)
    {
        testData->PAYLOAD[i] = 0xbb;
    }

    QByteArray currentData;
    currentData.append(testData->LEN);
    currentData.append(testData->SEQ);
    currentData.append(testData->DEV);
    currentData.append(testData->MSG);
    for(int i=0;i<255;i++){
        currentData.append(testData->PAYLOAD[i]);
    }


    unsigned short crc_cal_onespace;
    crc_cal_onespace=CRC_ONESPACE::cal_CRC((char*)(currentData.data()),259);

    testData->CKA = crc_cal_onespace >> 0;//低八位
    testData->CKB = crc_cal_onespace >> 8;//高八位
}

BaseProtocolFormat* MutliSerialPort1CommTestData::getTestData() {
    return testData;
}

MutliSerialPort2CommTestData::MutliSerialPort2CommTestData() {
    testData = new BaseProtocolFormat;
    testData->HDA = 0x55;
    testData->HDB = 0xaa;
    testData->LEN = 0xff;
    testData->DEV = 0x03;
    testData->MSG = 0x07;
    testData->PAYLOAD[0] = 0x11;
    testData->PAYLOAD[1] = 0x22;
    testData->PAYLOAD[2] = 0x33;
    testData->PAYLOAD[3] = 0x44;
    testData->PAYLOAD[4] = 0x55;
    testData->PAYLOAD[5] = 0x13;
    for (int i = 6; i < 255; i++)
    {
        testData->PAYLOAD[i] = 0xbb;
    }

    QByteArray currentData;
    currentData.append(testData->LEN);
    currentData.append(testData->SEQ);
    currentData.append(testData->DEV);
    currentData.append(testData->MSG);
    for(int i=0;i<255;i++){
        currentData.append(testData->PAYLOAD[i]);
    }


    unsigned short crc_cal_onespace;
    crc_cal_onespace=CRC_ONESPACE::cal_CRC((char*)(currentData.data()),259);

    testData->CKA = crc_cal_onespace >> 0;//低八位
    testData->CKB = crc_cal_onespace >> 8;//高八位
}

BaseProtocolFormat* MutliSerialPort2CommTestData::getTestData() {
    return testData;
}

MutliSerialPort3CommTestData::MutliSerialPort3CommTestData() {
    testData = new BaseProtocolFormat;
    testData->HDA = 0x55;
    testData->HDB = 0xaa;
    testData->LEN = 0xff;
    testData->DEV = 0x03;
    testData->MSG = 0x07;
    testData->PAYLOAD[0] = 0x11;
    testData->PAYLOAD[1] = 0x22;
    testData->PAYLOAD[2] = 0x33;
    testData->PAYLOAD[3] = 0x44;
    testData->PAYLOAD[4] = 0x55;
    testData->PAYLOAD[5] = 0x14;
    for (int i = 6; i < 255; i++)
    {
        testData->PAYLOAD[i] = 0xbb;
    }

    QByteArray currentData;
    currentData.append(testData->LEN);
    currentData.append(testData->SEQ);
    currentData.append(testData->DEV);
    currentData.append(testData->MSG);
    for(int i=0;i<255;i++){
        currentData.append(testData->PAYLOAD[i]);
    }


    unsigned short crc_cal_onespace;
    crc_cal_onespace=CRC_ONESPACE::cal_CRC((char*)(currentData.data()),259);

    testData->CKA = crc_cal_onespace >> 0;//低八位
    testData->CKB = crc_cal_onespace >> 8;//高八位
}

BaseProtocolFormat* MutliSerialPort3CommTestData::getTestData() {
    return testData;
}


AnalogCaptureTestData::AnalogCaptureTestData() {
	testData = new BaseProtocolFormat;
	testData->HDA = 0x55;
	testData->HDB = 0xaa;
    testData->LEN = 0xff;
	testData->DEV = 0x03;
	testData->MSG = 0x09;
	for (int i = 0; i < 255; i++)
	{
		testData->PAYLOAD[i] = 0xbb;
	}
    unsigned char currentData[259];
    currentData[0]=testData->LEN;
    currentData[1]=testData->SEQ;
    currentData[2]=testData->DEV;
    currentData[3]=testData->MSG;
    for(int i=0;i<255;i++){
        currentData[4+i]=testData->PAYLOAD[i];
    }


    unsigned short crc_cal_onespace;
    crc_cal_onespace=CRC_ONESPACE::cal_CRC((char*)currentData,259);

    testData->CKA = crc_cal_onespace >> 0;//低八位
    testData->CKB = crc_cal_onespace >> 8;//高八位
}

BaseProtocolFormat* AnalogCaptureTestData::getTestData() {
	return testData;
}

/**
    @brief SwitchOutputTestData object constructor 
**/
SwitchOutputTestData::SwitchOutputTestData() {
	testDataA = new BaseProtocolFormat;
	testDataB = new BaseProtocolFormat;
	testDataA->HDA = 0x55;
	testDataA->HDB = 0xaa;
    testDataA->LEN = 0xff;
	testDataA->DEV = 0x03;
	testDataA->MSG = 0x0c;
	testDataA->PAYLOAD[0] = 0x11;
	for (int i = 1; i < 255; i++)
	{
		testDataA->PAYLOAD[i] = 0xbb;
	}
    unsigned char currentDataA[259];
    currentDataA[0]=testDataA->LEN;
    currentDataA[1]=testDataA->SEQ;
    currentDataA[2]=testDataA->DEV;
    currentDataA[3]=testDataA->MSG;
    for(int i=0;i<255;i++){
        currentDataA[4+i]=testDataA->PAYLOAD[i];
    }


    unsigned short crc_cal_onespace;
    crc_cal_onespace=CRC_ONESPACE::cal_CRC((char*)currentDataA,259);

    testDataA->CKA = crc_cal_onespace >> 0;//低八位
    testDataA->CKB = crc_cal_onespace >> 8;//高八位

	testDataB->HDA = 0x55;
	testDataB->HDB = 0xaa;
    testDataB->LEN = 0xff;
	testDataB->DEV = 0x03;
	testDataB->MSG = 0x0c;
    testDataB->PAYLOAD[0] = 0x12;
	for (int i = 1; i < 255; i++)
	{
		testDataB->PAYLOAD[i] = 0xbb;
	}
	
    unsigned char currentDataB[259];
    currentDataB[0]=testDataB->LEN;
    currentDataB[1]=testDataB->SEQ;
    currentDataB[2]=testDataB->DEV;
    currentDataB[3]=testDataB->MSG;
    for(int i=0;i<255;i++){
        currentDataB[4+i]=testDataB->PAYLOAD[i];
    }


    //quint16 crc_cal = crc16.crcCompute((char*)(testData->PAYLOAD), 255);
    //quint16 crc_calB = crc16.crcCompute((char*)(currentDataB), 259);
    unsigned short crc_cal_onespaceB;
    crc_cal_onespaceB=CRC_ONESPACE::cal_CRC((char*)currentDataB,259);

    testDataB->CKA = crc_cal_onespaceB >> 0;//低八位
    testDataB->CKB = crc_cal_onespaceB >> 8;//高八位
}

/**
    @brief  零秒
    @retval 
**/
BaseProtocolFormat* SwitchOutputTestData::getTestDataA() {
	return testDataA;
}

/**
    @brief  解除自毁
    @retval 
**/
BaseProtocolFormat* SwitchOutputTestData::getTestDataB() {
	return testDataB;
}


SwitchInputTestData::SwitchInputTestData() {
	testData = new BaseProtocolFormat;
	testData->HDA = 0x55;
	testData->HDB = 0xaa;
    testData->LEN = 0xff;
    testData->DEV = 0x10;
	testData->MSG = 0x0e;
	for (int i = 0; i < 255; i++)
	{
		testData->PAYLOAD[i] = 0xbb;
	}
    unsigned char currentData[259];
    currentData[0]=testData->LEN;
    currentData[1]=testData->SEQ;
    currentData[2]=testData->DEV;
    currentData[3]=testData->MSG;
    for(int i=0;i<255;i++){
        currentData[4+i]=testData->PAYLOAD[i];
    }


    unsigned short crc_cal_onespace;
    crc_cal_onespace=CRC_ONESPACE::cal_CRC((char*)currentData,259);

    testData->CKA = crc_cal_onespace >> 0;//低八位
    testData->CKB = crc_cal_onespace >> 8;//高八位
}

BaseProtocolFormat* SwitchInputTestData::getTestData() {
	return testData;
}

SequentialOutputTestData::SequentialOutputTestData() {
    sequentialOutputTest1ONData = new BaseProtocolFormat;
    sequentialOutputTest1OFFData = new BaseProtocolFormat;
    sequentialOutputTest2ONData = new BaseProtocolFormat;
    sequentialOutputTest2OFFData = new BaseProtocolFormat;

    sequentialOutputTest1ONData->HDA = 0x55;
    sequentialOutputTest1ONData->HDB = 0xaa;
    sequentialOutputTest1ONData->LEN = 0xff;
    sequentialOutputTest1ONData->DEV = 0x12;
    sequentialOutputTest1ONData->MSG = 0xd0;

    sequentialOutputTest1OFFData->HDA = 0x55;
    sequentialOutputTest1OFFData->HDB = 0xaa;
    sequentialOutputTest1OFFData->LEN = 0xff;
    sequentialOutputTest1OFFData->DEV = 0x12;
    sequentialOutputTest1OFFData->MSG = 0xd4;

    sequentialOutputTest2ONData->HDA = 0x55;
    sequentialOutputTest2ONData->HDB = 0xaa;
    sequentialOutputTest2ONData->LEN = 0xff;
    sequentialOutputTest2ONData->DEV = 0x12;
    sequentialOutputTest2ONData->MSG = 0xd5;

    sequentialOutputTest2OFFData->HDA = 0x55;
    sequentialOutputTest2OFFData->HDB = 0xaa;
    sequentialOutputTest2OFFData->LEN = 0xff;
    sequentialOutputTest2OFFData->DEV = 0x12;
    sequentialOutputTest2OFFData->MSG = 0xd6;
	for (int i = 0; i < 255; i++)
	{
        sequentialOutputTest1ONData->PAYLOAD[i] = 0xbb;
        sequentialOutputTest1OFFData->PAYLOAD[i] = 0xbb;
        sequentialOutputTest2ONData->PAYLOAD[i] = 0xbb;
        sequentialOutputTest2OFFData->PAYLOAD[i] = 0xbb;
	}
    char current1ONData[259];
    current1ONData[0]=sequentialOutputTest1ONData->LEN;
    current1ONData[1]=sequentialOutputTest1ONData->SEQ;
    current1ONData[2]=sequentialOutputTest1ONData->DEV;
    current1ONData[3]=sequentialOutputTest1ONData->MSG;

    char current1OFFData[259];
    current1OFFData[0]=sequentialOutputTest1OFFData->LEN;
    current1OFFData[1]=sequentialOutputTest1OFFData->SEQ;
    current1OFFData[2]=sequentialOutputTest1OFFData->DEV;
    current1OFFData[3]=sequentialOutputTest1OFFData->MSG;

    char current2ONData[259];
    current2ONData[0]=sequentialOutputTest2ONData->LEN;
    current2ONData[1]=sequentialOutputTest2ONData->SEQ;
    current2ONData[2]=sequentialOutputTest2ONData->DEV;
    current2ONData[3]=sequentialOutputTest2ONData->MSG;

    char current2OFFData[259];
    current2OFFData[0]=sequentialOutputTest2OFFData->LEN;
    current2OFFData[1]=sequentialOutputTest2OFFData->SEQ;
    current2OFFData[2]=sequentialOutputTest2OFFData->DEV;
    current2OFFData[3]=sequentialOutputTest2OFFData->MSG;
    for(int i=0;i<255;i++){
        current1ONData[4+i]=sequentialOutputTest1ONData->PAYLOAD[i];
        current1OFFData[4+i]=sequentialOutputTest1OFFData->PAYLOAD[i];
        current2ONData[4+i]=sequentialOutputTest2ONData->PAYLOAD[i];
        current2OFFData[4+i]=sequentialOutputTest2OFFData->PAYLOAD[i];
    }

    unsigned short crc_cal_onespace1ON;
    crc_cal_onespace1ON=CRC_ONESPACE::cal_CRC(current1ONData,259);

    sequentialOutputTest1ONData->CKA = crc_cal_onespace1ON >> 0;//低八位
    sequentialOutputTest1ONData->CKB = crc_cal_onespace1ON >> 8;//高八位

    unsigned short crc_cal_onespace1OFF;
    crc_cal_onespace1OFF=CRC_ONESPACE::cal_CRC(current1OFFData,259);

    sequentialOutputTest1OFFData->CKA = crc_cal_onespace1OFF >> 0;//低八位
    sequentialOutputTest1OFFData->CKB = crc_cal_onespace1OFF >> 8;//高八位

    unsigned short crc_cal_onespace2ON;
    crc_cal_onespace2ON=CRC_ONESPACE::cal_CRC(current2ONData,259);

    sequentialOutputTest2ONData->CKA = crc_cal_onespace2ON >> 0;//低八位
    sequentialOutputTest2ONData->CKB = crc_cal_onespace2ON >> 8;//高八位

    unsigned short crc_cal_onespace2OFF;
    crc_cal_onespace2OFF=CRC_ONESPACE::cal_CRC(current2OFFData,259);

    sequentialOutputTest2OFFData->CKA = crc_cal_onespace2OFF >> 0;//低八位
    sequentialOutputTest2OFFData->CKB = crc_cal_onespace2OFF >> 8;//高八位
}

BaseProtocolFormat* SequentialOutputTestData::getSequentialOutputTest1ONData() {
    return sequentialOutputTest1ONData;
}

BaseProtocolFormat* SequentialOutputTestData::getSequentialOutputTest1OFFData() {
    return sequentialOutputTest1OFFData;
}

BaseProtocolFormat* SequentialOutputTestData::getSequentialOutputTest2ONData() {
    return sequentialOutputTest2ONData;
}

BaseProtocolFormat* SequentialOutputTestData::getSequentialOutputTest2OFFData() {
    return sequentialOutputTest2OFFData;
}


SatelliteNavgAndRemoteMeasTestData::SatelliteNavgAndRemoteMeasTestData() {
	testData = new BaseProtocolFormat;
	testData->HDA = 0x55;
	testData->HDB = 0xaa;
    testData->LEN = 0xff;
    testData->DEV = 0x14;
	testData->MSG = 0xd2;
	for (int i = 0; i < 255; i++)
	{
		testData->PAYLOAD[i] = 0xbb;
	}
    char currentData[259];
    currentData[0]=testData->LEN;
    currentData[1]=testData->SEQ;
    currentData[2]=testData->DEV;
    currentData[3]=testData->MSG;
    for(int i=0;i<255;i++){
        currentData[4+i]=testData->PAYLOAD[i];
    }


    unsigned short crc_cal_onespace;
    crc_cal_onespace=CRC_ONESPACE::cal_CRC(currentData,259);

    testData->CKA = crc_cal_onespace >> 0;//低八位
    testData->CKB = crc_cal_onespace >> 8;//高八位
}

BaseProtocolFormat* SatelliteNavgAndRemoteMeasTestData::getTestData() {
	return testData;
}

NoSourceInputTestData::NoSourceInputTestData(){
    testData = new BaseProtocolFormat;
    testData->HDA = 0x55;
    testData->HDB = 0xaa;
    testData->LEN = 0xff;
    testData->DEV = 0x12;
    testData->MSG = 0xd7;
    for (int i = 0; i < 255; i++)
    {
        testData->PAYLOAD[i] = 0xbb;
    }
    char currentData[259];
    currentData[0]=testData->LEN;
    currentData[1]=testData->SEQ;
    currentData[2]=testData->DEV;
    currentData[3]=testData->MSG;
    for(int i=0;i<255;i++){
        currentData[4+i]=testData->PAYLOAD[i];
    }


    unsigned short crc_cal_onespace;
    crc_cal_onespace=CRC_ONESPACE::cal_CRC(currentData,259);

    testData->CKA = crc_cal_onespace >> 0;//低八位
    testData->CKB = crc_cal_onespace >> 8;//高八位
}

BaseProtocolFormat* NoSourceInputTestData::getTestData() {
    return testData;
}



void RetTestResultValidate::retTestDataValidate(BaseProtocolFormat* data, int& testType) {
	int devVal = 0;
	int msgVal = 0;
	devVal = data->DEV;
	msgVal = data->MSG;
    testType = msgVal;
}

}
