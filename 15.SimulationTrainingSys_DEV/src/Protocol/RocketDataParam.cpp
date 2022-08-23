#include "RocketDataParam.h"
 
RocketDataParam::RocketDataParam()
	: m_id(0)
	, m_rocketDataId(0)
	, m_index(0)
	, m_length(0)
{ 
} 
RocketDataParam::~RocketDataParam()
{
}

void RocketDataParam::DeSerialize(unsigned char* const buff)
{ 
	memcpy(&m_id, buff + PARAM_ADDR_ID, 4);
	memcpy(&m_deviceParam->m_Validity, buff + PARAM_ADDR_Valid, 4);
	memcpy(&m_deviceParam->m_iCurValue, buff + PARAM_ADDR_VALUE, 8);
	m_deviceParam->setStatus(buff[PARAM_ADDR_STATUS]);
}

void RocketDataParam::Serialize(unsigned char* const buff)
{
	int len = sizeof(double);
	memcpy(buff + PARAM_ADDR_ID, &m_id, 4);
	memcpy(buff + PARAM_ADDR_Valid, &m_deviceParam->m_Validity, 4);
	memcpy(buff + PARAM_ADDR_VALUE, &m_deviceParam->m_iCurValue, 8);
	memcpy(buff + PARAM_ADDR_STATUS, &m_deviceParam->m_curStatus.m_id, 1);
}
 
int RocketDataParam::Size()
{
	return PARAM_LENGTH;
}
 


