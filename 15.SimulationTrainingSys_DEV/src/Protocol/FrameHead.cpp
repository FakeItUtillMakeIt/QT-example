#include "FrameHead.h"
#include <stdio.h> 
#include <boost/date_time/posix_time/posix_time.hpp>


FrameHead::FrameHead() 
	: m_iFrameSync1(FRAMEHEAD_SYNC_1)
	, m_iFrameSync2(FRAMEHEAD_SYNC_2)
	, m_iFrameLen(FRAMEHEAD_LENGTH)
	, m_iFrameType(FRAME_TYPE_UNDEFINE)
	, m_iYear(2016)
	, m_iMonth(12)
	, m_iDay(5)
	, m_ullTime(0)
	, m_iFrameCount(0)
	, m_iParamCount(0)
	, m_iCheckSum(0)
{
	//获取当前时间
	using namespace boost::posix_time;
	ptime now = microsec_clock::local_time();
	tm t = to_tm(now);
	m_iYear = t.tm_year + 1900;
    m_iMonth = t.tm_mon + 1;
    m_iDay = t.tm_mday;
	//当天的纳秒数
	time_duration td = now.time_of_day();
	m_ullTime = (unsigned long long)td.total_milliseconds()*1000;
}

FrameHead::FrameHead(const FrameHead& head)
{
	this->m_iFrameSync1 = head.FrameSync1();
	this->m_iFrameSync2 = head.FrameSync2();
	this->m_iFrameLen = head.FrameLen();
	this->m_iFrameType = head.FrameType();
	this->m_iYear = head.Year();
	this->m_iMonth = head.Month();
	this->m_iDay = head.Day();
	this->m_ullTime = head.Time();
	this->m_iFrameCount = head.FrameCount();
	this->m_iParamCount = head.ParamCount();
	this->m_iCheckSum = head.CheckSum();
}

FrameHead& FrameHead::operator=(const FrameHead& head)
{
	this->m_iFrameSync1 = head.FrameSync1();
	this->m_iFrameSync2 = head.FrameSync2(); 
	this->m_iFrameLen = head.FrameLen();
	this->m_iFrameType = head.FrameType(); 
	this->m_iYear = head.Year();
	this->m_iMonth = head.Month();
	this->m_iDay = head.Day();
	this->m_ullTime = head.Time();
	this->m_iFrameCount = head.FrameCount();
	this->m_iParamCount = head.ParamCount();
	this->m_iCheckSum = head.CheckSum();
	return *this;
}

FrameHead::~FrameHead() 
{
}

bool FrameHead::DeSerialize(unsigned char* const buff)
{
    //校验帧头
	if (buff[FRAMEHEAD_ADDR_SYNC] != FRAMEHEAD_SYNC_1)
	{
		m_error_info = "FRAME INIT ERROR: no match head_sync 1";
		return false;
	}

	if (buff[FRAMEHEAD_ADDR_SYNC + 1] != FRAMEHEAD_SYNC_2)
	{
		m_error_info = "FRAME INIT ERROR: no match head_sync 2";
		return false;
	}

	m_iFrameSync1 = FRAMEHEAD_SYNC_1;
	m_iFrameSync2 = FRAMEHEAD_SYNC_2; 
    //帧长 
	memcpy(&m_iFrameLen, buff + FRAMEHEAD_ADDR_LENGTH, 2);
    //帧类型
	m_iFrameType  = buff[FRAMEHEAD_ADDR_TYPE]; 
	memcpy(&m_iYear, buff + FRAMEHEAD_ADDR_YEAR, 2);
	m_iMonth    = buff[FRAMEHEAD_ADDR_MONTH];
    m_iDay      = buff[FRAMEHEAD_ADDR_DAY]; 
	memcpy(&m_ullTime, buff + FRAMEHEAD_ADDR_TIME, 8); 
	memcpy(&m_iFrameCount, (buff + FRAMEHEAD_ADDR_COUNT), 4);
	memcpy(&m_iParamCount, (buff + FRAMEHEAD_ADDR_PARAM_NUM), 4);
	return true;
}

bool FrameHead::Serialize(unsigned char* const buff)
{
    //帧头
	buff[FRAMEHEAD_ADDR_SYNC] = FRAMEHEAD_SYNC_1;
	buff[FRAMEHEAD_ADDR_SYNC + 1] = FRAMEHEAD_SYNC_2;
    //帧长
	memcpy(buff + FRAMEHEAD_ADDR_LENGTH, &m_iFrameLen, sizeof(unsigned short));
    //帧类型
	buff[FRAMEHEAD_ADDR_TYPE] = m_iFrameType; 
    //年
	memcpy((buff + FRAMEHEAD_ADDR_YEAR), &m_iYear, sizeof(unsigned short));
    //月
	buff[FRAMEHEAD_ADDR_MONTH] = m_iMonth;
    //日
	buff[FRAMEHEAD_ADDR_DAY] = m_iDay;
    //时间
	memcpy((buff + FRAMEHEAD_ADDR_TIME), &m_ullTime, sizeof(unsigned long long));
    //帧计数
	memcpy((buff + FRAMEHEAD_ADDR_COUNT), &m_iFrameCount, sizeof(unsigned long));
    //信息字个数
	memcpy((buff + FRAMEHEAD_ADDR_PARAM_NUM), &m_iParamCount, sizeof(unsigned short));
    //校验和
	memset((buff + FRAMEHEAD_ADDR_CHECKSUM), 0, 1);
	return true;
}

int FrameHead::Size() 
{ 
	return FRAMEHEAD_LENGTH;
} 