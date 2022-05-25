#include "RocketGroundProtolDataFormat.h"


char DataFormatTransform::byte2ascii(byte data) {

	return NULL;
}

QByteArray DataFormatTransform::byte2QByteArray(RocketGroundProtolDataFormat input) {
	QByteArray retData;
	//itoa
	char temp[1];
	itoa(input.HDA,temp,16);
	retData.append(temp[0]);
	itoa(input.HDB, temp, 16);
	retData.append(temp[0]);
	itoa(input.LEN, temp, 16);
	retData.append(temp[0]);
	itoa(input.SEQ, temp, 16);
	retData.append(temp[0]);
	itoa(input.DEV, temp, 16);
	retData.append(temp[0]);
	itoa(input.MSG, temp, 16);
	retData.append(temp[0]);

	for (int i=0;i<255;i++)
	{
		itoa(input.PAYLOAD[i], temp, 16);
		retData.append(temp[0]);
	}

	itoa(input.CKA, temp, 16);
	retData.append(temp[0]);
	itoa(input.CKB, temp, 16);
	retData.append(temp[0]);

	return retData;

}

RocketGroundProtolDataFormat DataFormatTransform::qbyteArray2byte(const QByteArray& input) {
	//×Ö·û´®×ªlong
	//atoi
	RocketGroundProtolDataFormat retData;
	char buff[1];
	buff[0] = input[0];
	retData.HDA = atoi(buff);
	buff[0] = input[1];
	retData.HDB = atoi(buff);
	buff[0] = input[2];
	retData.LEN = atoi(buff);
	buff[0] = input[3];
	retData.SEQ = atoi(buff);
	buff[0] = input[4];
	retData.DEV = atoi(buff);
	buff[0] = input[5];
	retData.MSG = atoi(buff);

	for (int i = 0; i < 255; i++) {
		buff[0] = input[i+6];
		retData.PAYLOAD[i] = atoi(buff);
		
	}
	buff[0] = input[261];
	retData.CKA = atoi(buff);
	buff[0] = input[262];
	retData.CKB = atoi(buff);

	return retData;

}
