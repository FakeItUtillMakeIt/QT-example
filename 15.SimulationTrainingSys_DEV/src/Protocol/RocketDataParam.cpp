#include "RocketDataParam.h"
 
RocketDataParam::RocketDataParam()
	: m_id(0)
	, m_rocket_data_id(0)
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
	memcpy(&m_deviceParam->m_iCurValue, buff + PARAM_ADDR_VALUE, 4);  
	m_deviceParam->setStatus(buff[PARAM_ADDR_STATUS]);
}

void RocketDataParam::Serialize(unsigned char* const buff)
{
	memcpy(buff + PARAM_ADDR_ID, &m_id, 4);
	memcpy(buff + PARAM_ADDR_VALUE, &m_deviceParam->m_iCurValue, 4);
	memcpy(buff + PARAM_ADDR_STATUS, &m_deviceParam->m_curStatus.m_id, 1);
}
 
int RocketDataParam::Size()
{
	return PARAM_LENGTH;
}
 


