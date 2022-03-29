#pragma once

#ifndef BASE_PROTOCOL_FORMAT_H
#define BASE_PROTOCOL_FORMAT_H

#include "crc_onespace.h"
#define byte unsigned char


class BaseProtocolFormat
{
public:
	byte HDA;
	byte HDB;
	byte LEN;
	byte SEQ;
	byte DEV;
	byte MSG;
	byte PAYLOAD[255];
	byte CKA;
	byte CKB;


};

#endif
